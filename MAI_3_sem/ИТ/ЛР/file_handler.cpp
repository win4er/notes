#include "file_handler.hpp"

// TODO: 
// + пользователь должен иметь возможность изменить введенное поле сразу после совершения ошибки,
// a не после заполнения остальных полей
// + уменьшить код и избавиться от ненужных штук

int validate_car_data(CarData info) {
    bool result = true;
    int sep_pos=6;
    if (info.license.size() < sep_pos) {
        printf("Неправильный формат номерного знака, длина строки меньше 6\n");
        return false;
    }
    std::string left_side = info.license.substr(0, sep_pos);
    std::string right_side = info.license.substr(sep_pos, info.license.size()-sep_pos);
    // представитель РФ
    // M976MM777RUS
    std::regex regular_l_pattern{
        "[ABEKMHOPCTYX][0-9]{3,3}[ABEKMHOPCTYX]{2,2}"
    };
    std::regex regular_r_pattern{
        "[0-9]{2,3}RUS"
    };
    bool condition = regex_match(left_side, regular_l_pattern) && regex_match(right_side, regular_r_pattern);
    if (!condition) {
        printf("Неправильный формат номерного знака\n");
        result = false;
    }    
    return result;
}

int validate_license_data(LicenseData info) {
    int result = 0;
    if (info.license.size() < 6) {
        printf("Неправильный формат номерного знака, длина строки меньше 6\n");
        return false;
    }
    std::string left_side = info.license.substr(0, 6);
    std::string right_side = info.license.substr(6, info.license.size()-6);

    std::regex regular_l_pattern{
        "[ABEKMHOPCTYX][0-9]{3,3}[ABEKMHOPCTYX]{2,2}"
    };
    std::regex regular_r_pattern{
        "[0-9]{2,3}RUS"
    };

    bool condition = regex_match(left_side, regular_l_pattern) && regex_match(right_side, regular_r_pattern);
    if (!condition) {
        printf("Неправильный формат номерного знака\n");
        result = false;
    }
    int year=0;
    for (int i=0; i<info.release_year.size(); ++i) {
        if (!isdigit(info.release_year[i])) {
            printf("Год должен содержать только цифры\n");
            result = false;
            break;
        } else {
            year *= 10;
            year += (info.release_year[i]-'0');
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

void make_file_car_data() {
    std::string file_name, default_file_name = "car_data.txt";
    FILE* file;
    bool exist;
    printf("Введите название файла(enter=%s):\n", default_file_name.c_str());
    while (scanf("%64[^\n]s", &file_name[0])) {
        fseek(stdin, 0, SEEK_END);
        exist = std::filesystem::exists(file_name);
        if (!exist) {
            file = std::fopen(file_name.c_str(), "w");
            if (file) {
                fclose(file);
                remove(file_name.c_str());
                break;
            }
        } else break;
        printf("Введенное имя файла неккоректно, введите другое\n");
    }
    fseek(stdin, 0, SEEK_END);
    if (file_name.size()==0) {
        file_name = default_file_name;
    }
    exist = std::filesystem::exists(file_name);
    if (exist) {
        printf("Файл %s уже существует. Желаете продолжить запись вместо заполнения с начала? (Y/N)\n", file_name.c_str());
        char option;
        while (option = getchar()) {
            fseek(stdin, 0, SEEK_END);
            if (option == 'Y' || option == 'y') {
                file = std::fopen(file_name.c_str(), "a");        
                break;
            } else
            if (option == 'N' || option == 'n') {
                file = std::fopen(file_name.c_str(), "w");
                break;
            } else printf("Введено %s можно ввести только Y или N\n");
        }
    } else file = std::fopen(file_name.c_str(), "w");   
    if (!file) {
        printf("Не удалось открыть файл '%s'\n", file_name.c_str());
        return;
    }
    printf("Файл %s успешно открыт для записи\n", file_name.c_str());

    char buffer[32+1];
    int field=1;
    CarData temp = {};
    printf("Начинаем заполнять автомобильные сведения.\nДля завершения ввода нажмите 'Q' либо 'q'.\n");
    printf("Поля идут в заданном порядке:\n");
    printf("Поле #1: [бренд]\nПоле #2: [модель]\nПоле #3: [номерной знак]\n");
    printf("Введите поле #%d:\n", field);
    bool filled = false;
    int note_number = 0;
    while (scanf("%32[^\n]s", buffer) != EOF) {
        if ((buffer[0]=='Q' || buffer[0] == 'q') && buffer[1] == '\0') {
            printf("Введен %s. Ввод сведений закончен\n", buffer);
            fseek(stdin, 0, SEEK_END);
            break;
        }
        // to skip the rest of the input
        fseek(stdin, 0, SEEK_END);
        switch (field) {
            case 1: {temp.brand.assign(buffer); break;}
            case 2: {temp.model.assign(buffer); break;}
            case 3: {temp.license.assign(buffer); filled = true; break;}
            field = (field+1)%3;
        }
        if (filled) {
            if (!validate_car_data(temp)) {
                printf("Запись содержит ошибки, исправьте содержимое.\n");
            } else {
                fwrite(temp.brand.c_str(), 1, temp.brand.size(), file);
                fputc(' ', file);
                fwrite(temp.model.c_str(), 1, temp.model.size(), file);
                fputc(' ', file);
                fwrite(temp.license.c_str(), 1, temp.license.size(), file);
                fputc('\n', file);
                printf("Запись #%d завершена:\nбренд: '%s'\nмодель: '%s'\nномерной знак: '%s'\n", ++note_number, temp.brand.c_str(), temp.model.c_str(), temp.license.c_str());    
            }
            printf("Продолжаем запись сведений\n");
            temp = {};
            filled = false;
        }
        printf("Введите поле #%d:\n", field);
    }
    fclose(file);
    if (!file) printf("Не удалось закрыть файл: %s\n", file_name.c_str());
    else printf("Файл %s успешно закрыт\n", file_name.c_str());
    // clear stdin to ignore EOF
    clearerr(stdin);
    return;
}

void make_file_license_data() {
    std::string file_name, default_file_name = "license_data.txt";
    FILE* file;
    printf("Введите название файла(enter=%s):\n", default_file_name.c_str());
    while (scanf("%64[^\n]s", &file_name[0])) {
        fseek(stdin, 0, SEEK_END);
        file = std::fopen(file_name.c_str(), "a");
        if (file) {
            fclose(file);
            remove(file_name.c_str());
            break;
        }
        printf("Введенное имя файла неккоректно, введите другое\n");
    }
    fseek(stdin, 0, SEEK_END);
    if (file_name.size()==0) {
        file_name = default_file_name;
    }
    bool exist = std::filesystem::exists(file_name);
    if (exist) {
        printf("Файл %s уже существует. Желаете продолжить запись вместо заполнения с начала? (Y/N)\n", file_name.c_str());
        char option;
        while (option = getchar()) {
            fseek(stdin, 0, SEEK_END);
            if (option == 'Y' || option == 'y') {
                file = std::fopen(file_name.c_str(), "a");
                break;
            } else
            if (option == 'N' || option == 'n') {
                file = std::fopen(file_name.c_str(), "w");
                break;
            } else {
                printf("Введено %s можно ввести только 'Y' или 'N'\n");
            }
        }
    } else {
        file = std::fopen(file_name.c_str(), "w");        
    }
    if (!file) {
        printf("Не удалось открыть файл %s\n", file_name.c_str());
        return;
    }
    printf("Файл %s успешно открыт для записи\n", file_name.c_str());

    char buffer[32+1];
    int field=1;
    LicenseData temp = {};

    printf("Начинаем заполнять регистрационные сведения.\nДля завершения ввода нажмите 'Q' либо 'q'.\n");
    printf("Поля идут в заданном порядке:\n");
    printf("Поле #1: [номерной знак]\nПоле #2: [фамилия владельца]\nПоле #3: [адрес владельца]\nПоле #4: [год выпуска]\n");
    printf("Введите поле #%d:\n", field);
    bool filled = false;
    int note_number = 0;
    while (scanf("%32[^\n]s", buffer) != EOF) {
        if ((buffer[0]=='Q' || buffer[0] == 'q') && buffer[1] == '\0') {
            printf("Введен %s. Ввод сведений закончен\n", buffer);
            fseek(stdin, 0, SEEK_END);
            break;
        }
        // to skip the rest of the input
        fseek(stdin, 0, SEEK_END);
        switch (field) {
            case 1: {temp.license.assign(buffer); break;}
            case 2: {temp.surname.assign(buffer); break;}
            case 3: {temp.address.assign(buffer); break;}
            case 4: {temp.release_year.assign(buffer);filled = true;break;}
            field = (field+1)%4;
        }
        if (filled) {
            if (!validate_license_data(temp)) {
                printf("Запись содержит ошибки, исправьте содержимое.\n");
            } else {
                fwrite(temp.license.c_str(), 1, temp.license.size(), file);
                fputc(' ', file);
                fwrite(temp.surname.c_str(), 1, temp.surname.size(), file);
                fputc(' ', file);
                fwrite(temp.address.c_str(), 1, temp.address.size(), file);
                fputc(' ', file);
                fwrite(temp.release_year.c_str(), 1, temp.release_year.size(), file);
                fputc('\n', file);
                printf("Запись #%d завершена:\nномерной знак: '%s'\nфамилия владельца: '%s'\nадрес: '%s'\nгод выпуска: '%s'\n", ++note_number, temp.license.c_str(), temp.surname.c_str(), temp.address.c_str(), temp.release_year.c_str());    
            }
            printf("Продолжаем запись сведений\n");
            temp = {};
            filled = false;
        }
        printf("Введите поле #%d:\n", field);
    }
    if (!file) {
        printf("Не удалось закрыть файл: %s\n", file_name.c_str());
    } else {
        printf("Файл %s успешно закрыт\n", file_name.c_str());
    }
    // clear stdin to ignore EOF
    clearerr(stdin);
    return;
}

void program_info() {
    printf("Назначение программы: Создание файлов на основе сведений, вводимых пользователем\n"); 
}

void UI() {
    printf("Что требуется записать?\n1)Файл автомобильных сведений\n2)Файл регистрационных сведений\n");
    
    char input='\0';
    while (scanf("%c", &input) != EOF) {
        printf("Введено: %c\n", input);
        fseek(stdin, 0, SEEK_END);
        if (input == '1' || input == '2') {
            if (input == '1') make_file_car_data();
            else 
            if (input == '2') make_file_license_data();
            printf("Желаете продолжить запись? (Y/N)\n");
            input = getchar();
            fseek(stdin, 0, SEEK_END);
            if (input == 'N' || input == 'n') {
                break;
            } else
            if (input == 'Y' || input == 'y') {
                printf("Что требуется записать?\n1)Файл автомобильных сведений\n2)Файл регистрационных сведений\n");
            } else printf("Введен неизвестный символ, программа продолжает работу.\n");
        } else {
            printf("Введенный символ не подходит. Можно ввести только 1 или 2.");
            printf("Что требуется записать?\n1)Файл автомобильных сведений\n2)Файл регистрационных сведений\n");
        }
    }
    printf("Нажмите любую клавишу для завершения программы...\n");
    scanf("%c", &input);
    printf("Программа завершена\n");
}