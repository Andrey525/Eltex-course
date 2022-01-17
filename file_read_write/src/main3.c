#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd;
  ssize_t ret;
  long position_last;
  char ch;
  int i;
  char *buf = malloc(sizeof(char) * 256);
  fd = open("file.txt", O_RDONLY);
  if (fd == -1) {
    perror("File opening error");
    return -1;
  }
  // переместились в конец, узнали позицию последнего символа
  position_last = lseek(fd, 0, SEEK_END);
  if (position_last == -1) {
    perror("File lseek error");
    return -3;
  }
  // вернулись в начало файла
  if (lseek(fd, 0, SEEK_SET) == -1) {
    perror("File lseek error");
    return -3;
  }
  i = position_last - 1;
  while ((ret = read(fd, &ch, 1)) > 0) {
    buf[i] = ch;
    i--;
  }
  if (ret == -1) {
    perror("File read error");
    return -2;
  }
  buf[position_last] = '\0';
  printf("%s\n", buf);
  free(buf);
  close(fd);
  return 0;
}