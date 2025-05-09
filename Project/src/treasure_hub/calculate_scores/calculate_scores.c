#include "calculate_scores.h"

// receives the path of the directory where to compute the scores
int main(int argc, char **argv) {

  if (argc != 2) {
    dprintf(STDOUT_FILENO, "Need treasure hunt directory path as an argument!");
    return 1;
  }

  dprintf(STDOUT_FILENO, "DIRECTORY SEARCHED: %s\n", argv[1]);

  fflush(stdout);

  user_score_t us[MAX_USERS_PER_HUNT];

  return 0;
}
