#include <stdio.h>
#include <stdlib.h>

void bubble_sort(int *list)
{
 int i = 0;
 int j = 0;
 int t = 0;

 for (i = 0; i < 9; i++) {
   for (j = 0; j < 9 - i; j++) {
     if (list[j] > list[j+1]){
       t = list[j+1];
       list[j+1] = list[j];
       list[j] = t;
     }
   }
 }
}

void print_list(int *list)
{
  int i = 0;
  for (i = 0; i < 10; i++) {
    printf("%d:%d\n", i, list[i]);
  }
}

int main (int argc, char **argv)
{
  int i = 0;
  int j = 0;
  int list[10];

  printf("Input 10 numbers.\n");
  for (i = 0; i < 10; i ++) {
    printf("%d:", i);
    scanf("%d\n", &list[i]);
  }
  printf("Input END.\n");

  bubble_sort(list);
  print_list(list);

  return 0;
}
