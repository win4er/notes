#include <cstdio>
#include <locale>
#include <regex>
#include <iostream>
#include <string>

int main() {
    setlocale(LC_ALL, "ru_RU.UTF8");
    std::regex onlyCyrillic(R"([ФфРрТтУуХхЦцЧчШшЩщЪъЬьЭэЮюЁёЫыА-Яа-я]+)");  // Только кириллица + пробелы/пунктуация
// или байты:
    std::regex onlyCyrillicBytes(R"([\xD0\x81\xD1\x91\xD0\x90-\xD0\xBF\xD1\x80-\xD1\x8F]*)");

    char input[256];
    while (scanf("%255s", input)!=EOF) {
        std::cout << std::regex_match(std::string(input), onlyCyrillic) << '\n';
    }
    return 0;
}
