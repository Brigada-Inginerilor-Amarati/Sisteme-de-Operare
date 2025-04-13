#include "treasure.h"
#include "../utils/utils.h"

int is_valid_latitude(double latitude) {
  return latitude >= -90.0 && latitude <= 90.0;
}

int is_valid_longitude(double longitude) {
  return longitude >= -180.0 && longitude <= 180.0;
}

treasure create_treasure(int fd) {
  treasure void_t = {0, "\0", 0, 0.0, 0.0, "\0"};
  treasure t = void_t;

  char buffer[BUFSIZ];

  // id
  write(STDOUT_FILENO, "Enter id: ", 10);
  if (!read_line(fd, buffer, sizeof(buffer))) {
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
  if (!read_line(fd, buffer, sizeof(buffer))) {
    perror("INVALID USER NAME FORMAT");
    return t;
  }

  // remove newline if present
  char *newline = strchr(buffer, '\n');
  if (newline)
    *newline = '\0';

  strncpy(t.user_name, buffer, sizeof(t.user_name) - 1);

  char *endptr;

  // value
  write(STDOUT_FILENO, "Enter value: ", 13);
  if (!read_line(fd, buffer, sizeof(buffer))) {
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
  if (!read_line(fd, buffer, sizeof(buffer))) {
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
  if (!read_line(fd, buffer, sizeof(buffer))) {
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
  if (!read_line(fd, buffer, sizeof(buffer))) {
    perror("INVALID CLUE TEXT FORMAT");
    return t;
  }

  // remove newline if present
  newline = strchr(buffer, '\n');
  if (newline)
    *newline = '\0';

  strncpy(t.clue_text, buffer, sizeof(t.clue_text) - 1);

  return t;
}

int is_void_treasure(const treasure *t) {
  return t->id == 0 || t->user_name[0] == '\0';
}

void print_treasure(const treasure *t) {
#define DOUBLE_NUM_BUFFER_SIZE 32

  char double_nr_buffer[DOUBLE_NUM_BUFFER_SIZE];
  char buffer[BUFSIZ] = "";

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
  snprintf(buffer, BUFSIZ, "%d,%s,%d,%lf,%lf,%s\n", t->id, t->user_name,
           t->value, t->latitude, t->longitude, t->clue_text);
}

operation_error write_treasure_to_file(const treasure *t, const char *path) {

  int fd = open(path, O_WRONLY | O_APPEND);
  if (fd == -1) {
    perror("WRITE TREASURE TO FILE ERROR");
    return FILE_ERROR;
  }

  char buffer[BUFSIZ];
  get_treasure_string(buffer, t);
  write(fd, buffer, strlen(buffer));

  close(fd);
  return NO_ERROR;
}
