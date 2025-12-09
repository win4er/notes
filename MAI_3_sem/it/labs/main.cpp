#include "file_handler.hpp"

int main() {
    setlocale(LC_ALL, "en_RU.UTF-8");
    printf("Назначение программы: Создание файлов на основе сведений, вводимых пользователем\n");
    printf("Что требуется записать?\n1) Файл автомобильных сведений\n2) Файл регистрационных сведений\n");
    printf("q) Для завершения программы нажмите q.\n");
    char input;
    while ((input = getchar()) != EOF) {
        clear_stdin();
        if (input == '1' || input == '2') {
            if (input == '1') write_car_data();
            else 
            if (input == '2') write_license_data();
        } else if (input == 'q' || input == 'Q') {
            break;
        }
        else printf("Данной опции не существует.\n");

        printf("Желаете продолжить запись? (enter=Да/N=Нет)\n");
        input = getchar();
        if (input == 'N' || input == 'n') {
            break;
        } else
        if (input == '\n') {
            printf("Введен enter, программа продолжает работу\n");
        } else printf("Введен %c, программа продолжает работу\n", input);

        printf("Что требуется записать?\n1) Файл автомобильных сведений\n2) Файл регистрационных сведений\n");
        printf("q) Для завершения программы нажмите q.\n");
    }
    printf("Нажмите любую клавишу для завершения программы...");
    scanf("%c", &input);
    printf("Программа завершена.\n");
    return 0;
}
