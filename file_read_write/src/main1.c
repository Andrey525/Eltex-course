#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int fd;
  char *buf = malloc(sizeof(char) * 256);
  strcpy(buf, "Some string");
  fd = open("file.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    fd = open("file.txt", O_WRONLY | O_TRUNC, S_IRUSR);
    if (fd == -1) {
      perror("File opening error");
      return -1;
    }
  }
  if (write(fd, buf, strlen(buf)) == -1) {
    perror("File write error");
    return -2;
  }
  free(buf);
  close(fd);
  return 0;
}