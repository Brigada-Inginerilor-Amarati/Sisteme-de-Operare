#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <unistd.h>

#define PERMISSIONS (S_IRWXU | S_IRWXG | S_IRWXO)

int main() {
    // Create a file with specific permissions
    const char *file_path = "example.txt";
    const char *symlink_path = "symlink_example.txt";

    // Create the file (this would usually be in a directory with appropriate permissions)
    int fd = open(file_path, O_CREAT | O_WRONLY, 0777);

    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    close(fd);

    // Now create a symlink to that file
    if (symlink(file_path, symlink_path) == -1) {
        perror("Error creating symlink");
        return 1;
    }

    return 0;
}
