#include "header.h"


block *create_block(int kval, int tag, int type, int offset, long sz, block *next,block* prev) {
  block *tmp = (block *)malloc(sizeof(block));
  tmp->kval = kval;
  tmp->tag = tag;
  tmp->type = type;
  tmp->offset = offset;
  tmp->sz = sz;
  tmp->next = next;
  tmp->prev = prev;
  return tmp;
}


block* insert_usl(block** head,int kval,int tag,int type, int offset,long sz)
{
   if(*head == NULL)
   { (*head) = create_block( kval, tag,  type,  offset,sz, NULL,NULL);
    return *head;
  }
  (*head)->prev = create_block( kval, tag,  type,  offset,sz,*head,NULL);
  (*head) = (*head)->prev;
  return *head;    
}

block* delete_usl(block** head,block* b)
{
  if(head!=NULL && b!= NULL)
  {
    if(b->next)
    b->next->prev = b->prev;
    if(b->prev)
     b->prev->next =b->next;
    else
     *head = b->next;
 }
}


block* delete_asl(SP_heap* hp,block* b,int k)
{
  if(hp!=NULL && b!= NULL)
  {
    if(b->next)
    b->next->prev = b->prev;
    else
     hp->asl_tail[k] = b->prev;
    if(b->prev)
     b->prev->next =b->next;
    else
     hp->asl_head[k] = b->next;
 
 }
}

block* insert_block(block **tail,int kval, int tag, int type, int offset,long sz) {
  if (*tail == NULL) {
    (*tail) = create_block( kval, tag,  type,  offset, sz, NULL,NULL);
    return *tail;
  }
  (*tail)->next = create_block( kval, tag,  type,  offset,sz, NULL,*tail);
  (*tail) = (*tail)->next;
  return *tail;
}

int delete_block(block **head) {
  if (*head == NULL) 
    return 1;
  block *tmp = *head;
  (*head) = (*head)->next;
  if(*head)
  (*head)->prev = NULL;
  free(tmp);
  return 0;
}

void insert(SP_heap* hp,int k,int kval, int tag, int type, int offset,long sz)
{
    block* tmp = insert_block(&(hp->asl_tail[k]),kval, tag, type, offset,sz);
    if(!(hp->asl_head[k]))
      hp->asl_head[k] = tmp;
    
    return;
}

void delete(SP_heap* hp,int k)
{
    if(hp->asl_head[k] == hp->asl_tail[k])
      hp->asl_tail[k] =NULL;
    delete_block(&(hp->asl_head[k]));
    return;
}


int power_2(long n)
{
   return (n&(n-1)) == 0;
}


int get_index_b(int* sizes, int size, int k)
{
   int h,l,m;
   l =0,h=size-1;
   while(l<=h)
   {
      m = l+(h-l)/2;
      if(sizes[m] == k)
          return m;
      if(sizes[m] < k)
         l = m+1;
      else
         h = m-1;
   }
   return -1;
}

void init_SP_heap(int size,SP_heap* hp)
{
   hp->asl_head = (block**)malloc(sizeof(block*)*2*size);
   hp->asl_tail = (block**)malloc(sizeof(block*)*2*size);
   hp->sizes = (int*)malloc(2*size*sizeof(int));
   hp->size = size;
   hp->alloc_SP_heap = hp->req_size = hp->alloc_size = hp->no_alloc = hp->no_dealloc = hp->no_splits= hp->no_recombs = 0;
   int i,j=1,k=3;
   for(i=0;i<2*size;i++){
      hp->asl_head[i] = NULL;
      hp->asl_tail[i] = NULL;
      if(j<k){
        hp->sizes[i] = j;
        j*=2;}
      else
      {
        hp->sizes[i] = k;
        k*=2;  
      }
   }
   hp->usl = NULL;
   hp->base_ptr = malloc(1ll<<size);
   hp->asl_tail[2*hp->size-1] = hp->asl_head[2*hp->size-1] = create_block(hp->size,1,0,0,1ll<<(hp->size),NULL,NULL);
}
int get_index(int size,SP_heap* hp)
{
  for(int i=0;i<2*hp->size;i++)
     if(hp->sizes[i]>=size && hp->asl_head[i])
        return i;
  return -1;
}

void free_SP_heap(SP_heap* hp)
{
   if(hp!= NULL)
   free(hp->base_ptr);
}


void* sp_alloc(int size,SP_heap* hp)
{
   hp->no_alloc++;
   int k = get_index(size,hp);
   if(k==-1)
      {
      //   printf("Memory could not be allocated: (%d,%f,%d %f)\n",size, hp->alloc_SP_heap, 1ll<<(hp->size),100*hp->alloc_SP_heap/(1ll<<(hp->size)));
          printf("Memory could not be allocated: (%d,%f)\n",size,100*hp->alloc_SP_heap/(1ll<<(hp->size)));
         return NULL;
      }   
   int f= 1,j,i,s,s1,s2;
   block* tmp;
   while( f )
   {
       s = hp->sizes[k];
       
      if(power_2(s))
      {
         if(size>3*(s/4))
            f = 0;
         else
          {
             s1 = 3*(s/4);
             s2 = s/4;
             hp->no_splits++;
             tmp = hp->asl_head[k];
             insert(hp,k-1,tmp->kval-2,1,3,tmp->offset,tmp->sz);
             insert(hp,k-4,tmp->kval-2,1,3,tmp->offset+s1,s2);
             delete(hp,k);
             if(size > s2)
                k = k-1;
             else
               k = k-4;
          }
      }
      else
      {
         if(size> s*2/3 )
           f = 0;
         else
         {
             hp->no_splits++;
             s1 = s*2/3;
             s2 = s-s1;
             tmp = hp->asl_head[k];
             insert(hp,k-1,tmp->kval+1,1,1,tmp->offset,tmp->sz);
             insert(hp,k-3,tmp->kval,1,2,tmp->offset+s1,s2);
             delete(hp,k);
             if(size > s2)
                k = k-1;
             else
               k = k-3;
         
         }
      }
      
   }
   tmp = hp->asl_head[k];
   tmp = insert_usl(&(hp->usl),tmp->kval,0,tmp->type,tmp->offset,tmp->sz);
   delete(hp,k);
   hp->req_size += size;
   hp->alloc_size += hp->sizes[k];
   hp->alloc_SP_heap += hp->sizes[k];
   return hp->base_ptr + tmp->offset; 

}



int sp_free(void* ptr,SP_heap* hp)
{
    int i,j,k,o = ptr-hp->base_ptr,bo,z;
    int bud_sz,b_sz;
    hp->no_dealloc++;
    block* b = hp->usl;    
    while( b!=NULL && b->offset != o)
       b= b->next;
   if(b == NULL)
     return 1;
   delete_usl(&( hp->usl), b);     
   int f = 1;
   do {
    k = b->kval;
    o = b->offset;
    hp->no_recombs++;
    
   if( b->type == 3 )
   {
      if(o % (1ll<<(k+2)) == 0)
      {      
            bo = o + 3*(1ll<<k);
            b_sz = 3*(1ll<<k);
            bud_sz = (1ll<<k);
      }
      else
       {    bo = o - 3*(1ll<<k);
            b_sz = (1ll<<k);
            bud_sz = 3*(1ll<<k);
       }
   }
   else if( b->type == 2 )
   {
      bo = o - (1ll<<(k+1));
      b_sz = (1ll<<k);
      bud_sz = (1ll<<(k+1));
   }
   else if( b->type == 1 )
   {
      bo = o + (1ll<<k);
      b_sz = (1ll<<k);
      bud_sz = (1ll<<(k-1));
   }
   else
   {
      bud_sz = 0;
   }
     block* tmp = NULL;
     if(f)
    {
      f = 0;
      hp->alloc_SP_heap -= b_sz;
    }



   if(b_sz && bud_sz){
    j  = get_index_b(hp->sizes, 2*hp->size,bud_sz);
    z  = get_index_b(hp->sizes, 2*hp->size,b_sz);
  
   if(bud_sz>1ll<<(hp->size))
     tmp = NULL;
   else
     tmp= hp->asl_head[j];
  
  while(tmp != NULL && tmp->offset != bo)
        tmp = tmp->next;
     
        
     
    }
   if(!tmp)
   {    
      insert( hp,b->type?z:(2*hp->size-1), b->kval, 1, b->type, b->offset,b->sz);
      free(b);
      hp->no_recombs--;
      return 0;
   }
  
   
    delete_asl(hp,tmp,j);
   
    int type,kval,sz,mn,mx,off;
    mn = min(b_sz,bud_sz);
    mx = max(b_sz,bud_sz);
    off = min(o,bo);
    if(mx == 3*mn)
     kval = k+2;
    else
     kval = min(b->kval,tmp->kval);
     
    if(off==o)
      sz = b->sz;
    else
      sz = tmp->sz;
      
      
    if(!power_2(b_sz+bud_sz)|| off%(1ll<<kval+1))
      type = 3; 
    else
    {
      if(sz == (b_sz+bud_sz)) 
          type = 2;
      else
          type = 1;
    }
  
    free(tmp);
    free(b);
    if(b_sz + bud_sz == (1ll<<hp->size))
      type = 0;
    b = create_block( kval,1, type,off,sz, NULL,NULL);
   }while(true);
}

