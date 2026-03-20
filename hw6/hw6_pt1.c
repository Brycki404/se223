#include "../se233.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    FILE *out = fopen("outfile.txt", "w");
    if (!out) {
        perror("Error opening outfile.txt");
        fclose(in);
        return 1;
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), in)) {
        int hasZ = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] == 'Z' || buffer[i] == 'z') {
                hasZ = 1;
                break;
            }
        }

        if (hasZ) {
            fputs(buffer, out);
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}