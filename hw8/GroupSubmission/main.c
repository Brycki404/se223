/*
    Authors: Dustin Knecht & River Bieberich
    Favorite Location: Trine University Library "The LINK" (1st Floor, Corner by the Stairs)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

void timestamp(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%H:%M:%S", t);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf(RED "Usage: %s [on|off] [max_passes]\n" RESET, argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "off") == 0) {
        printf(YELLOW "C: Received 'off' — exiting.\n" RESET);
        return 0;
    }

    int max_passes = (argc >= 3) ? atoi(argv[2]) : 100;
    int passes = 0;
    char buffer[128];
    char ts[32];

    while (passes < max_passes) {

        timestamp(ts, sizeof(ts));
        printf(BLUE "[%s] C: Starting cycle with %d\n" RESET, ts, passes);

        // --- Send to C++ ---
        char cmd_cpp[128];
        #ifdef _WIN32
        snprintf(cmd_cpp, sizeof(cmd_cpp),
                "\"build\\process.exe\" %d", passes);
        #else
        snprintf(cmd_cpp, sizeof(cmd_cpp),
                "./build/process %d", passes);
        #endif
        
        FILE *fp_cpp = popen(cmd_cpp, "r");
        if (!fp_cpp) {
            perror("C: popen to C++ failed");
            return 1;
        }

        if (!fgets(buffer, sizeof(buffer), fp_cpp)) {
            printf(RED "C: Error reading from C++\n" RESET);
            pclose(fp_cpp);
            return 1;
        }
        pclose(fp_cpp);

        int from_cpp = atoi(buffer);
        timestamp(ts, sizeof(ts));
        printf(CYAN "[%s] C: Got %d from C++\n" RESET, ts, from_cpp);

        // --- Send to Python ---
        char cmd_py[128];
        #ifdef _WIN32
        snprintf(cmd_py, sizeof(cmd_py),
                "python \"process.py\" %d", from_cpp);
        #else
        snprintf(cmd_py, sizeof(cmd_py),
                "python3 process.py %d", from_cpp);
        #endif

        FILE *fp_py = popen(cmd_py, "r");
        if (!fp_py) {
            perror("C: popen to Python failed");
            return 1;
        }

        if (!fgets(buffer, sizeof(buffer), fp_py)) {
            printf(RED "C: Error reading from Python\n" RESET);
            pclose(fp_py);
            return 1;
        }
        pclose(fp_py);

        int from_py = atoi(buffer);
        timestamp(ts, sizeof(ts));
        printf(GREEN "[%s] C: Got %d from Python\n" RESET, ts, from_py);

        passes = from_py;
    }

    timestamp(ts, sizeof(ts));
    printf(YELLOW "[%s] C: Max passes (%d) reached. Ending.\n" RESET, ts, max_passes);

    return 0;
}