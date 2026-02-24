#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <cstring> // библиотека для подключения memset
#include <cstdio> // библиотека стандартного С ввода/вывода 
#include <cstdlib> // стандартная библиотека C
#include <filesystem> // библиотека для работы с файловой системой (требует C++17)
#include <regex> // библиотека для работы с регулярными варажениями
#include <string> // библиотека для работы с "умными" строками
#include <stdio.h>
#include <stdio_ext.h>

// Структура для временного хранения автомобильных сведений
struct CarData {
    std::string license;
    std::string brand;
    std::string model;
};

// Структура для временного хранения регистрационных сведений
struct LicenseData {
    std::string license;
    std::string surname;
    std::string address;
    std::string release_year;
};

// Ниже указаны прототипы функций,
// подробнее о них в файле реализаций file_handler.cpp

// прототипы функций валидации
bool validate_filename(const std::string& filename);
bool validate_license(const std::string& license);
bool validate_brand(const std::string& brand);
bool validate_model(const std::string& model);
bool validate_surname(const std::string& surname);
bool validate_address(const std::string& address);
bool validate_release_year(const std::string& release_year);

// прототипы функций, связанных с работой с файлами
std::pair<std::string, std::string> file_info(
    char* buffer,
    const size_t& buf_size,
    const std::string& default_filename
);
void write_data(char* buffer, const size_t& buf_size, const size_t& option);

#endif
