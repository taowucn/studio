//#include<iostream.h>
#include<stdio.h>

typedef int (*FP_CALC)(int, int);
//עÒÕÀ²»Êº¯ÊÉÃ¶øÊ¶¨Ò£¬ËÊһ¸ö·£¬Ä¿ÉÔ±½Óäadd¿´¿´
int add(int a, int b)
{
     return a + b;
}
int sub(int a, int b)
{
     return a - b;
}
int mul(int a, int b)
{
     return a * b;
}
int div(int a, int b)
{
     return b? a/b : -1;
}
//¶¨Òһ¸öýý¬·µ»Ø»¸ö롣¸Ã¸ÕÀÐΪ ӵÓ}¸ö²Îý·µ»ØàΪint µĺ¯ÊָÕ¡£ËµÄ÷Ǹù×·ûÏӦº¯ÊµĵØ·
FP_CALC calc_func(char op)
{
     switch (op)
     {
     case '+': return add;//·µ»غ¯ÊµĵØ·
     case '-': return sub;
     case '*': return mul;
     case '/': return div;
     default:
         return NULL;
     }
     return NULL;
}
//s_calc_funcΪº¯Ê£¬ËµĲÎý£¬
//·µ»ØµΪһ¸öÐ}¸ö²Î������ØàΪint µĺ¯ÊָÕ
int (*s_calc_func(char op)) (int, int)
{
     return calc_func(op);
}
//×ÖÓ»§ֱ½ӵ÷ĺ¯Ê£¬¸ú¯Ê½ÓÕ½¸öÕÊ£¬ºÍ»¸öõã£¬·µ»Ø½ÊµÄËã¹�
int calc(int a, int b, char op)
{

     FP_CALC fp = calc_func(op); //¸ùË·û¸÷Ëãº¯ÊµĵØ·
         int (*s_fp)(int, int) = s_calc_func(op);//ÓÓ²â
         // ASSERT(fp == s_fp);   // ¿ÉԶÏÔâÊÏµȵÄ
         if (fp) return fp(a, b);//¸ùһ²½µõ½µĺ¯ÊµĵØ·µ÷àº¯Ê£¬²¢·µ»ؽá
     else return -1;
}

void main()
{
    int a = 100, b = 20;

     printf("calc(%d, %d, %c) = %d\n", a, b, '+', calc(a, b, '+'));
     printf("calc(%d, %d, %c) = %d\n", a, b, '-', calc(a, b, '-'));
     printf("calc(%d, %d, %c) = %d\n", a, b, '*', calc(a, b, '*'));
     printf("calc(%d, %d, %c) = %d\n", a, b, '/', calc(a, b, '/'));
}


