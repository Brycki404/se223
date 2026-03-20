#include "../se233.h"

int main() {
    const char *filename = "bestNumber.txt";
    const char *msg = "The magic number is 22\n";

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    ssize_t written = write(fd, msg, 24);  // 24 bytes including newline
    if (written == -1) {
        perror("write");
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    if (chmod(filename, 0777) == -1) {
        perror("chmod");
        return 1;
    }

    return 0;
}