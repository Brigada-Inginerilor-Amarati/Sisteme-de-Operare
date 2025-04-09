#include "treasure.h"
#include "../log/log.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslimits.h>
#include <unistd.h>

// max length of clue + 1 for newline
#define BUFFER_SIZE 65

int is_valid_latitude(double latitude) {
  return latitude >= -90.0 && latitude <= 90.0;
}

int is_valid_longitude(double longitude) {
  return longitude >= -180.0 && longitude <= 180.0;
}

// Helper: read a line from stdin using read()
int read_line(char *buf, size_t size) {
  ssize_t bytes_read = read(STDIN_FILENO, buf, size - 1);
  if (bytes_read <= 0)
    return 0;

  buf[bytes_read] = '\0';

  // Remove newline if present
  char *newline = strchr(buf, '\n');
  if (newline)
    *newline = '\0';

  return 1;
}

treasure create_treasure() {
  treasure void_t = {0, "\0", 0, 0.0, 0.0, "\0"};
  treasure t = void_t;

  char buffer[BUFFER_SIZE];

  // id
  write(STDOUT_FILENO, "Enter id: ", 10);
  if (!read_line(buffer, sizeof(buffer))) {
    perror("INVALID ID FORMAT");
    return t;
  }
  t.id = atoi(buffer);

  // check atoi error
  if (t.id == 0 && errno == EINVAL) {
    perror("INVALID ID FORMAT");
    return t;
  }

  // user_name
  write(STDOUT_FILENO, "Enter user name: ", 17);
  if (!read_line(buffer, sizeof(buffer))) {
    perror("INVALID USER NAME FORMAT");
    return t;
  }

  strncpy(t.user_name, buffer, sizeof(t.user_name) - 1);

  char *endptr;

  // value
  write(STDOUT_FILENO, "Enter value: ", 13);
  if (!read_line(buffer, sizeof(buffer))) {
    perror("INVALID VALUE FORMAT");
    return t;
  }
  t.value = (int)strtol(buffer, &endptr, 10);
  if (endptr == buffer) {
    perror("INVALID ID FORMAT");
    return t;
  }

  // check value error
  if (t.value == 0 && errno == EINVAL) {
    perror("INVALID VALUE FORMAT");
    return t;
  }

  // latitude
  write(STDOUT_FILENO, "Enter latitude: ", 16);
  if (!read_line(buffer, sizeof(buffer))) {
    perror("INVALID LATITUDE FORMAT");
    return t;
  }

  t.latitude = strtod(buffer, &endptr);
  if (endptr == buffer || !is_valid_latitude(t.latitude)) {
    perror("INVALID LATITUDE FORMAT");
    return t;
  }

  // check latitude error
  if (t.latitude == 0 && errno == EINVAL) {
    perror("INVALID LATITUDE FORMAT");
    return t;
  }

  // longitude
  write(STDOUT_FILENO, "Enter longitude: ", 17);
  if (!read_line(buffer, sizeof(buffer))) {
    perror("INVALID LONGITUDE FORMAT");
    return t;
  }
  t.longitude = strtod(buffer, &endptr);
  if (endptr == buffer || !is_valid_longitude(t.longitude)) {
    perror("INVALID LONGITUDE FORMAT");
    return t;
  }

  // check longitude error
  if (t.longitude == 0 && errno == EINVAL) {
    perror("INVALID LONGITUDE FORMAT");
    return t;
  }

  // clue_text
  write(STDOUT_FILENO, "Enter clue text: ", 17);
  if (!read_line(buffer, sizeof(buffer))) {
    perror("INVALID CLUE TEXT FORMAT");
    return t;
  }
  strncpy(t.clue_text, buffer, sizeof(t.clue_text) - 1);

  return t;
}

int is_void_treasure(const treasure *t) {
  return t->id == 0 || t->user_name[0] == '\0' || t->latitude == 0.0 ||
         t->longitude == 0.0 || t->clue_text[0] == '\0' || t->value == 0;
}

void print_treasure(const treasure *t) {
#define DOUBLE_NUM_BUFFER_SIZE 32
#define MAX_BUFFER 1024

  char double_nr_buffer[DOUBLE_NUM_BUFFER_SIZE];
  char buffer[MAX_BUFFER] = "";

  sprintf(buffer, "Treasure Info:\n");

  strcat(buffer, "ID: ");
  sprintf(double_nr_buffer, "%d", t->id);
  strcat(buffer, double_nr_buffer);

  strcat(buffer, "User Name: ");
  strcat(buffer, t->user_name);

  strcat(buffer, "\nValue: ");
  sprintf(double_nr_buffer, "%d", t->value);
  strcat(buffer, double_nr_buffer);

  strcat(buffer, "\nLatitude: ");
  sprintf(double_nr_buffer, "%lf", t->latitude);
  strcat(buffer, double_nr_buffer);

  strcat(buffer, "\nLongitude: ");
  sprintf(double_nr_buffer, "%lf", t->longitude);
  strcat(buffer, double_nr_buffer);

  strcat(buffer, "\nClue Text: ");
  strcat(buffer, t->clue_text);
  write(STDOUT_FILENO, buffer, strlen(buffer));
}

void get_treasure_string(char *buffer, const treasure *t) {
  snprintf(buffer, MAX_TREASURE_STRING_LENGTH, "%d,%s,%d,%lf,%lf,%s\n", t->id,
           t->user_name, t->value, t->latitude, t->longitude, t->clue_text);
}

operation_error write_treasure_to_file(const treasure *t,
                                       const char *dir_path) {

  char full_path[PATH_MAX];
  snprintf(full_path, PATH_MAX, "%s/%s", dir_path, TREASURE_FILE_NAME);

  int fd = open(full_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (fd == -1) {
    perror("WRITE TREASURE TO FILE ERROR");
    return FILE_ERROR;
  }

  char buffer[MAX_TREASURE_STRING_LENGTH];
  get_treasure_string(buffer, t);
  write(fd, buffer, strlen(buffer));

  close(fd);
  return NO_ERROR;
}
