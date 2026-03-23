#include "file_handler.hpp"
#include <clocale>

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    printf("═══════════════════════════════════\n");
    printf("   ПРОГРАММА ДЛЯ СОЗДАНИЯ ФАЙЛОВ   \n");
    printf("   ВСЕ ФАЙЛЫ СОХРАНЯЮТСЯ ЗДЕСЬ     \n");
	printf("   (в папке, вместе с file_h)      \n");
    printf("═══════════════════════════════════\n\n");
	 
    const size_t BUF_SIZE = 512;
    char BUFFER[BUF_SIZE];

	printf("\n");
    printf("┌─────────────────────────────┐\n");
    printf("│         ГЛАВНОЕ МЕНЮ        │\n");
    printf("├─────────────────────────────┤\n");
    printf("│ 1) Автомобильные данные     │\n");
    printf("│ 2) Регистрационные данные   │\n");
    printf("│ q) Выход                    │\n");
    printf("└─────────────────────────────┘\n");
    printf("Выбор: ");
    fflush(stdout);
    
    while (fgets(BUFFER, BUF_SIZE, stdin) != nullptr) {
        __fpurge(stdin);
        BUFFER[strcspn(BUFFER, "\n")] = 0;
        
        if (BUFFER[0] == 'q' || BUFFER[0] == 'Q') {
            break;
        }
        else if (BUFFER[0] == '1' || BUFFER[0] == '2') {
            printf("\n═══════════════════════════════════\n");
            printf("   %s\n", BUFFER[0] == '1' ? "АВТОМОБИЛЬНЫЕ ДАННЫЕ" : "РЕГИСТРАЦИОННЫЕ ДАННЫЕ");
            printf("═══════════════════════════════════\n");
            printf("* 'q' - возвращение в меню\n");
            fflush(stdout);
            
            write_data(BUFFER, BUF_SIZE, BUFFER[0] - '0');
            __fpurge(stdin);
            
            printf("\nНажмите Enter...");
            fflush(stdout);
            getchar();
            __fpurge(stdin);
        }
        else {
            printf("Ошибка: введите 1, 2 или q\n");
            fflush(stdout);
        }
        
		printf("\n");
        printf("┌─────────────────────────────┐\n");
        printf("│         ГЛАВНОЕ МЕНЮ        │\n");
        printf("├─────────────────────────────┤\n");
        printf("│ 1) Автомобильные данные     │\n");
        printf("│ 2) Регистрационные данные   │\n");
        printf("│ q) Выход                    │\n");
        printf("└─────────────────────────────┘\n");
        printf("Выбор: ");
        fflush(stdout);
    }
    
    printf("\nПрограмма завершена.\n");
    fflush(stdout);
    return 0;
}
