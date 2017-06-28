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
  int log=0;
  struct spisok *ancillary_memory;
  if(memory==NULL){
      memory=sbrk(0);
      sbrk(sizeof(struct spisok));
      memory->address=sbrk(0);
      memory->size=size;
      memory->state=1;
      memory->previous=NULL;
      memory->next=NULL;
      sbrk(size);
      return memory->address;
  }
  else{
    ancillary_memory=memory;
    while((memory->previous!=NULL)&&(log!=1)){
        memory=memory->previous;
        if((memory->state==0)&&(memory->size>=size))
            log=1;
    }
    if(log==1){
        memory->state=1;
        return memory->address;
    }
    else{
        memory=ancillary_memory;
        memory->next=sbrk(0);
        sbrk(sizeof(struct spisok));
        memory->next->previous=memory;
        memory=memory->next;
        memory->address=sbrk(0);
        memory->size=size;
        memory->state=1;
        memory->next=NULL;
        sbrk(size);
        return memory->address;
    }
  }
}

void new_free(void*name, const void*b)
{
    struct spisok *ancillary_memory;
    ancillary_memory=memory;
    while(memory->address!=name){
        memory=memory->previous;
    }
    if(memory->next==NULL){
        memory->previous->next=NULL;
        sbrk(-(memory->size));
        sbrk(-(sizeof(struct spisok)));
        memory=memory->previous; 
    }
    else{
        memory->state=0;
        memory=ancillary_memory;
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
    int *a, *b;
    char*c, *e;
    a=malloc(sizeof(int)*5);
    *a=10; a[4]=10;
    b=malloc(sizeof(int)*2);
    b[0]=1;
    c=malloc(sizeof(char));
    *c='c';
    printf("%d %d %c\n", a[4], b[0], *c);
    printf("%p %p %p\n", a, b, c);
    free(c);
    e=malloc(sizeof(char));
    *e='e';
    printf("%d %d %c\n", a[4], b[0], *e);
    printf("%p %p  %p\n",a, b, e);

}
