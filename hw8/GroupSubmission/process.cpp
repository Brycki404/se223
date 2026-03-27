/*
    Authors: Dustin Knecht & River Bieberich
    Favorite Location: Trine University Library "The LINK" (1st Floor, Corner by the Stairs)
*/

#include <iostream>
#include <string>
#include <ctime>

#define MAGENTA "\033[35m"
#define RESET   "\033[0m"

std::string timestamp() {
    time_t now = time(NULL);
    char buf[32];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
    return buf;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << MAGENTA "[" << timestamp() << "] C++: Missing argument" << RESET << std::endl;
        return 1;
    }

    int value = std::atoi(argv[1]);
    value++;

    std::cout << value << std::endl;

    std::cerr << MAGENTA "[" << timestamp() << "] C++ processed: " << value << RESET << std::endl;

    return 0;
}