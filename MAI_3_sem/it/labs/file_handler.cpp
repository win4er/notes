#include "file_handler.hpp"


// буффер ввода
char BUFFER[256];

// Инициализируем основные стандартные параметры
std::string DEFAULT_FILENAME_1 = "car_data.txt";
std::string DEFAULT_FILENAME_2 = "license_data.txt";
int LOWER_BOUND_YEAR = 1960;

// Стоит отметить, что почти все функции валидации используют
// регулярные выражения, сделано это для упрощения проверок

// функция валидации имени файла
bool validate_filename(std::string filename) {
    std::regex regex_filename{"[a-zA-Z0-9_-]+\\.txt"};
    return std::regex_match(filename, regex_filename);
}

// функция валидации поля "номерной знак"
bool validate_license(std::string license) {
    // M976MM777RUS
    std::regex pattern{"[ABEKMHOPCTYX][0-9]{3,3}[ABEKMHOPCTYX]{2,2}[0-9]{2,3}RUS"};
    return regex_match(license, pattern);
}

// функция валидации поля "бренд"
bool validate_brand(std::string brand) {
    std::regex pattern{"[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9-_.! ]+"};
    return regex_match(brand, pattern);
}

// функция валидации поля "модель"
bool validate_model(std::string model) {
    std::regex pattern{"[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9-_.!() ]+"};
    return regex_match(model, pattern);
}

// функция валидации поля "фамилия владельца"
bool validate_surname(std::string surname) {
    std::regex pattern{"[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z]+"};
    return regex_match(surname, pattern);
}

// функция валидации поля "адрес владельца"
bool validate_address(std::string address) {
    std::regex address_pattern{"[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9!_. ]+"};
    return regex_match(address, address_pattern);
}

// функция валидации поля "год выпуска"
// Здесь проверка не содержит регулярных выражений
// Проверка заключается в попытке перевести пользовательский ввод в число,
// а затем в проверке подходит ли число временному диапазону
bool validate_release_year(std::string release_year) {
    bool result=true;
    if (release_year.size()==0) {
        printf("Введена пустая строка.\n");
        return false;
    }
    int year=0;
    int ten = 1;
    for (int i=release_year.size()-1; i>=0; --i) {
        if (!isdigit(release_year[i])) {
            printf("Строка не полностью состоит из цифр.\n");
            return false;
        }
        year += (release_year[i]-'0')*ten;
        ten *= 10;
    }
    std::time_t t = std::time(nullptr);
    std::tm *const pTInfo = std::localtime(&t);
    int current_year = 1900 + pTInfo->tm_year;
    if (!(year >= LOWER_BOUND_YEAR && year <= current_year) || year <= 0) {
        printf("Год вне диапазона(%d-%d)\n", LOWER_BOUND_YEAR, current_year);
        result = false;
    }
    return result;
}

// функция для определения имени и режима открытия файла
// Она спрашивает у пользователя имя файла и, если
// ОС позволяет использовать данное имя файла, то
// файл будет с заданным пользователем именем файла,
// иначе надо будет вводить имя файла, пока оно не удовлетворит критериям ОС.
std::pair<std::string, std::string> file_info(std::string default_file_name) {
    std::pair<std::string, std::string> file_info;
    std::string file_name;
    printf("Введите название файла(enter=%s): ", default_file_name.c_str());
    while (scanf("%255[^\n]s", BUFFER)) {
        fseek(stdin, 0, SEEK_END);
        file_name = BUFFER;
		memset(BUFFER, 0xFF, 256);
		BUFFER[0]='\0';
        if (validate_filename(file_name)) break;
        else {
            printf("Введенное имя файла некорректно. Файл должен иметь вид file.txt, \n");
            printf("где file состоит из латинских символов, дефиса(-) или нижнего подчеркивания(_),\n");
            printf("а после следует его расширение .txt\n");
            printf("Введите название файла(enter=%s): ", default_file_name.c_str());
        }
    }
	if (file_name.size()==0 || BUFFER[0]=='\0') file_name = default_file_name;
    file_info.first = file_name;
    // Дальше идет проверка существует ли данный файл, если да, то необходимо спросить у пользователя,
    // как именно предстоит работать с ним.
    if (std::filesystem::exists(file_name)) {
        printf("Файл %s уже существует.\nЖелаете продолжить запись вместо заполнения с начала(enter=Да/N=Нет)?", file_name.c_str());
        fseek(stdin, 0, SEEK_END);
        while ((BUFFER[0] = getchar()) != EOF) {
            if (BUFFER[0] == '\n') {
                file_info.second = "a";
                break;
            } else
            if (BUFFER[0] == 'n' || BUFFER[0] == 'N') {
                file_info.second = "w";
                break;
            } else printf("Введено '%c' можно ввести только enter или N\n", BUFFER[0]);
        }
    } else file_info.second = "w";
    fseek(stdin, 0, SEEK_END);
	memset(BUFFER, 0xFF, 256);
	BUFFER[0]='\0';
	return file_info;
}

// функция записи автомобильных сведений в файл
void write_car_data() {
    std::pair<std::string, std::string> file_information = file_info(DEFAULT_FILENAME_1);
    std::string file_name = file_information.first;
    FILE* file = std::fopen(file_name.c_str(), file_information.second.c_str());
    
    // проверка на открытие файла
    if (!file) {
        printf("Не удалось открыть файл %s\n", file_name.c_str());
        return;
    }
    printf("Файл %s успешно открыт для записи\n", file_name.c_str());

    int field_index=0;
    CarData temp = {};
    printf("Начинаем заполнять автомобильные сведения.\nДля завершения ввода нажмите 'Q' либо 'q'.\n");
    std::string field_names[3] = {"бренд (символы: А-ЯЁа-яёA-Za-z0-9-_.! )", "модель (символы: А-ЯЁа-яёA-Za-z0-9-_.!() )", "номерной знак (пример: M976MM777RUS)"};
    printf("Введите поле %s:\n", field_names[field_index].c_str());
    bool filled = false;
    int note_number = 0;

    // Ниже идет последовательная запись полей и соответствующих им записей в файл
    // Если поле не проходит валидацию, то оно не будет занесено в файл, 
    // пока не пройдет валидацию.
    while (scanf("%255[^\n]s", BUFFER) != EOF) {
        if ((BUFFER[0]=='Q' || BUFFER[0] == 'q') && BUFFER[1] == '\0') {
            printf("Введен %s. Ввод сведений закончен\n", BUFFER);
			fseek(stdin, 0, SEEK_END);
			memset(BUFFER, 0xFF, 256);
			BUFFER[0]='\0';
            break;
        }
		fseek(stdin, 0, SEEK_END);
        switch (field_index) {
            case 0: {
                temp.brand.assign(BUFFER);
				memset(BUFFER, 0xFF, 256);
				BUFFER[0]='\0';
                if (validate_brand(temp.brand))
                    field_index=(field_index+1)%3;
                else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }
            case 1: {
                temp.model.assign(BUFFER);
				memset(BUFFER, 0xFF, 256);
				BUFFER[0]='\0';
                if (validate_model(temp.model))
                    field_index=(field_index+1)%3;
                else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }
            case 2: {
                temp.license.assign(BUFFER);
				memset(BUFFER, 0xFF, 256);
				BUFFER[0]='\0';
                if (validate_license(temp.license)) {
                    field_index = (field_index+1)%3;
                    filled = true;
                } else {
					printf("Поле введено некорректно, попробуйте еще раз\n");
					printf("Первый элемент должен быть 1 из [ABEKMHOPCTYX],\n");
					printf("Следующие 3 элемента это цифры: [0-9]\n");
					printf("Следующие 2 элемента должны быть из [ABEKMHOPCTYX]\n");
					printf("Следующие 2-3 элемента это цифры: [0-9]\n");
					printf("После всего этого следует строчка: RUS\n");
					printf("Номерной знак не должен содержать никаких разделителей\n");
				}
                break;
            }
        }
        if (filled) {
            fprintf(file, "%s %s %s\n", temp.brand.c_str(), temp.model.c_str(), temp.license.c_str());
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
			memset(BUFFER, 0xFF, 256);
			BUFFER[0]='\0';
        }
        printf("Введите поле %s:\n", field_names[field_index].c_str());
    }
    fclose(file);
    // Здесь происходит очистка потока ввода от EOF
    clearerr(stdin);
    return;
}

// функция записи регистрационных сведений в файл
// она польностью аналогична функции write_car_data(),
// лишь за тем исключением что записывает другие поля
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
    std::string field_names[4] = {"номерной знак (пример: M976MM777RUS)", "фамилия владельца (символы: А-ЯЁа-яёA-Za-z)", "адрес владельца (символы: A-Za-zА-ЯЁа-яё0-9!_-. )", "год выпуска (символы: 0-9)"};
    printf("Введите поле %s:\n", field_names[field_index].c_str());
    bool filled = false;
    int note_number = 0;
    while (scanf("%255[^\n]s", BUFFER) != EOF) {
        if ((BUFFER[0]=='Q' || BUFFER[0] == 'q') && BUFFER[1] == '\0') {
            printf("Введен %s. Ввод сведений закончен\n", BUFFER);
			fseek(stdin, 0, SEEK_END);
            break;
        }
		fseek(stdin, 0, SEEK_END);
        switch (field_index) {
            case 0: {
                temp.license.assign(BUFFER);
				memset(BUFFER, 0xFF, 256);
                if (validate_license(temp.license)) {
                    field_index = (field_index+1)%4;
                } else {
					printf("Поле введено некорректно, попробуйте еще раз\n");
					printf("Первый элемент должен быть 1 из [ABEKMHOPCTYX],\n");
					printf("Следующие 3 элемента это цифры: [0-9]\n");
					printf("Следующие 2 элемента должны быть из [ABEKMHOPCTYX]\n");
					printf("Следующие 2-3 элемента это цифры: [0-9]\n");
					printf("После всего этого следует строчка: RUS\n");
					printf("Номерной знак не должен содержать никаких разделителей\n");
				}
                break;
            }
            case 1: {
                temp.surname.assign(BUFFER);
				memset(BUFFER, 0xFF, 256);
				BUFFER[0]='\0';
                if (validate_surname(temp.surname)) {
                    field_index = (field_index+1)%4;
                } else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }
            case 2: {
                temp.address.assign(BUFFER);
				memset(BUFFER, 0xFF, 256);
				BUFFER[0]='\0';
                if (validate_address(temp.address)) {
                    field_index = (field_index+1)%4;
                } else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }
            case 3: {
                temp.release_year.assign(BUFFER);
				memset(BUFFER, 0xFF, 256);
				BUFFER[0]='\0';
                if (validate_release_year(temp.release_year)) {
                    field_index = (field_index+1)%4;
                    filled = true;
                } else printf("Поле введено некорректно, попробуйте еще раз\n");
                break;
            }

        }
        if (filled) {
            fprintf(file, "%s %s %s %s\n", temp.license.c_str(), temp.surname.c_str(), temp.address.c_str(), temp.release_year.c_str());
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
    // очищаем поток от EOF
    clearerr(stdin);
	memset(BUFFER, 0xFF, 256);
	BUFFER[0]='\0';
    return;
}
