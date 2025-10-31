#include "file_handler.hpp"

int main() {
    setlocale(LC_ALL, "rus_rus.1251");
    printf("Назначение программы: Создание файлов на основе сведений, вводимых пользователем\n");
    printf("Что требуется записать?\n1) Файл автомобильных сведений\n2) Файл регистрационных сведений\n");
    
    char input='\0';
    while (scanf("%c", &input) != EOF) {
        printf("Введено: %c\n", input);
        fseek(stdin, 0, SEEK_END);
        if (input == '1' || input == '2') {
            if (input == '1') write_car_data();
            else 
            if (input == '2') write_license_data();
            printf("Желаете продолжить запись? (enter=Да/N=Нет)\n");
            input = getchar();
            fseek(stdin, 0, SEEK_END);
            if (input == 'N' || input == 'n') {
                break;
            } else
            if (input == '\n') {
                printf("Что требуется записать?\n1) Файл автомобильных сведений\n2) Файл регистрационных сведений\n");
            } else printf("Введен %c, можно ввести только enter или N.\n", input);
        } else {
            printf("Введен %c. Можно ввести только 1 или 2. Попробуйте еще раз.\n", input);
            printf("Что требуется записать?\n1) Файл автомобильных сведений\n2) Файл регистрационных сведений\n");
        }
    }
    printf("Нажмите любую клавишу для завершения программы...\n");
    scanf("%c", &input);
    printf("Программа завершена\n");
    return 0;
}