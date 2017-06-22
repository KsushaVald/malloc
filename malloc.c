#include <stdio.h>
#include <malloc.h>

struct spisok{
  void* adress;
  int  size;
  int  state;
  struct spisok* next;
  struct spisok* previous;
};


void* new_malloc(size_t size, const void*a)
{
  void *p;
  p=sbrk(0);
  sbrk(size);
  return p;
}

void new_free(void*name, const void*b)
{
    brk(name);
}

void init(void)
{
    __malloc_hook=new_malloc;
    __free_hook=new_free;
}
void (* __malloc_initialize_hook)(void)=init;

int main()
{
    int *a, *b, *c;
    a=malloc(sizeof(int));
    a=10;
    b=malloc(sizeof(int));
    b=11;
    c=malloc(sizeof(int));
    c=12;
    printf("%d %d %d\n", a, b, c);
    printf("%d %d %d\n", &a, &b, &c);
    free(b);
    b=15;
    printf("%d %d %d\n", a, b, c);
    printf("%d %d  %d\n", &a,&b, &c);

}
