#include "file_handler.hpp"

// Инициализируем основные стандартные параметры
std::string DEFAULT_FILENAME_1 = "car_data.txt";
std::string DEFAULT_FILENAME_2 = "license_data.txt";

// функция валидации имени файла
bool validate_filename(const std::string& filename) {
    return std::regex_match(filename, std::regex("[a-zA-Z0-9_-]+\\.txt"));
}

// функция валидации поля "номерной знак"
bool validate_license(const std::string& license) {
    // M976MM777RUS 
    return regex_match(license, std::regex("[ABEKMHOPCTYX][0-9]{3,3}[ABEKMHOPCTYX]{2,2}[0-9]{2,3}RUS"));
}

// функция валидации поля "бренд"
bool validate_brand(const std::string& brand) {
    return regex_match(brand, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9-_.! ]+"));
}

// функция валидации поля "модель"
bool validate_model(const std::string& model) {
    return regex_match(model, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9-_.!() ]+"));
}

// функция валидации поля "фамилия владельца"
bool validate_surname(const std::string& surname) {
    return regex_match(surname, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z]+"));
}

// функция валидации поля "адрес владельца"
bool validate_address(const std::string& address) {
    return regex_match(address, std::regex("[ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-яA-Za-z0-9_. ]+"));
}

// функция валидации поля "год выпуска"
// Проверка заключается в проверке на число и на принадлежность временному диапазону,
bool validate_release_year(const std::string& release_year) {
    static size_t lower_bound_year = 1960;
    std::time_t t = std::time(nullptr);
    std::tm *const pTInfo = std::localtime(&t);
    static size_t current_year = 1900 + pTInfo->tm_year;
    
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
    if (!(year >= lower_bound_year && year <= current_year) || year <= 0) {
        printf("Год вне диапазона(%d-%d)\n", lower_bound_year, current_year);
        result = false;
    }
    return result;
}

// функция для определения имени и режима открытия файла
std::pair<std::string, std::string> file_info(char* BUFFER, const size_t& BUF_SIZE, const std::string& default_file_name) {
    std::pair<std::string, std::string> file_info;
    std::string file_name;
    printf("Введите название файла(enter=%s): ", default_file_name.c_str());
    while (scanf("%255[^\n]s", BUFFER)) {
		__fpurge(stdin);
		file_name = BUFFER;
		if (file_name == "q" || file_name == "Q") {
			break;
		}
        if (validate_filename(file_name)) break;
        else {
            printf("Введенное имя файла некорректно. Файл должен иметь вид file.txt, \n");
            printf("где file состоит из латинских символов, дефиса(-) или нижнего подчеркивания(_),\n");
            printf("а после следует его расширение .txt\n");
            printf("Введите название файла(enter=%s): ", default_file_name.c_str());
			file_name = default_file_name;
        }
    }
	__fpurge(stdin);
	if (file_name.size()==0) file_name = default_file_name;
    file_info.first = file_name;
    // Дальше идет проверка существует ли данный файл, если да, то необходимо спросить у пользователя,
    // как именно предстоит работать с ним.
    if (std::filesystem::exists(file_name)) {
        printf("Файл %s уже существует.\nЖелаете продолжить запись вместо заполнения с начала(enter=Да/N=Нет)?", file_name.c_str());
		while ((BUFFER[0] = getchar()) != EOF) {
            if (BUFFER[0] == '\n') {
				printf("Продолжаем запись сведений после существующих.");
                file_info.second = "a";
                break;
            } else {
				__fpurge(stdin);
				if (BUFFER[0] == 'n' || BUFFER[0] == 'N') {
					printf("Заполняем файл %s сведениями с нуля.", file_name.c_str());
					file_info.second = "w";
					break;
				} else {
					printf("Введено '%c' можно ввести только enter или N\n", BUFFER[0]);
				}
			}
        }
    } else file_info.second = "w";
	return file_info;
}

// функция для записи сведений в файл, 
//	option 1: car_data
//	option 2: license_data
void write_data(char* BUFFER, const size_t& BUF_SIZE, const size_t& option) {
	std::pair<std::string, std::string> file_information;
	
	if (option == 1)
		file_information = file_info(BUFFER, BUF_SIZE, DEFAULT_FILENAME_1);
	else if (option == 2)
		file_information = file_info(BUFFER, BUF_SIZE, DEFAULT_FILENAME_2);
    std::string file_name = file_information.first;
    FILE* file = std::fopen(file_name.c_str(), file_information.second.c_str());
    
    // проверка на открытие файла
    if (!file) {
        printf("Не удалось открыть файл %s\n", file_name.c_str());
        return;
    }
    printf("Файл %s успешно открыт для записи\n", file_name.c_str());

    int field_index=0;
	printf("Начинаем заполнять сведения.\nДля завершения ввода нажмите 'Q' либо 'q'.\n");
	int note_number = 0;
	
	const std::string mask = "%"+std::to_string(BUF_SIZE-1)+"[^\n]s";
	CarData temp_1 = {};
	LicenseData temp_2 = {};
	std::string field_names_1[3] = {
		"бренд (символы: А-ЯЁа-яёA-Za-z0-9-_.! )",
		"модель (символы: А-ЯЁа-яёA-Za-z0-9-_.!() )",
		"номерной знак (пример: M976MM777RUS)"
	};
	std::string field_names_2[4] = {
		"номерной знак (пример: M976MM777RUS)", 
		"фамилия владельца (символы: А-ЯЁа-яёA-Za-z)", 
		"адрес владельца (символы: A-Za-zА-ЯЁа-яё0-9!_-. )", 
		"год выпуска (символы: 0-9)"
	};
	
	if (option == 1)
		printf("Введите поле %s:\n", field_names_1[field_index].c_str());
	else if (option == 2)
		printf("Введите поле %s:\n", field_names_2[field_index].c_str());
		
	while (scanf(mask.c_str(), BUFFER) != EOF) {
		__fpurge(stdin);
		if ((BUFFER[0]=='Q' || BUFFER[0] == 'q') && BUFFER[1] == '\0') {
			printf("Введен %s. Ввод сведений закончен\n", BUFFER);
			break;
		}
		switch (field_index) {
		case 0:
			if (option == 1) {
				temp_1.brand.assign(BUFFER);
				if (validate_brand(temp_1.brand))
					field_index=(field_index+1)%3;
				else printf("Поле введено некорректно, попробуйте еще раз\n");
			} else if (option == 2) {
				temp_2.license.assign(BUFFER);
				if (validate_license(temp_2.license)) {
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
			}
			break;
		case 1:
			if (option == 1) {
				temp_1.model.assign(BUFFER);
				if (validate_model(temp_1.model)) {
					field_index=(field_index+1)%3;
					break;
				}
			} else if (option == 2) {
				temp_2.surname.assign(BUFFER);
				if (validate_surname(temp_2.surname)) {
					field_index = (field_index+1)%4;
					break;
				}
			}
			printf("Поле введено некорректно, попробуйте еще раз\n");
			break;
		case 2:
			if (option == 1) {
				temp_1.license.assign(BUFFER);
				if (validate_license(temp_1.license)) {
					field_index = (field_index+1)%3;
					fprintf(file, "%s %s %s\n", 
						temp_1.brand.c_str(), 
						temp_1.model.c_str(), 
						temp_1.license.c_str()
					);
					printf(
						"Запись #%d завершена:\nбренд: %s\nмодель: %s\nномерной знак: %s\n",
						++note_number,
						temp_1.brand.c_str(),
						temp_1.model.c_str(),
						temp_1.license.c_str()
					);
					printf("Продолжаем запись сведений\n");
					temp_1 = {};
				} else {
					printf("Поле введено некорректно, попробуйте еще раз\n");
					printf("Первый элемент должен быть 1 из [ABEKMHOPCTYX],\n");
					printf("Следующие 3 элемента это цифры: [0-9]\n");
					printf("Следующие 2 элемента должны быть из [ABEKMHOPCTYX]\n");
					printf("Следующие 2-3 элемента это цифры: [0-9]\n");
					printf("После всего этого следует строчка: RUS\n");
					printf("Номерной знак не должен содержать никаких разделителей\n");
				}
			} else if (option == 2) {
				temp_2.address.assign(BUFFER);
				if (validate_address(temp_2.address))
					field_index = (field_index+1) % 4;
				else printf("Поле введено некорректно, попробуйте еще раз\n");
			}
			break;
		case 3:
			temp_2.release_year.assign(BUFFER);
			if (validate_release_year(temp_2.release_year)) {
				field_index = (field_index+1)%4;
				fprintf(file, "%s %s %s %s\n",
					temp_2.license.c_str(),
					temp_2.surname.c_str(),
					temp_2.address.c_str(),
					temp_2.release_year.c_str()
				);
				printf(
					"Запись #%d завершена:\nномерной знак: %s\nфамилия владельца: %s\nадрес: %s\nгод выпуска: %s\n",
					++note_number,
					temp_2.license.c_str(),
					temp_2.surname.c_str(),
					temp_2.address.c_str(),
					temp_2.release_year.c_str()
				);
				printf("Продолжаем запись сведений\n");
				temp_2 = {};
			}
			else printf("Поле введено некорректно, попробуйте еще раз\n");
			break;
		}
		if (option == 1)
			printf("\nВведите поле %s:\n", field_names_1[field_index].c_str());
		else if (option == 2)
			printf("\nВведите поле %s:\n", field_names_2[field_index].c_str());
		memset(BUFFER, 0x00, BUF_SIZE);
	}
    fclose(file);
    clearerr(stdin); // Здесь происходит очистка потока ввода от EOF
}
