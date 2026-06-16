#include "editor.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    try {
        std::string filepath = (argc >= 2) ? argv[1] : "";
        minivim::Editor editor(filepath);
        editor.run();
    } catch (const std::exception& e) {
        std::cerr << "\r\n[Fatal Error] " << e.what() << "\r\n";
        return 1;
    }
    return 0;
}
