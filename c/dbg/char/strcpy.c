#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *my_strcpy(char *dst, const char *src)
{
	char *dstt = dst;

	if ((src == NULL)||(dst == NULL)) {
		return NULL;
	}
	while ((*(dstt++) = *(src++)) != 0) {
		NULL;
	}

	return dst;
}

char *loop_str_move(char *str, unsigned int step)
{
	char tmp[64];
	unsigned int len = strlen(str);

	strncpy(tmp, str+(len-step), step);
	strncpy(str+step, str, len-step);
	strncpy(str, tmp, step);

	return str;
}

int check_string( const char *str)
{
	int ret = 0;

	switch (*str) {
	case 'a':
		printf("match abc\n");
		break;
	case '1':
		printf("match 123\n");
		break;
	default:
		printf("unknown\n");
		ret = -1;
		break;
	}

	return ret;
}

void get_string(char **p)
{
	char *str = "I'm in code area";
	*p = str;
}

int main (int argc, char **argv)
{
	const char str[10]="abcdefg";
	char *dst = NULL;

	char s[16]="123456789";
	char d[]="123";
	char c[2]={'1','2'};
	char *ptr = NULL;

	dst = (char *)malloc(10 * sizeof(char));
	if (dst == NULL) {
		printf("malloc err\n");
		return -1;
	}

	strcpy(d, s);
	printf("strcpy: s[%ld]=%s %ld, d[%ld]=%s %ld.\n", sizeof(s), s, strlen(s), sizeof(d), d, strlen(d));

	dst = my_strcpy(dst, str);
	printf("my_strcpy: dst=%s.\n", dst);

	dst = loop_str_move(dst, 2);
	printf("loop_str_move:loop move 2, dst=%s.\n", dst);
	free(dst);
	printf("strlen * = %ld\n", strlen("*"));
	check_string("123");

	printf("strtoul 123 = 0x%lx\n", strtoul("0x123", NULL, 0));
	printf("atoi 123 = %d\n", atoi("123"));

	get_string(&ptr);
	printf("get str= %s\n", ptr);

	return 0;
}
