#include <stdio.h>
#include <string.h>

int main()
{
	int i, j, edi;
	char buf[50], ch;
	char out[50];
	unsigned char check;
	int ret = 0, val, len, rem;
	int magic;
	int k;
	unsigned char arr[] = {0x6a, 0xfb, 0x4c, 0x8d, 0x58, 0x0f, 0xd4, 0xe8,
		0x94, 0x98, 0xee, 0x6b, 0x18, 0x30, 0xe0, 0x55, 0xc5, 0x28,
		0x0e};
	
	for (i = 0; i < 19; i++)
		out[i] = 'x';
	out[i] = '\0';

	len = strlen(out);
	if (len == 19)
		ret = 1;

	for (i = 10; i > 0; i--) {
		int m2;

		val = random();
		rem = val%19;
		check = arr[rem] & 0xff;
		ch = buf[rem++];

		j = 0;
		magic = 0;
		printf("rem = %d\n", rem);
		while (j < rem) {
			magic *= 1828812941;
			magic += 12345;
			j++;
		}
		m2 = magic;

		magic ^= ch;
		if ((magic & 0xff) != (check & 0xff))
			ret = 0;
		out[rem - 1] = (m2 & 0xff) ^ (check & 0xff);
	}
	printf("Password: %s\n", out);
}
