#include "file_handler.hpp"

int main() {
    setlocale(LC_ALL, "rus_rus.1251");
    printf("���������� ���������: �������� ������ �� ������ ��������, �������� �������������\n");
    printf("��� ��������� ��������?\n1) ���� ������������� ��������\n2) ���� ��������������� ��������\n");
    
    char input='\0';
    while (scanf("%c", &input) != EOF) {
        printf("�������: %c\n", input);
        fseek(stdin, 0, SEEK_END);
        if (input == '1' || input == '2') {
            if (input == '1') write_car_data();
            else 
            if (input == '2') write_license_data();
            printf("������� ���������� ������? (enter=��/N=���)\n");
            input = getchar();
            fseek(stdin, 0, SEEK_END);
            if (input == 'N' || input == 'n') {
                break;
            } else
            if (input == '\n') {
                printf("��� ��������� ��������?\n1) ���� ������������� ��������\n2) ���� ��������������� ��������\n");
            } else printf("������ %c, ����� ������ ������ enter ��� N.\n", input);
        } else {
            printf("������ %c. ����� ������ ������ 1 ��� 2. ���������� ��� ���.\n", input);
            printf("��� ��������� ��������?\n1) ���� ������������� ��������\n2) ���� ��������������� ��������\n");
        }
    }
    printf("������� ����� ������� ��� ���������� ���������...\n");
    scanf("%c", &input);
    printf("��������� ���������\n");
    return 0;
}