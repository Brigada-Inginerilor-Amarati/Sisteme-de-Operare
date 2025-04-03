#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd;
  fd = open("file.txt", O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  printf("FILE : %d\n", fd);

  char buffer[1024];
  ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
  if (bytes_read == -1) {
    perror("read");
    close(fd);
    return 1;
  }

  // lseek(fd, 0, SEEK_SET);
  // it is used to move the file pointer to a specific position in the file.

  write(STDOUT_FILENO, buffer, bytes_read);

  close(fd);
  return 0;
}
