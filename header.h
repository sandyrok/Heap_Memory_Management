#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#define min(a,b) (a<b)?a:b;
#define max(a,b) (a>b)?a:b;

typedef struct block
{
  int kval,tag,type,offset;
  long sz;
  struct block* next, *prev;
}block;


typedef struct SP_heap
{
   block** asl_head, **asl_tail;
   block* usl;
   int size;
   void* base_ptr;
   int* sizes;
   long no_alloc,no_dealloc;
   long no_splits,no_recombs;
   double req_size,alloc_size;
   double alloc_SP_heap;
}SP_heap;

int sp_free(void* ptr,SP_heap* hp);
void* sp_alloc(int size,SP_heap* hp);
void init_SP_heap(int size,SP_heap* hp);
void free_SP_heap(SP_heap* hp);

typedef struct one_bin
{
   block* usl;
   long size,cap;
   void* base_ptr;
   int* sizes;
   int no_alloc,no_dealloc;
   int no_splits,no_recombs;
   int req_size,alloc_size;
   int alloc_one_bin;
}one_bin;

int one_bin_free(void* ptr,one_bin* hp);
void* one_bin_alloc(one_bin* hp);
void init_one_bin(long size,one_bin* hp,long cap);
void free_one_bin(one_bin* hp);




