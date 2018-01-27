/* foo.c */
#include <stdio.h>
#include "t.h"

struct {
   char a;
   int b;
} bb = { 2, 4 };

int main();

void foo()
{
    printf("foo:\t(&a)=0x%08x\n\t(&bb)=0x%08x\n\tsizeof(bb)=%d\n\tbb.a=%d\n\tbb.b=%d\n\tmain:0x%08x\n",
        &a, &bb, sizeof bb, bb.a, bb.b, main);
}
