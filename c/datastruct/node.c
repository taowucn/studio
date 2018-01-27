#include <stdio.h>
#include <stdlib.h>

typedef struct _node{
  int data;
  struct _node *next;
} node;

node *create_node(void)
{
  node *head, *p, *s;
  int i;
  int cycle = 1;
  head = (node*) malloc(sizeof(node));
  head->next = NULL;
  p = head;
  while (cycle){
    printf("Input data:");
    scanf("%d", &i);
    if (i != 9){
      s = (node *) malloc (sizeof(node));
      s->data = i;
      s->next = NULL;
      p->next = s;
      p = s;
    }else {
      printf("Exit\n");
      cycle = 0;
      break;
    }
  }
  p = head;
  head = head->next;
  free(p);
  return head;
}

unsigned int length_node(node *head)
{
  int n = 0;
  node *p = head;
  while (p != NULL){
    p = p->next;
    n++;
  }
  return n;
}

void print_node(node *head)
{
  node *p = head;
  while (p != NULL) {
    printf("%d\n",p->data);
    p = p->next;
  }
}

void clean_node(node *head)
{
  node *p;
  while(head != NULL) {
    p = head;
    head = head->next;
    free(p);
  }
}

node * del_node(node *head, int num)
{
  node *p1 = head;
  node *p2;
  while ( (p1 != NULL) && (p1->data != num) ) {
    p2 = p1;
    p1 = p1->next;
  }
  if (p1 == NULL) {
   printf("Not found\n");
  }else {
   printf("Delete %d\n", p1->data);
   if (p1 == head) {
     printf("Delete head\n");
     head = head->next;
     free(p1);
   }else {
     p2->next = p1->next;
     free(p1);
   }
  }
  return head;
}

node * insert_node(node * head, int num)
{
  node *p = (node *)malloc(sizeof(node));
  printf("Insert %d\n", num);
  p->data = num;
  p->next = head;
  return p;
}

//bubble sort
node * sort_node(node *head)
{
   int len = 0;
   int i = 0;
   int j = 0;

   node *p = head;
   len = length_node(head);

   if ( (p == NULL) || (p->next == NULL) ) {
     return head;
   }

   for ( i = 1; i < len; i++) {
     p = head;
     for (j = 0 ; j < len-i; j++) {
      if (p->data > p->next->data) {
       int tmp = p->data;
       p->data = p->next->data;
       p->next->data = tmp;
      }
      p = p->next;
     }
   }
   return head;
}

node *reverse_node(node *head)
{
  node *p1;
  node *p2;
  node *p3;

  if ( (head == NULL) || (head->next == NULL) ) {
    return head;
  }

  p1 = head;
  p2 = p1->next;

  while (p2){
    p3 = p2->next;
    p2->next = p1;
    p1 = p2;
    p2 = p3;
  }
  head->next = NULL;
  head = p1;
  return head;
}

int main (int argc, char ** argv)
{
  int len = 0;
  int d = 1;
  node *p = create_node();
  printf("Input delete node:\n");
  scanf("%d", &d);
  p = del_node(p, d);
  printf("Input insert node:\n");
  scanf("%d", &d);
  p = insert_node(p, d);
  printf("Sort node.\n");
  p = sort_node(p);
  printf("Reverse node.\n");
  p = reverse_node(p);
  len = length_node(p);
  printf("Total node = %d\n", len);
  printf("Print node.\n");
  print_node(p);
  printf("Clean node.\n");
  clean_node(p);
  return 0;
}
