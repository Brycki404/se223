/*
    Authors: Dustin Knecht & [Partner Name]
    Favorite Location: Trine University Library (2nd Floor Quiet Area)
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = popen("./receiver_cpp", "w");
    if (!fp) {
        perror("popen failed");
        return 1;
    }

    fprintf(fp, "Hello from C to C++!\n");
    pclose(fp);

    return 0;
}