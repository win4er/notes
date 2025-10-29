#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP
#include <cstdio>
#include <string>
#include <regex>
#include <string.h>
#include <cstdlib>
#include <filesystem>

struct CarData {
    std::string license;
    std::string brand;
    std::string model;
};

struct LicenseData {
    std::string license;
    std::string surname;
    std::string address;
    std::string release_year;
};

int validate_car_data(CarData info);
int validate_license_data(LicenseData info);

void make_file_car_data();
void make_file_license_data();

void program_info();
void UI();
#endif
