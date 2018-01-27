#include <stdlib.h>
#include <stdio.h>

typedef struct student_s
{
  int data;
  struct student_s *next;
}node;

typedef struct queue_s
{
  node *first;
  node *rear;
}queue;

queue *insert (queue *HQ, int x)
{
  node *s = NULL;
  s = (node *)malloc(sizeof(node));
  s->data = x;
  s->next = NULL;
  if (HQ->rear == NULL) {
    HQ->first = s;
    HQ->rear = s;
  }else {
    HQ->rear->next = s;
    HQ->rear = s;
  }
  return HQ;
}

queue *del (queue *HQ)
{
  node *p = NULL;
  p = HQ->first;
  if (p == NULL) {
    printf("ERROR:queue is null.\n");
  }else if(HQ->first == HQ->rear) {
    HQ->first = NULL;
    HQ->rear = NULL;
  }else {
    HQ->first = p->next;
    free(p);
  }
  return HQ;
}

void print_queue(queue *HQ)
{
  node *p = NULL;
  p = HQ->first;
  if (p == NULL) {
    printf("ERROR: queue is null.\n");
  }else {
    while (p != NULL){
      printf("Node:%d\n", p->data);
      p = p->next;
    }
  }
}

int main (int argc, char ** argv)
{
  queue *hq;
  hq = (queue *)malloc(sizeof(queue));
  hq->first = NULL;
  hq->rear = NULL;
  hq = insert(hq, 1);
  hq = insert(hq, 2);
  hq = insert(hq, 3);
  printf("Insert done, show node.\n");
  print_queue(hq);
  hq = del(hq);
  printf("Delete done, show node.\n");
  print_queue(hq);
  return 0;
}
