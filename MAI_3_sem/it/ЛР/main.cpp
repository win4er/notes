#include "file_handler.hpp"
#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #else
        setlocale(LC_ALL, "ru_RU.UTF-8");
    #endif
    program_info();
    UI();
    return 0;
}
