#include "calculate_scores.h"

// read data from file
int read_treasure_from_file(treasure *t, int fd) {

  ssize_t bytes_read = read(fd, t, sizeof(treasure));
  if (bytes_read == sizeof(treasure))
    return 1;

  if (bytes_read == 0)
    return 0;

  perror("PARTIAL READ ERROR");
  return 0;
}

// go through the treasures.dat file and put each treasure in an array
void compute_scores(user_score_t *us, int *size, int fd) {

  int index = 0;
  treasure t;

  while (read_treasure_from_file(&t, fd)) {
    // go through the array and add the user
    int i = 0;
    for (i = 0; i < index; i++) {
      if (strcmp(us[i].user, t.user_name) == 0) {
        // found the same user, increase score
        us[i].total_score += t.value;
        break;
      }
    }

    // did not find the user, add it to the list
    if (i == index) {
      strcpy(us[index].user, t.user_name);
      us[index].total_score = t.value;
      index++;
    }
  }

  *size = index;
}

// print scores
void print_scores(const user_score_t *us, int size) {
  for (int i = 0; i < size; i++)
    printf("User: %10s | Score: %d\n", us[i].user, us[i].total_score);
  printf("\n");
}

// print the target hunt
void get_target_hunt(char *target, char *path) {
  // path structure: treasure_hunts/hunt_name/treasures.bin

  char path_copy[PATH_MAX];
  strcpy(path_copy, path);

  char *sep = "/";
  char *token = strtok(path_copy, sep);
  token = strtok(NULL, sep);

  strcpy(target, token);
}

// receives the path of the directory where to compute the scores
int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Need treasure hunt directory path as an argument!");
    return 1;
  }

  char target_hunt[PATH_MAX];
  get_target_hunt(target_hunt, argv[1]);
  printf("HUNT SEARCHED: %s\n", target_hunt);

  // open the file and check if it's valid
  int fd = open(argv[1], O_RDONLY);

  if (fd == -1) {
    perror(argv[1]);
    return 1;
  }

  int size = 0;
  user_score_t us[MAX_USERS_PER_HUNT];
  memset(us, 0, MAX_USERS_PER_HUNT * sizeof(user_score_t));

  compute_scores(us, &size, fd);
  print_scores(us, size);

  close(fd);

  return 0;
}
