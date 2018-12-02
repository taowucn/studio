#include <stdlib.h>
#include <stdio.h>
#define ERROR 0

typedef struct lnode_s{
  int data;
  struct lnode_s * link;
}lnode;

int joshepus(int n, int k, int m)
{
  lnode *curr = NULL;
  lnode *p = NULL;
  lnode *r = NULL;
  int i = 0;
  int s = 0;
  p = (lnode *)malloc (sizeof(lnode));
  p->data = 0;
  p->link = p;
  curr = p;

  for (i = 1; i < n; i++ ) {
    lnode *tmp = (lnode *)malloc(sizeof(lnode));
    tmp->data = i;
    tmp->link = curr->link;
    curr->link = tmp;
    curr = tmp;
  }
  while (k--) {
    r = p;
    p = p->link;
  }

  while (n--){
    for (s = m-1; s--; r=p, p=p->link);
      printf("%d->", p->data);
      r->link = p->link;
      free(p);
      p = r->link;
  }
  printf("\n");
}

int main (int argc, char ** argv)
{
  joshepus(13, 4 , 1);
  return 0;
}
