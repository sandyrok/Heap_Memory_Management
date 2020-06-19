#include "header.h"
#include<time.h>


typedef struct node
{
  void *ptr;
  double sz;
  struct node* next;
}node;

node *create_node(void* ptr, node *next,double sz) {
  node *tmp = (node *)malloc(sizeof(node));
  tmp->next = next;
  tmp->ptr = ptr;
  tmp->sz = sz;
  return tmp;
}

node* insert_simu(node** head,void* ptr,double size)
{
   if(*head == NULL)
   { (*head) = create_node( ptr,NULL,size);
    return *head;
  }
  node* tmp  = create_node(ptr,*head,size);
  (*head) = tmp;
  return *head;    
}


int main()
{
int tme = 1,m,i,j,c,s;
 SP_heap hp;
   init_SP_heap(28,&hp);
   m = 20000000;
   node** del;
   del = (node**)malloc((m+1)*sizeof(node*));
   if(del == NULL)
   {
     printf("Error");
     return 0;
   }
   for(i=1;i<=m;i++)
     del[i] = NULL;
  void* ptr;
  double sz = 0;
 srand(time(0));
while(tme<=m)
{
  if(tme+100<=m){
   i = rand()%100 + 1;
   s = 100+ rand()%1900 + 1;
   ptr = sp_alloc(s, &hp);
   if(ptr)
    {insert_simu(&del[tme+i],ptr,s); sz+=s;}
   else
   {
     node* tmp,*t;
     while(tme<=m)
     {
        tmp = del[tme];
        while(tmp != NULL){
        sp_free(tmp-> ptr, &hp);
        t = tmp ->next;
        free(tmp);
        tmp = t;    
        }
        tme++;
     }
   }
   }
   node* tmp = del[tme],*t;
if(tme<=2000)
  while(tmp != NULL){
    sz -= tmp->sz;	
    sp_free(tmp-> ptr, &hp);
    t = tmp;
    tmp = tmp->next;
    free(t);
   }
   tme = tme +1;
}
  
  free_SP_heap(&hp);
  printf("%ld %ld %f %f %f\n",hp.no_splits,hp.no_recombs,hp.req_size,hp.alloc_size, - (hp.req_size-hp.alloc_size)/hp.req_size*100 );
}

