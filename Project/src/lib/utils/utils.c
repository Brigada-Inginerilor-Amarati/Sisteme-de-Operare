#include "utils.h"
#include <unistd.h>

ssize_t read_line(int fd, char *buf, size_t max_len) {
  size_t i = 0;
  char ch;
  ssize_t n;

  while (i < max_len - 1) {
    n = read(fd, &ch, 1);

    if (n == 0) {
      break; // EOF
    } else if (n < 0) {
      return -1; // read error
    }

    buf[i++] = ch;

    if (ch == '\n') {
      break;
    }
  }

  buf[i] = '\0';
  return i;
}
