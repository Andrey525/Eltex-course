#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int fd;
  ssize_t ret;
  char ch;
  int i;
  char *buf = malloc(sizeof(char) * 256);
  fd = open("file.txt", O_RDONLY);
  if (fd == -1) {
    perror("File opening error");
    return -1;
  }
  i = 0;
  while ((ret = read(fd, &ch, 1)) > 0) {
    buf[i] = ch;
    i++;
  }
  if (ret == -1) {
    perror("File read error");
    return -2;
  }
  buf[i] = '\0';
  printf("%s\n", buf);
  free(buf);
  close(fd);
  return 0;
}