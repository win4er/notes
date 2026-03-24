#include "file_handler.hpp"

// Объявляем стандартные имена файлов
const std::string DEFAULT_FILENAME_1 = "car_data.txt";
const std::string DEFAULT_FILENAME_2 = "license_data.txt";

// Ниже представлены реализации функций валидаторов
bool validate_filename(const std::string& filename) {
    return std::regex_match(filename, std::regex("[a-zA-Z0-9_-]+\\.txt$"));
}

bool validate_license(const std::string& license) {
    bool cond1 = std::regex_match(license, std::regex("[ABEKMHOPCTYX][0-9]{3}[ABEKMHOPCTYX]{2}[0-9]{2,3}RUS$"));
    bool cond2 = std::regex_match(license, std::regex("[- ]+$"));
	return cond1 || cond2;
}

bool validate_brand(const std::string& brand) {
    return std::regex_match(brand, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9-_. -]+$"));
}

bool validate_model(const std::string& model) {
    return std::regex_match(model, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9-_.() -]+$"));
}

bool validate_surname(const std::string& surname) {
    return std::regex_match(surname, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z -]+$"));
}

bool validate_address(const std::string& address) {
    return std::regex_match(address, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9_., -]+$"));
}
bool validate_release_year(const std::string& release_year) {
	if (std::regex_match(release_year, std::regex("[ -]+$"))) {
		return true;
	}
    static size_t lower_bound_year = 1960;
    std::time_t t = std::time(nullptr);
    std::tm *const pTInfo = std::localtime(&t);
    static size_t current_year = 1900 + pTInfo->tm_year;
    
    if (release_year.empty()) {
        printf("Ошибка: год выпуска не может быть пустым.\n");
        return false;
    }
    
    // Проверка длины строки (год не может быть длиннее 4 цифр)
    if (release_year.length() > 4) {
        printf("Ошибка: год должен состоять из 4 цифр (например: 2015).\n");
        return false;
    }
    
    for (char c : release_year) {
        if (!isdigit(c)) {
            printf("Ошибка: год должен состоять только из цифр (например: 2015).\n");
            return false;
        }
    }
    
    // Безопасное преобразование с обработкой исключения
    int year = 0;
    try {
        year = std::stoi(release_year);
    } catch (const std::out_of_range&) {
        printf("Ошибка: введенное значение слишком большое.\n");
        return false;
    } catch (const std::invalid_argument&) {
        printf("Ошибка: неверный формат года.\n");
        return false;
    }
    
    if (year < lower_bound_year) {
        printf("Ошибка: год выпуска не может быть раньше %zu года.\n", lower_bound_year);
        return false;
    }
    if (year > current_year) {
        printf("Ошибка: год выпуска не может быть позже текущего (%zu).\n", current_year);
        return false;
    }
    
    return true;
}

// Функция для выбора режима работы с файлом.
std::pair<std::string, std::string> file_info(
    char* BUFFER, 
    const size_t& BUF_SIZE, 
    const std::string& default_file_name) {
    
    std::pair<std::string, std::string> result;
    std::string file_name;
    
    printf("Введите название файла (Enter='%s', 'q'=меню): ", default_file_name.c_str());
    fflush(stdout);
    
    while (fgets(BUFFER, BUF_SIZE, stdin) != nullptr) {
        __fpurge(stdin);
        BUFFER[strcspn(BUFFER, "\n")] = 0;
        file_name = BUFFER;
        
        if (file_name == "q" || file_name == "Q") {
            result.first = "";
            result.second = "";
            return result;
        }
        
        if (file_name.empty()) {
            file_name = default_file_name;
            break;
        }
        
        if (validate_filename(file_name)) {
            break;
        } else {
            printf("Ошибка: неверное имя файла.\n");
            printf("Имя должно содержать только латинские буквы, цифры, дефис или подчеркивание\n");
            printf("и иметь расширение .txt (например: my_data.txt или car-2024.txt)\n\n");
            printf("Введите название файла (Enter='%s', 'q'=меню): ", default_file_name.c_str());
            fflush(stdout);
        }
    }
    
    result.first = file_name;
    
    if (std::filesystem::exists(file_name)) {
        printf("\nФайл '%s' уже существует.\n", file_name.c_str());
        printf("Enter - добавить записи в конец\n");
        printf("N - перезаписать файл с начала\n");
        printf("q - вернуться в главное меню\n");
        printf("Ваш выбор: ");
        fflush(stdout);
        
        if (fgets(BUFFER, BUF_SIZE, stdin) != nullptr) {
            __fpurge(stdin);
            if (BUFFER[0] == 'q' || BUFFER[0] == 'Q') {
                result.first = "";
                result.second = "";
                return result;
            }
            
            if (BUFFER[0] == 'n' || BUFFER[0] == 'N') {
                result.second = "w";
                printf("Режим: перезапись файла\n");
            } else {
                result.second = "a";
                printf("Режим: добавление в конец файла\n");
            }
        } else {
            result.second = "a";
        }
        fflush(stdout);
    } else {
        result.second = "w";
        printf("Будет создан новый файл '%s'\n", file_name.c_str());
        fflush(stdout);
    }
    
    return result;
}

// Функция записи данных в файл
void write_data(char* BUFFER, const size_t& BUF_SIZE, const size_t& option) {
    std::pair<std::string, std::string> file_information;
    
    if (option == 1)
        file_information = file_info(BUFFER, BUF_SIZE, DEFAULT_FILENAME_1);
    else
        file_information = file_info(BUFFER, BUF_SIZE, DEFAULT_FILENAME_2);
    
    if (file_information.first.empty()) {
        printf("Возврат в главное меню\n");
        fflush(stdout);
        return;
    }
    
    FILE* file = std::fopen(file_information.first.c_str(), file_information.second.c_str());
    if (!file) {
        printf("Ошибка: не удалось открыть файл '%s'\n", file_information.first.c_str());
        printf("Проверьте права доступа или путь к файлу.\n");
        fflush(stdout);
        return;
    }
    
	// Выводим пользователю информацию об успешном открытии и знакомим
	// его с взаимодействием
    printf("\nФайл '%s' успешно открыт\n", file_information.first.c_str());
    printf("═══════════════════════════════════════\n");
    printf("            ВВОД ДАННЫХ\n");
    printf("═══════════════════════════════════════\n");
    printf("* 'q' - завершить ввод и сохранить файл, а затем вернуться в меню\n");
    printf("═══════════════════════════════════════\n");
    fflush(stdout);
    
    int field_index = 0;
    int note_number = 0;
    
	// Задаем массив имен полей CarData
    const std::string field_names_1[3] = {
        "МАРКА автомобиля",
        "МОДЕЛЬ автомобиля",
        "НОМЕРНОЙ ЗНАК"
    };
    
	// Задаем массив советов по заполнению полей CarData
    const std::string field_hints_1[3] = {
        "Разрешены: русские/английские буквы, цифры, пробел, дефис(-), подчеркивание(_), точка(.)",
        "Разрешены: русские/английские буквы, цифры, пробел, дефис(-), подчеркивание(_), точка(.), скобки()",
        "Формат: A999AA99RUS или A999AA999RUS (где A - одна из букв: A, B, E, K, M, H, O, P, C, T, Y, X) или дефисы и пробелы"
    };
    
	// Задаем массив имен полей LicenseData
    const std::string field_names_2[4] = {
        "НОМЕРНОЙ ЗНАК",
        "ФАМИЛИЯ владельца",
        "АДРЕС владельца",
        "ГОД ВЫПУСКА"
    };
    
	// Задаем массив советов по заполнению полей LicenseData
    const std::string field_hints_2[4] = {
        "Формат: A999AA99RUS или A999AA999RUS (где A - одна из букв: A, B, E, K, M, H, O, P, C, T, Y, X)",
        "Разрешены: буквы (русские/английские), дефис для двойных фамилий и пробел",
        "Разрешены: буквы, цифры, пробел, точка(.), запятая(,), дефис(-), подчеркивание(_)",
        "Четыре цифры от 1960 до текущего года, а также дефис(-) и пробел"
    };
    
	// Объявляем переменные куда будем первоначально записывать данные
    CarData temp_1;
    LicenseData temp_2;
    
    if (option == 1) {
        printf("\n--- Поле %d/3: %s ---\n", field_index + 1, field_names_1[field_index].c_str());
        printf("   %s\n", field_hints_1[field_index].c_str());
    } else {
        printf("\n--- Поле %d/4: %s ---\n", field_index + 1, field_names_2[field_index].c_str());
        printf("   %s\n", field_hints_2[field_index].c_str());
    }
    printf("> ");
    fflush(stdout);
   
	// Начинаем работу по заполнению файла
    while (fgets(BUFFER, BUF_SIZE, stdin) != nullptr) {
        __fpurge(stdin);
        BUFFER[strcspn(BUFFER, "\n")] = 0;
        std::string input = BUFFER;
        
        if (input == "q" || input == "Q") {
            printf("\nВвод завершен. Сохраняем данные...\n");
            break;
        }
        
        if (input.empty()) {
            printf("Ошибка: поле не может быть пустым. Введите значение или 'q' для выхода.\n");
        } else {
            bool valid = false;
            
            if (option == 1) {
                switch (field_index) {
                    case 0: // МАРКА
                        temp_1.brand = input;
                        if (validate_brand(temp_1.brand)) {
                            field_index = 1;
                            valid = true;
                            printf("Поле принято\n");
                        } else {
                            printf("Ошибка: неверный формат марки.\n");
                            printf("Используйте только разрешенные символы: буквы, цифры, пробел, . _ ! -\n");
                        }
                        break;
                    case 1: // МОДЕЛЬ
                        temp_1.model = input;
                        if (validate_model(temp_1.model)) {
                            field_index = 2;
                            valid = true;
                            printf("Поле принято\n");
                        } else {
                            printf("Ошибка: неверный формат модели.\n");
                            printf("Используйте только разрешенные символы: буквы, цифры, пробел, . _ ! ( ) -\n");
                        }
                        break;
                    case 2: // НОМЕРНОЙ ЗНАК
                        temp_1.license = input;
                        if (validate_license(temp_1.license)) {
                            fprintf(file, "%s %s %s\n", 
                                temp_1.brand.c_str(), 
                                temp_1.model.c_str(), 
                                temp_1.license.c_str());
                            note_number++;
                            printf("\nЗАПИСЬ #%d СОХРАНЕНА В ФАЙЛ\n", note_number);
                            printf("Марка: %s\n", temp_1.brand.c_str());
                            printf("Модель: %s\n", temp_1.model.c_str());
                            printf("Номер: %s\n", temp_1.license.c_str());
                            printf("──────────────────────────────────\n");
                            fflush(stdout);
                            field_index = 0;
                            temp_1 = CarData();
                            valid = true;
                        } else {
                            printf("Ошибка: неверный формат номерного знака.\n");
                            printf("Правильный формат: A999AA99RUS или A999AA999RUS\n");
                            printf("Где A - одна из букв: A, B, E, K, M, H, O, P, C, T, Y, X\n");
                            printf("Например: M976MM777RUS или A123BC98RUS\n");
                        }
                        break;
                }
            } else {
                switch (field_index) {
                    case 0:
                        temp_2.license = input;
                        if (validate_license(temp_2.license)) {
                            field_index = 1;
                            valid = true;
                            printf("Поле принято\n");
                        } else {
                            printf("Ошибка: неверный формат номерного знака.\n");
                            printf("Правильный формат: A999AA99RUS или A999AA999RUS\n");
                            printf("Где A - одна из букв: A, B, E, K, M, H, O, P, C, T, Y, X\n");
                            printf("Например: M976MM777RUS или A123BC98RUS\n");
                        }
                        break;
                    case 1:
                        temp_2.surname = input;
                        if (validate_surname(temp_2.surname)) {
                            field_index = 2;
                            valid = true;
                            printf("Поле принято\n");
                        } else {
                            printf("Ошибка: неверный формат фамилии.\n");
                            printf("Фамилия должна содержать только буквы (русские/английские).\n");
                            printf("Для двойных фамилий используйте дефис (например: Иванов-Петров).\n");
                        }
                        break;
                    case 2:
                        temp_2.address = input;
                        if (validate_address(temp_2.address)) {
                            field_index = 3;
                            valid = true;
                            printf("Поле принято\n");
                        } else {
                            printf("Ошибка: неверный формат адреса.\n");
                            printf("Используйте только буквы, цифры, пробел, точку, запятую, дефис, подчеркивание.\n");
                        }
                        break;
                    case 3:
                        temp_2.release_year = input;
                        if (validate_release_year(temp_2.release_year)) {
                            fprintf(file, "%s %s %s %s\n",
                                temp_2.license.c_str(),
                                temp_2.surname.c_str(),
                                temp_2.address.c_str(),
                                temp_2.release_year.c_str());
                            note_number++;
                            printf("\nЗАПИСЬ #%d СОХРАНЕНА В ФАЙЛ\n", note_number);
                            printf("Номер: %s\n", temp_2.license.c_str());
                            printf("Владелец: %s\n", temp_2.surname.c_str());
                            printf("Адрес: %s\n", temp_2.address.c_str());
                            printf("Год выпуска: %s\n", temp_2.release_year.c_str());
                            printf("──────────────────────────────────\n");
                            fflush(stdout);
                            field_index = 0;
                            temp_2 = LicenseData();
                            valid = true;
                        }
                        break;
                }
            }
            
            if (!valid && field_index != 3) {
                printf("Попробуйте еще раз или введите 'q' для выхода.\n");
                fflush(stdout);
            }
        }
        
        if (option == 1) {
            printf("\n--- Поле %d/3: %s ---\n", field_index + 1, field_names_1[field_index].c_str());
            printf("   %s\n", field_hints_1[field_index].c_str());
        } else {
            printf("\n--- Поле %d/4: %s ---\n", field_index + 1, field_names_2[field_index].c_str());
            printf("   %s\n", field_hints_2[field_index].c_str());
        }
        printf("> ");
        fflush(stdout);
    }

    fclose(file);

	// Выводим информацию по кол-ву заполненных записей.
    printf("\n═══════════════════════════════════════\n");
    printf("РАБОТА ЗАВЕРШЕНА\n");
    printf("Файл: %s\n", file_information.first.c_str());
    printf("Сохранено записей: %d\n", note_number);
    printf("═══════════════════════════════════════\n");
    fflush(stdout);
}
