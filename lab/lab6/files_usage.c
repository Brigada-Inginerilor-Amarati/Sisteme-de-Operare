#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  int fd;
  fd = open("file.txt", O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  // create a directory
  mkdir("new_directory", 0755);
  struct stat st;
  stat("new_directory", &st);

  printf("STAT DIRECTORY : %x\n", st.st_mode);

  lstat("new_directory", &st);
  printf("LSTAT DIRECTORY : %x\n", st.st_mode);
  fstat(fd, &st);

  printf("FSTAT FILE : %x\n", st.st_mode);

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
