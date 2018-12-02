#include <stdlib.h>
#include <stdio.h>
#define MAX 10

int dichotomy(int *list, int key)
{
  int low = 0;
  int high = MAX - 1;
  int mid = (low + high)/2;
  printf("low:%d, mid:%d, high:%d\n", low, mid, high);
  while(high >= low) {
    if (key > list[mid]) {
      low = mid + 1;
      mid = (low + high)/2;
    }else if( key < list[mid]) {
      high = mid - 1;
      mid = (low + high)/2;
    }else if( key == list[mid]){
      return (mid + 1);
    }
//    printf(".");
  }

  return -1;
}

int main (int argc, char **argv)
{
  int list[MAX] = {0,1,2,3,4,5,6,7,8,9};
  int key = 0;
  int pos = 0;
  printf("Input finding number:");
  scanf("%d", &key);
  pos = dichotomy(list , key);
  if ( pos < 0 ) {
    printf("Can't find number.\n");
  }else {
    printf("Found key postion:%d\n", pos);
  }
  return 0;
}
