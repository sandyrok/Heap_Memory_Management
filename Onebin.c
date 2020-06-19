#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
void *ptr;
FILE *in;
FILE *out ;
#define min(a,b) (a<b)?a:b;
#define max(a,b) (a>b)?a:b;
typedef struct block
{
  int offset;
  struct block* next;
}block;



block *create_block(  int offset, block *next) {
  block *tmp = (block *)malloc(sizeof(block));
  tmp->offset = offset;
  tmp->next = next;
  return tmp;
}

void init_one_bin(long size,one_bin* hp,long cap)
{
   hp->size = size;
   hp->cap = cap;
   hp->alloc_one_bin = hp->req_size = hp->alloc_size = hp->no_alloc = hp->no_dealloc = hp->no_splits= hp->no_recombs = 0;
   hp->usl = NULL;
   hp->base_ptr = malloc(1ll<<cap);
}

void * hp)
{
   if(hp!= NULL)
   free(hp.base->ptr);
}


block* insert_usl(block** head,int tag, int offset)
{
   if(*head == NULL)
   { (*head) = create_block( offset,NULL);
    return *head;
  }
  block* tmp  = create_block(offset,*head);
  (*head) = tmp;
  return *head;    
}

int delete_usl(block** head,int offset)
{
    if(head && *head)
    {
       block* tmp = *head,*tmp2 = NULL;
       while(tmp && tmp->offset!=offset)
        { tmp2 = tmp;  tmp = tmp->next;}
       if(!tmp)
         return 1;
       if(tmp == *head)
       {
           *head = (*head)->next;
           free(tmp);
           return 0;
       }
       tmp2->next = tmp->next;
       free(tmp);
       return 0;
    }
    return 1;
}




void* one_bin_alloc(one_bin* hp)
{
   hp->no_alloc++;
   long j = 0;
  
   if(hp->usl == NULL){
     hp->usl  = create_block( 0,NULL);
     return hp->base_ptr;
   }
    block *tmp = hp->usl,*tmp2;
  
   while(tmp!= NULL && tmp->offset == j)
   {
     tmp2 = tmp;
     tmp = tmp->next;
     j += hp->size;
   }
   if( j == (1ll<<(hp->cap)))
      {
         printf("Memory could not be allocated: ");
         return NULL;
      }
   tmp2->next = create_block( j,tmp);
   printf("%d--\n",j);
   return hp->base_ptr + j; 

}

int one_bin_free(void* ptr,one_bin* hp)
{
    if(hp)
     return delete_usl(&(hp->usl),ptr-hp->base_ptr);
    return 0;
}

