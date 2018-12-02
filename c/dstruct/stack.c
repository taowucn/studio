#include <stdlib.h>
#include <stdio.h>

typedef struct student_s
{
  int data;
  struct student_s *next;
}node;

typedef struct stack_s
{
  node *bottom;
  node *top;
}stack;

stack *push(stack *SK, int x)
{
  node *s = NULL;
  s = (node *)malloc(sizeof(node));
  s->data = x;
  s->next = NULL;

  if (SK->top == NULL) {
    SK->top = s;
    SK->bottom = s;
  }else {
    SK->top->next = s;
    SK->top = s;
  }

  return SK;
}

stack *pop(stack *SK)
{
  node *p = NULL;
  p = SK->bottom;

  if (p == NULL) {
    printf("ERROR: stack is null.\n");
  }else if( p == SK->top) {
    SK->top = NULL;
    SK->bottom = NULL;
  }else {
    while (p->next != SK->top) {
      p = p->next;
    }
    SK->top = p;
    SK->top->next = NULL;
  }
  return SK;
}

void print_stack(stack *SK)
{
  node *p;
  p = SK->bottom;
  if(p == NULL) {
    printf("ERROR:stack is null.\n");
  }else {
    while(p != NULL){
      printf("Node:%d\n", p->data);
      p = p->next;
    }
  }
}

int main(int argc, char **argv)
{
  stack *SK = NULL;
  SK = (stack *)malloc(sizeof(stack));
  SK->bottom = NULL;
  SK->top = NULL;
  SK = push(SK, 1);
  SK = push(SK, 2);
  SK = push(SK, 3);
  printf("Push done.\n");
  print_stack(SK);
  SK = pop(SK);
  printf("Pop done.\n");
  print_stack(SK);
  return 0;
}
