#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = popen("python3 receiver_py.py", "w");
    if (!fp) {
        perror("popen failed");
        return 1;
    }

    fprintf(fp, "42\n");
    fprintf(fp, "100\n");
    pclose(fp);

    return 0;
}