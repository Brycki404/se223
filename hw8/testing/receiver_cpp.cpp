#include <iostream>
#include <string>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << "C++ received: " << line << std::endl;
    }
    return 0;
}