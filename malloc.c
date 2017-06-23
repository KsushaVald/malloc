#include <stdio.h>
#include <malloc.h>

struct spisok{
  void* address;
  size_t  size;
  int  state;
  struct spisok* next;
  struct spisok* previous;
};
struct spisok *memory=NULL;

void* new_malloc(size_t size, const void*a)
{
  if(memory==NULL){
    memory->address=sbrk(0);
    memory->size=size;
    memory->state=1;
    memory->previous=NULL;
    memory->next=NULL;
    sbrk(size);
    return memory->address;
  }
  else{
    while( (memory->next!=NULL) || ((memory->next->state!=0)&&(memory->next->size>=size)) ){
        memory=memory->next;
    }
    if(memory->next==NULL){
        memory->next->previous=memory;
        memory=memory->next;
        memory->address=sbrk(0);
        memory->size=size;
        memory->state=1;
        memory->next=NULL;
        sbrk(size);
        return memory->address;
    }
    else{
        memory=memory->next;
        memory->state=1;
        return memory->address;
    }
  }
}

void new_free(void*name, const void*b)
{
    while(memory->address!=name){
        memory=memory->next;
    }
    if(memory->next==NULL){
        memory->previous->next=NULL;
        brk(name);
    }
    else{
        memory->state=0;
    }
}

void init(void)
{
    __malloc_hook=new_malloc;
    __free_hook=new_free;
}
void (* __malloc_initialize_hook)(void)=init;

int main()
{
    int *a, *b, *c, *e;
    a=malloc(sizeof(int));
    a=10;
    b=malloc(sizeof(int));
    b=11;
    c=malloc(sizeof(int));
    c=12;
    printf("%d %d %d\n", *a, *b, *c);
    printf("%p %p %p\n", a, b, c);
    free(b);
    e=malloc(sizeof(int));
    printf("%d %d %d\n", *a, *c, *e);
    printf("%p %p  %p\n",a, c, e);

}
