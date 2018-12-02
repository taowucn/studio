#include <stdio.h>
#include <stdlib.h>

typedef struct dnode_s{
  int data;
  struct dnode_s *prev;
  struct dnode_s *next;
}dnode_t;

dnode_t *create_dnode (void)
{
  dnode_t *head;
  dnode_t *p;
  dnode_t *s;

  int d = 0;
  int cycle = 1;
  head = (dnode_t*) malloc (sizeof(dnode_t));
  head->next = NULL;
  head->prev = NULL;
  p = head;

  while (cycle){
   printf("Input dnode:");
   scanf("%d", &d);
   if (d != 9) {
     s = (dnode_t *) malloc (sizeof(dnode_t));
     s->data = d;
     s->next = NULL;
     s->prev = NULL;

     p->next = s;
     s->prev = p;
     p = s;
   }else {
    cycle = 0;
   }
  }
  p->next = NULL;
  head = head->next;
  head->prev = NULL;
  return head;
}

dnode_t *delete_dnode(dnode_t *head, int num)
{
  dnode_t *p = head;

  while( ( p != NULL ) && (num != p->data) && (p->next != NULL) ) {
   p = p->next;
  }
  if (num == p->data) {
   if (p == head) {
     head = head->next;
     head->prev = NULL;
     free(p);
   }else if(p->next == NULL) {
     p->prev->next = NULL;
     free(p);
   }else {
     p->next->prev = p->prev;
     p->prev->next = p->next;
     free(p);
   }
  }else {
   printf("Not Found.\n");
  }
  return head;
}

dnode_t *insert_dnode(dnode_t *head, int num)
{
  dnode_t *p = head;
  dnode_t *s = (dnode_t *) malloc (sizeof(dnode_t));
  s->data = num;
  s->prev = NULL;
  s->next = NULL;

  while ( (p != NULL) && (num > p->data) && (p->next != NULL)) {
   p = p->next;
  }

  if (num <= p->data) {
    //add at head
    s->next = p;
    p->prev = s;
    head = s;
  }else if (p->next == NULL) {
    //add at tail
    p->next = s;
    s->prev = p;
  }else {
/*
    s->prev = p;
    s->next = p->next;
    p->next->prev = s;
    p->next = s;
*/
    p->prev->next = s;
    s->next = p;
    s->prev = p->prev;
    p->prev = s;
  }
  return head;
}

void print_dnode(dnode_t *head)
{
  dnode_t *p = head;
  while (p != NULL) {
    printf("%d\n",p->data);
    p = p->next;
  }
}

int main (int argc, char ** argv)
{
  dnode_t *head;
  int d;
  head = create_dnode();

  printf("Input delete dnode:");
  scanf("%d", &d);
  head = delete_dnode(head, d);

  printf("Input insert dnode:");
  scanf("%d", &d);
  head = insert_dnode(head, d);

  printf("Print dnode.\n");
  print_dnode(head);
  return 0;
}
