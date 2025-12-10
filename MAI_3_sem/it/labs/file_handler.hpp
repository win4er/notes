#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <cstdio> // библиотека стандартного С ввода/вывода 
#include <string> // библиотека для работы с "умными" строками
#include <regex> // библиотека для работы с регулярными варажениями
#include <cstdlib> // стандартная библиотека C
#include <filesystem> // библиотека для работы с файловой системой (требует C++17)

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

// прототип функции очистки ввода(костыль)
void clear_stdin();

// прототипы функций валидации
bool validate_filename(std::string filename);
bool validate_license(std::string license);
bool validate_brand(std::string brand);
bool validate_model(std::string model);
bool validate_surname(std::string surname);
bool validate_address(std::string address);
bool validate_release_year(std::string release_year);

// прототипы функций, связанных с работой с файлами
std::pair<std::string, std::string> file_info(std::string default_filename);
void write_car_data();
void write_license_data();

#endif
