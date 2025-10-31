#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <cstdio>
#include <string>
#include <regex>
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

bool validate_filename(std::string filename);
bool validate_license(std::string license);
bool validate_brand(std::string brand);
bool validate_model(std::string model);
bool validate_surname(std::string surname);
bool validate_address(std::string address);
bool validate_release_year(std::string release_year);

std::pair<std::string, std::string> file_info(std::string default_filename);
void write_car_data();
void write_license_data();

#endif