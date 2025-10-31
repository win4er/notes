#include "file_handler.hpp"

std::string DEFAULT_FILENAME_1 = "car_data.txt";
std::string DEFAULT_FILENAME_2 = "license_data.txt";

char BUFFER[256];

bool validate_filename(std::string filename) {
    std::regex regex_filename{"[a-zA-Z0-9_-]+\\.txt"};
    return std::regex_match(filename, regex_filename);
}

bool validate_license(std::string license) {
    // M976MM777RUS
    std::regex pattern{"[ABEKMHOPCTYX][0-9]{3,3}[ABEKMHOPCTYX]{2,2}[0-9]{2,3}RUS"};
    return regex_match(license, pattern);
}

bool validate_brand(std::string brand) {
    std::regex pattern{"[А-ЯЁа-яёA-Za-z0-9-_.! ]+"};
    return regex_match(brand, pattern);
}

bool validate_model(std::string model) {
    std::regex pattern{"[А-ЯЁа-яёA-Za-z0-9-_.!() ]+"};
    return regex_match(model, pattern);
}

bool validate_surname(std::string surname) {
    std::regex pattern{"[А-ЯЁа-яёA-Za-z]+"};
    return regex_match(surname, pattern);
}

bool validate_address(std::string address) {
    std::regex address_pattern{"[A-Za-zА-ЯЁа-яё0-9!_-. ]+"};
    return regex_match(address, address_pattern);
}

bool validate_release_year(std::string release_year) {
    bool result=true;
    int year=0;
    for (int i=0; i<release_year.size(); ++i) {
        if (!isdigit(release_year[i])) {
            printf("Год должен содержать только цифры\n");
            result = false;
            break;
        } else {
            year *= 10;
            year += (release_year[i]-'0');
        }
    }
    std::time_t t = std::time(nullptr);
    std::tm *const pTInfo = std::localtime(&t);
    int current_year = 1900 + pTInfo->tm_year;
    if (!(year >= 1886 && year <= current_year)) {
        printf("Год вне диапазона\n");
        result = false;
    }
    return result;
}

std::pair<std::string, std::string> file_info(std::string default_file_name) {
    std::pair<std::string, std::string> file_info;
    std::string file_name;
    printf("Введите название файла(enter=%s): ", default_file_name.c_str());
    while (scanf("%255[^\n]s", BUFFER)) {
        fseek(stdin, 0, SEEK_END);
        file_name = BUFFER;
        if (validate_filename(file_name)) {
            break;
        }
        else {
            printf("Введенное имя файла некорректно. Файл должен иметь вид file.txt, \n");
            printf("где file состоит из латинских символов, дефиса(-) или нижнего подчеркивания(_),\n");
            printf("а, после следует его расширение .txt");
            printf("Введите название файла(enter=%s): ", default_file_name.c_str());
        }
    }
    fseek(stdin, 0, SEEK_END);
    if (file_name.size()==0) {
        file_name = default_file_name;
    }
    file_info.first = file_name;
    if (std::filesystem::exists(file_name)) {
        printf("Файл %s уже существует. Желаете продолжить запись вместо заполнения с начала(enter=Да/N=Нет)? ", file_name.c_str());
        char option;
        while (option = getchar()) {
            fseek(stdin, 0, SEEK_END);
            if (option == '\n') {
                file_info.second = "a";
                break;
            } else
            if (option == 'N' || option == 'n') {
                file_info.second = "w";
                break;
            } else printf("Введено %s можно ввести только enter или N\n");
        }
    } else file_info.second = "w";
    return file_info;
}

void write_car_data() {
    std::pair<std::string, std::string> file_information = file_info(DEFAULT_FILENAME_1);
    std::string file_name = file_information.first;
    FILE* file = std::fopen(file_name.c_str(), file_information.second.c_str());
     
    if (!file) {
        printf("Не удалось открыть файл %s\n", file_name.c_str());
        return;
    }
    printf("Файл %s успешно открыт для записи\n", file_name.c_str());

    int field_index=0;
    CarData temp = {};
    printf("Начинаем заполнять автомобильные сведения.\nДля завершения ввода нажмите 'Q' либо 'q'.\n");
    std::string field_names[3] = {"бренд", "модель", "номерной знак"};
    printf("Введите поле %s:\n", field_names[field_index].c_str());
    bool filled = false;
    int note_number = 0;
    while (scanf("%32[^\n]s", BUFFER) != EOF) {
        if ((BUFFER[0]=='Q' || BUFFER[0] == 'q') && BUFFER[1] == '\0') {
            printf("Введен %s. Ввод сведений закончен\n", BUFFER);
            fseek(stdin, 0, SEEK_END);
            break;
        }
        // to skip the rest of the input
        fseek(stdin, 0, SEEK_END);
        switch (field_index) {
            case 0: {
                temp.brand.assign(BUFFER);
                if (validate_brand(temp.brand))
                    field_index=(field_index+1)%3;
                break;
            }
            case 1: {
                temp.model.assign(BUFFER);
                if (validate_model(temp.model))
                    field_index=(field_index+1)%3;
                break;
            }
            case 2: {
                temp.license.assign(BUFFER);
                if (validate_license(temp.license)) {
                    field_index = (field_index+1)%3;
                    filled = true;
                }
                break;
            }
        }
        if (filled) {
            fwrite(temp.brand.c_str(), 1, temp.brand.size(), file);
            fputc(' ', file);
            fwrite(temp.model.c_str(), 1, temp.model.size(), file);
            fputc(' ', file);
            fwrite(temp.license.c_str(), 1, temp.license.size(), file);
            fputc('\n', file);
            printf(
                "Запись #%d завершена:\nбренд: %s\nмодель: %s\nномерной знак: %s\n",
                ++note_number,
                temp.brand.c_str(),
                temp.model.c_str(),
                temp.license.c_str()
            );
            printf("Продолжаем запись сведений\n");
            temp = {};
            filled = false;
        }
        printf("Введите поле %s:\n", field_names[field_index].c_str());
    }
    fclose(file);
    if (file) printf("Не удалось закрыть файл: %s\n", file_name.c_str());
    else printf("Файл %s успешно закрыт\n", file_name.c_str());
    // clear stdin to ignore EOF
    clearerr(stdin);
    return;
}

void write_license_data() {
    std::pair<std::string, std::string> file_information = file_info(DEFAULT_FILENAME_2);
    std::string file_name = file_information.first;
    FILE* file = std::fopen(file_name.c_str(), file_information.second.c_str());
    
    if (!file) {
        printf("Не удалось открыть файл %s\n", file_name.c_str());
        return;
    }
    printf("Файл %s успешно открыт для записи\n", file_name.c_str());

    int field_index=0;
    LicenseData temp = {};

    printf("Начинаем заполнять регистрационные сведения.\nДля завершения ввода нажмите 'Q' либо 'q'.\n");
    std::string field_names[4] = {"номерной знак", "фамилия владельца", "адрес владельца", "год выпуска"};
    printf("Введите поле '%s':\n", field_names[field_index].c_str());
    bool filled = false;
    int note_number = 0;
    while (scanf("%32[^\n]s", BUFFER) != EOF) {
        if ((BUFFER[0]=='Q' || BUFFER[0] == 'q') && BUFFER[1] == '\0') {
            printf("Введен %s. Ввод сведений закончен\n", BUFFER);
            fseek(stdin, 0, SEEK_END);
            break;
        }
        // to skip the rest of the input
        fseek(stdin, 0, SEEK_END);
        switch (field_index) {
            case 0: {
                temp.license.assign(BUFFER);
                if (validate_license(temp.license)) {
                    field_index = (field_index+1)%4;
                } else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }
            case 1: {
                temp.surname.assign(BUFFER);
                if (validate_surname(temp.surname)) {
                    field_index = (field_index+1)%4;
                } else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }
            case 2: {
                temp.address.assign(BUFFER);
                if (validate_address(temp.address)) {
                    field_index = (field_index+1)%4;
                } else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }
            case 3: {
                temp.release_year.assign(BUFFER);
                if (validate_address(temp.release_year)) {
                    field_index = (field_index+1)%4;
                    filled = true;
                } else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }

        }
        if (filled) {
            fwrite(temp.surname.c_str(), 1, temp.surname.size(), file);
            fputc(' ', file);
            fwrite(temp.address.c_str(), 1, temp.address.size(), file);
            fputc(' ', file);
            fwrite(temp.release_year.c_str(), 1, temp.release_year.size(), file);
            fputc('\n', file);
            printf(
                "Запись #%d завершена:\nномерной знак: %s\nфамилия владельца: %s\nадрес: %s\nгод выпуска: %s\n",
                ++note_number,
                temp.license.c_str(),
                temp.surname.c_str(),
                temp.address.c_str(),
                temp.release_year.c_str()
            );
        
            printf("Продолжаем запись сведений\n");
            temp = {};
            filled = false;
        }
        printf("Введите поле %s:\n", field_names[field_index].c_str());
    }
    fclose(file);
    if (file) printf("Не удалось закрыть файл: %s\n", file_name.c_str());
    else printf("Файл %s успешно закрыт\n", file_name.c_str());
    // clear stdin to ignore EOF
    clearerr(stdin);
    return;
}