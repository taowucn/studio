/* main.c */
#include <stdio.h>
#include "t.h"

int b=3;
int c;

int main(int argc, char ** argv)
{
    foo();
    printf("main:\t(&a)=0x%08x\n\t(&b)=0x%08x\n\t(&c)=0x%08x\n\tsize(b)=%d\n\tb=%d\n\tc=%d\n",
        &a, &b, &c, sizeof b, b, c);
    return 0;
}
