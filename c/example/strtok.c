#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
  int ret = -1;
  int fd = -1;
  char buf[64];
  char *c = NULL;
  int m = 0;
  int n = 0;

  system("echo 12,98 > /tmp/hi");

  fd = open("/tmp/hi", O_RDONLY);

  if (fd < 0) {
    perror("open");
    return ret;
  }
  read(fd, buf, sizeof(buf));
  printf("buf[%d]=%s\n", strlen(buf), buf);
  close(fd);
  fd = -1;

  c = strtok(buf, ",");
  m = atoi(c);
  printf("buf=%s, c=%s, m=%d\n", buf, c, m);
  c = strtok(NULL, ",");
  n = atoi(c);
  printf("buf=%s, c=%s, n=%d\n", buf, c, n);


  return ret;
}
