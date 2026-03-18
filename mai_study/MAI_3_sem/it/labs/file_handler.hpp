#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <regex>
#include <string>
#include <ctime>
#include <cctype>
#include <stdio_ext.h>

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

bool validate_filename(const std::string& filename);
bool validate_license(const std::string& license);
bool validate_brand(const std::string& brand);
bool validate_model(const std::string& model);
bool validate_surname(const std::string& surname);
bool validate_address(const std::string& address);
bool validate_release_year(const std::string& release_year);

std::pair<std::string, std::string> file_info(
    char* buffer,
    const size_t& buf_size,
    const std::string& default_filename
);
void write_data(char* buffer, const size_t& buf_size, const size_t& option);

#endif
