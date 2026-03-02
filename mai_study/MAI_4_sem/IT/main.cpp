#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cctype>

namespace fs = std::filesystem;

class FileBrowser {
private:
    std::vector<fs::directory_entry> entries;
    std::string current_path;
    int selected_index;
    int scroll_offset;
    int terminal_height;
    int terminal_width;
    
    // Для просмотра файла
    bool viewing_file;
    std::vector<std::string> file_content;
    std::string current_file;
    int file_scroll_offset;      // Вертикальная прокрутка
    int file_horizontal_offset;   // Горизонтальная прокрутка
    bool show_line_numbers;
    
    // Для поиска
    bool search_mode;
    std::string search_query;
    std::vector<int> search_results; // Индексы строк с совпадениями
    int current_search_result;        // Текущий выбранный результат
    bool search_case_sensitive;

    // Включаем raw mode для терминала
    void enable_raw_mode() {
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    // Выключаем raw mode
    void disable_raw_mode() {
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag |= ICANON | ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    // Получаем размер терминала
    void get_terminal_size() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        terminal_height = w.ws_row;
        terminal_width = w.ws_col;
    }

    // Очищаем экран
    void clear_screen() {
        std::cout << "\033[2J\033[H";
    }

    // Читаем текущую директорию
    void read_directory() {
        entries.clear();
        try {
            for (const auto& entry : fs::directory_iterator(current_path)) {
                entries.push_back(entry);
            }
        } catch (const std::exception& e) {
            // Игнорируем ошибки доступа
        }
        
        // Сортируем: директории первые, потом файлы
        std::sort(entries.begin(), entries.end(), 
            [](const fs::directory_entry& a, const fs::directory_entry& b) {
                if (a.is_directory() != b.is_directory()) {
                    return a.is_directory() > b.is_directory();
                }
                return a.path().filename().string() < b.path().filename().string();
            });
    }

    // Читаем содержимое файла
    bool read_file_content(const std::string& filename) {
        file_content.clear();
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            return false;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            file_content.push_back(line);
        }
        
        return true;
    }

    // Выполнить поиск по текущему файлу
    void perform_search() {
        search_results.clear();
        current_search_result = -1;
        
        if (search_query.empty()) return;
        
        std::string query = search_query;
        if (!search_case_sensitive) {
            // Приводим к нижнему регистру для case-insensitive поиска
            std::transform(query.begin(), query.end(), query.begin(), 
                          [](unsigned char c){ return std::tolower(c); });
        }
        
        for (size_t i = 0; i < file_content.size(); ++i) {
            std::string line = file_content[i];
            if (!search_case_sensitive) {
                std::transform(line.begin(), line.end(), line.begin(),
                              [](unsigned char c){ return std::tolower(c); });
            }
            
            if (line.find(query) != std::string::npos) {
                search_results.push_back(i);
            }
        }
        
        // Если есть результаты, переходим к первому
        if (!search_results.empty()) {
            current_search_result = 0;
            jump_to_search_result(current_search_result);
        }
    }

    // Перейти к результату поиска
    void jump_to_search_result(int index) {
        if (index < 0 || index >= search_results.size()) return;
        
        int target_line = search_results[index];
        int max_display = terminal_height - 6; // Учитываем место для поисковой строки
        
        // Центрируем результат на экране
        file_scroll_offset = std::max(0, target_line - max_display / 2);
        
        // Сбрасываем горизонтальную прокрутку
        file_horizontal_offset = 0;
    }

    // Подсветить поисковый запрос в строке
    std::string highlight_search(const std::string& line, int line_number) {
        if (search_query.empty() || !is_search_result(line_number)) {
            return line;
        }
        
        std::string result;
        std::string query = search_query;
        std::string line_lower = line;
        
        if (!search_case_sensitive) {
            std::transform(query.begin(), query.end(), query.begin(),
                          [](unsigned char c){ return std::tolower(c); });
            std::transform(line_lower.begin(), line_lower.end(), line_lower.begin(),
                          [](unsigned char c){ return std::tolower(c); });
        }
        
        size_t pos = 0;
        size_t found;
        
        while ((found = line_lower.find(query, pos)) != std::string::npos) {
            // Добавляем текст до совпадения
            result += line.substr(pos, found - pos);
            // Добавляем подсвеченное совпадение
            result += "\033[7;31m" + line.substr(found, query.length()) + "\033[0m";
            pos = found + query.length();
        }
        
        // Добавляем оставшийся текст
        result += line.substr(pos);
        
        return result;
    }

    // Проверить, является ли строка результатом поиска
    bool is_search_result(int line_number) {
        return std::find(search_results.begin(), search_results.end(), line_number) 
               != search_results.end();
    }

    // Отображаем содержимое директории
    void display_directory() {
        clear_screen();
        get_terminal_size();
        
        // Заголовок с путем
        std::string header = " Текущая директория: " + current_path + " ";
        std::cout << "\033[44;37m" << header;
        for (int i = header.length(); i < terminal_width; ++i) {
            std::cout << " ";
        }
        std::cout << "\033[0m\n";
        
        // Инструкции
        std::cout << "\033[32m↑/↓: навигация | Enter: открыть | Backspace: назад | ";
        std::cout << "q: выход | Пробел: показать инфо\033[0m\n";
        
        // Содержимое
        int max_display = terminal_height - 4; // Оставляем место для заголовка и инструкций
        for (int i = 0; i < max_display && i + scroll_offset < entries.size(); ++i) {
            int idx = i + scroll_offset;
            const auto& entry = entries[idx];
            
            // Определяем цвет и символ
            std::string color;
            std::string prefix;
            
            if (entry.is_directory()) {
                color = "\033[34m"; // Синий для директорий
                prefix = "📁 ";
            } else if (entry.is_symlink()) {
                color = "\033[36m"; // Голубой для ссылок
                prefix = "🔗 ";
            } else {
                color = "\033[37m"; // Белый для файлов
                prefix = "📄 ";
            }
            
            // Выделяем выбранный элемент
            if (idx == selected_index) {
                color = "\033[7;33m"; // Инвертированный желтый для выделения
            }
            
            std::string name = entry.path().filename().string();
            if (name.empty()) name = "/";
            
            // Добавляем информацию о размере для файлов
            if (entry.is_regular_file()) {
                try {
                    auto size = entry.file_size();
                    if (size < 1024) {
                        name += " (" + std::to_string(size) + " B)";
                    } else if (size < 1024 * 1024) {
                        name += " (" + std::to_string(size / 1024) + " KB)";
                    } else {
                        name += " (" + std::to_string(size / (1024 * 1024)) + " MB)";
                    }
                } catch (...) {}
            }
            
            // Обрезаем длинные имена
            if (name.length() > terminal_width - 5) {
                name = name.substr(0, terminal_width - 8) + "...";
            }
            
            std::cout << color << prefix << name << "\033[0m\n";
        }
        
        std::cout.flush();
    }

    // Отображаем содержимое файла
    void display_file() {
        clear_screen();
        get_terminal_size();
        
        // Заголовок с именем файла
        std::string header = " Просмотр файла: " + current_file + " ";
        if (show_line_numbers) {
            header += "[Номера строк включены] ";
        }
        if (!search_results.empty()) {
            header += "[Найдено: " + std::to_string(search_results.size()) + "] ";
        }
        std::cout << "\033[44;37m" << header;
        for (int i = header.length(); i < terminal_width; ++i) {
            std::cout << " ";
        }
        std::cout << "\033[0m\n";
        
        // Инструкции
        std::cout << "\033[32m↑/↓: вверх/вниз | ←/→: влево/вправо | ";
        std::cout << "Ctrl+F: поиск | n/N: след./пред. результат | ";
        std::cout << "q: выход\033[0m\n";
        
        // Строка поиска, если активен режим поиска
        if (search_mode) {
            std::cout << "\033[43;30m Поиск: " << search_query;
            if (search_query.empty()) {
                std::cout << " (введите текст для поиска)";
            }
            // Мигающий курсор
            std::cout << " \033[5m_\033[0m";
            
            // Информация о регистре
            std::cout << " [";
            if (search_case_sensitive) {
                std::cout << "Чувствителен к регистру";
            } else {
                std::cout << "Не чувствителен к регистру";
            }
            std::cout << "] ";
            
            // Заполняем оставшееся место
            for (int i = 0; i < terminal_width - 20; ++i) {
                std::cout << " ";
            }
            std::cout << "\033[0m\n";
        }
        
        // Содержимое файла
        int max_display = terminal_height - (search_mode ? 6 : 5); // Учитываем строку поиска
        int line_num_width = show_line_numbers ? 7 : 0;
        int content_width = terminal_width - line_num_width;
        
        bool can_scroll_left = (file_horizontal_offset > 0);
        bool can_scroll_right = false;
        bool horizontal_scroll_indicator = false;
        
        for (int i = 0; i < max_display && i + file_scroll_offset < file_content.size(); ++i) {
            int line_idx = i + file_scroll_offset;
            std::string line = file_content[line_idx];
            
            // Номер строки
            if (show_line_numbers) {
                std::string line_num = std::to_string(line_idx + 1);
                // Подсвечиваем номер строки, если это результат поиска
                if (is_search_result(line_idx)) {
                    std::cout << "\033[7;33m" << std::string(line_num_width - line_num.length() - 1, ' ') 
                             << line_num << " \033[0m";
                } else {
                    std::cout << "\033[33m" << std::string(line_num_width - line_num.length() - 1, ' ') 
                             << line_num << " \033[0m";
                }
            }
            
            // Проверяем, можно ли прокрутить эту строку вправо
            if (!can_scroll_right && line.length() > file_horizontal_offset + content_width) {
                can_scroll_right = true;
            }
            
            // Применяем горизонтальную прокрутку
            if (file_horizontal_offset < line.length()) {
                line = line.substr(file_horizontal_offset);
            } else {
                line = "";
            }
            
            // Подсвечиваем поисковый запрос
            if (!search_query.empty()) {
                line = highlight_search(line, line_idx);
            }
            
            // Обрезаем по ширине терминала
            if (line.length() > content_width) {
                // Учитываем escape-последовательности при подсчете длины
                int visible_length = 0;
                bool in_escape = false;
                for (char c : line) {
                    if (c == '\033') in_escape = true;
                    else if (in_escape && c == 'm') in_escape = false;
                    else if (!in_escape) visible_length++;
                    
                    if (visible_length > content_width - 1) break;
                }
                
                if (visible_length > content_width - 1) {
                    line = line.substr(0, content_width - 1) + "→";
                }
                horizontal_scroll_indicator = true;
            }
            
            std::cout << line << "\n";
        }
        
        // Статусная строка
        std::string status_line = "";
        
        // Вертикальная позиция
        if (file_content.size() > 0) {
            int percent = (file_scroll_offset * 100) / file_content.size();
            status_line += "Строка " + std::to_string(file_scroll_offset + 1) + "-" 
                     + std::to_string(std::min(file_scroll_offset + max_display, (int)file_content.size())) 
                     + " из " + std::to_string(file_content.size()) + " (" + std::to_string(percent) + "%)";
        } else {
            status_line += "Файл пуст";
        }
        
        // Информация о поиске
        if (!search_results.empty()) {
            status_line += " | Результат " + std::to_string(current_search_result + 1) + 
                          " из " + std::to_string(search_results.size());
        }
        
        // Горизонтальная позиция
        if (file_horizontal_offset > 0 || can_scroll_right) {
            if (!status_line.empty()) status_line += " | ";
            
            size_t max_visible_line_length = 0;
            int start_line = file_scroll_offset;
            int end_line = std::min(file_scroll_offset + max_display, (int)file_content.size());
            
            for (int i = start_line; i < end_line; ++i) {
                max_visible_line_length = std::max(max_visible_line_length, file_content[i].length());
            }
            
            status_line += "Колонка " + std::to_string(file_horizontal_offset + 1);
            
            if (max_visible_line_length > content_width) {
                int total_columns = max_visible_line_length;
                int current_end = std::min(file_horizontal_offset + content_width, (int)max_visible_line_length);
                status_line += "-" + std::to_string(current_end) + 
                              " (всего " + std::to_string(total_columns) + ")";
            }
        }
        
        // Индикаторы возможности прокрутки
        std::string scroll_indicators = "";
        if (can_scroll_left) scroll_indicators += "←";
        if (can_scroll_right) scroll_indicators += "→";
        if (file_scroll_offset > 0) scroll_indicators += "↑";
        if (file_scroll_offset + max_display < file_content.size()) scroll_indicators += "↓";
        
        if (!scroll_indicators.empty()) {
            status_line += " [" + scroll_indicators + "]";
        }
        
        std::cout << "\033[32m" << status_line << "\033[0m";
        
        std::cout.flush();
    }

    // Обработка ввода в режиме поиска
    void handle_search_input(char c) {
        if (c == 27) { // ESC - выход из поиска
            search_mode = false;
            search_query.clear();
            search_results.clear();
            current_search_result = -1;
        }
        else if (c == 127 || c == 8) { // Backspace
            if (!search_query.empty()) {
                search_query.pop_back();
                perform_search();
            }
        }
        else if (c == '\n') { // Enter - выполнить поиск и выйти из режима
            search_mode = false;
        }
        else if (c == 6) { // Ctrl+F - переключить регистр (ASCII 6)
            search_case_sensitive = !search_case_sensitive;
            perform_search();
        }
        else if (c == 'n') { // Следующий результат
            if (!search_results.empty()) {
                current_search_result = (current_search_result + 1) % search_results.size();
                jump_to_search_result(current_search_result);
            }
        }
        else if (c == 'N' || c == 14) { // Предыдущий результат (N или Ctrl+N)
            if (!search_results.empty()) {
                current_search_result = (current_search_result - 1 + search_results.size()) % search_results.size();
                jump_to_search_result(current_search_result);
            }
        }
        else if (isprint(c)) { // Печатаемые символы
            search_query += c;
            perform_search();
        }
    }

    // Обработка ввода в режиме просмотра файла
    void handle_file_input() {
        char c;
        read(STDIN_FILENO, &c, 1);
        
        if (search_mode) {
            handle_search_input(c);
            return;
        }
        
        int max_display = terminal_height - 5;
        int line_num_width = show_line_numbers ? 7 : 0;
        int content_width = terminal_width - line_num_width;
        
        switch (c) {
            case 'q': // Выход из просмотра файла
                viewing_file = false;
                file_content.clear();
                search_results.clear();
                search_query.clear();
                search_mode = false;
                break;
            
            case 'n': // Переключить номера строк
                show_line_numbers = !show_line_numbers;
                break;
            
            case 6: // Ctrl+F - поиск (ASCII 6)
                search_mode = true;
                search_query.clear();
                search_results.clear();
                current_search_result = -1;
                break;
            
            case 'N': // Следующий результат (с Shift)
                if (!search_results.empty()) {
                    current_search_result = (current_search_result + 1) % search_results.size();
                    jump_to_search_result(current_search_result);
                }
                break;
            
            case 14: // Ctrl+N - предыдущий результат (ASCII 14)
                if (!search_results.empty()) {
                    current_search_result = (current_search_result - 1 + search_results.size()) % search_results.size();
                    jump_to_search_result(current_search_result);
                }
                break;
            
            case 'j':
            case 0x42: // Стрелка вниз
                if (file_scroll_offset + max_display < file_content.size()) {
                    file_scroll_offset++;
                }
                break;
            
            case 'k':
            case 0x41: // Стрелка вверх
                if (file_scroll_offset > 0) {
                    file_scroll_offset--;
                }
                break;
            
            case 'l':
            case 0x43: // Стрелка вправо
                {
                    int scroll_step = std::max(1, content_width / 8);
                    
                    bool can_scroll_right = false;
                    int start_line = file_scroll_offset;
                    int end_line = std::min(file_scroll_offset + max_display, (int)file_content.size());
                    
                    for (int i = start_line; i < end_line; ++i) {
                        if (file_content[i].length() > file_horizontal_offset + scroll_step) {
                            can_scroll_right = true;
                            break;
                        }
                    }
                    
                    if (can_scroll_right) {
                        file_horizontal_offset += scroll_step;
                    }
                }
                break;
            
            case 'h':
            case 0x44: // Стрелка влево
                {
                    int scroll_step = std::max(1, content_width / 8);
                    file_horizontal_offset = std::max(0, file_horizontal_offset - scroll_step);
                }
                break;
            
            case 'L': // Shift + l - быстрая прокрутка вправо
                {
                    int fast_scroll = std::max(5, content_width / 3);
                    
                    bool can_scroll_right = false;
                    int start_line = file_scroll_offset;
                    int end_line = std::min(file_scroll_offset + max_display, (int)file_content.size());
                    
                    for (int i = start_line; i < end_line; ++i) {
                        if (file_content[i].length() > file_horizontal_offset + fast_scroll) {
                            can_scroll_right = true;
                            break;
                        }
                    }
                    
                    if (can_scroll_right) {
                        file_horizontal_offset += fast_scroll;
                    }
                }
                break;
            
            case 'H': // Shift + h - быстрая прокрутка влево
                {
                    int fast_scroll = std::max(5, content_width / 3);
                    file_horizontal_offset = std::max(0, file_horizontal_offset - fast_scroll);
                }
                break;
            
            case '0': // В начало строки
                file_horizontal_offset = 0;
                break;
            
            case '$': // В конец строки
                {
                    size_t max_line_length = 0;
                    int start_line = file_scroll_offset;
                    int end_line = std::min(file_scroll_offset + max_display, (int)file_content.size());
                    
                    for (int i = start_line; i < end_line; ++i) {
                        max_line_length = std::max(max_line_length, file_content[i].length());
                    }
                    
                    if (max_line_length > content_width) {
                        int new_offset = max_line_length - content_width + 3;
                        file_horizontal_offset = std::max(0, new_offset);
                    }
                }
                break;
            
            case 'g': // В начало файла
                file_scroll_offset = 0;
                file_horizontal_offset = 0;
                break;
            
            case 'G': // В конец файла
                if (file_content.size() > max_display) {
                    file_scroll_offset = file_content.size() - max_display;
                } else {
                    file_scroll_offset = 0;
                }
                break;
            
            case 0x35: // PageUp
                file_scroll_offset = std::max(0, file_scroll_offset - max_display);
                break;
            
            case 0x36: // PageDown
                file_scroll_offset = std::min((int)file_content.size() - max_display, 
                                              file_scroll_offset + max_display);
                if (file_scroll_offset < 0) file_scroll_offset = 0;
                break;
            
            case 8: // Ctrl+H - Home (ASCII 8)
            case 127: // Также Backspace, но мы уже обработали его отдельно? 
                // Backspace обрабатывается в search_mode, а здесь игнорируем
                break;
            
            default:
                // Игнорируем все остальные клавиши
                break;
        }
    }

    // Обработка ввода в режиме просмотра директории
    void handle_directory_input() {
        char c;
        read(STDIN_FILENO, &c, 1);
        
        switch (c) {
            case 'q':
                throw 0; // Выход
            
            case 'j':
            case 0x42: // Стрелка вниз
                if (selected_index < entries.size() - 1) {
                    selected_index++;
                    if (selected_index >= scroll_offset + terminal_height - 4) {
                        scroll_offset++;
                    }
                }
                break;
            
            case 'k':
            case 0x41: // Стрелка вверх
                if (selected_index > 0) {
                    selected_index--;
                    if (selected_index < scroll_offset) {
                        scroll_offset--;
                    }
                }
                break;
            
            case ' ': // Пробел - показать информацию
                if (!entries.empty()) {
                    show_file_info();
                }
                break;
            
            case '\n': // Enter
                if (!entries.empty()) {
                    const auto& entry = entries[selected_index];
                    if (entry.is_directory()) {
                        current_path = entry.path().string();
                        selected_index = 0;
                        scroll_offset = 0;
                        read_directory();
                    } else {
                        // Пытаемся прочитать файл
                        current_file = entry.path().string();
                        if (read_file_content(current_file)) {
                            viewing_file = true;
                            file_scroll_offset = 0;
                            file_horizontal_offset = 0;
                            show_line_numbers = true;
                            search_mode = false;
                            search_query.clear();
                            search_results.clear();
                            current_search_result = -1;
                            search_case_sensitive = false; // По умолчанию без учета регистра
                        } else {
                            // Если не удалось прочитать, показываем информацию
                            show_file_info();
                        }
                    }
                }
                break;
            
            case 0x7F: // Backspace
                {
                    fs::path path(current_path);
                    if (path.has_parent_path()) {
                        current_path = path.parent_path().string();
                        selected_index = 0;
                        scroll_offset = 0;
                        read_directory();
                    }
                }
                break;
        }
    }

    // Показать информацию о файле
    void show_file_info() {
        const auto& entry = entries[selected_index];
        clear_screen();
        
        std::cout << "\033[44;37m Информация о файле \033[0m\n\n";
        std::cout << "\033[33mИмя: \033[0m" << entry.path().filename().string() << "\n";
        std::cout << "\033[33mПолный путь: \033[0m" << entry.path().string() << "\n";
        
        if (entry.is_regular_file()) {
            try {
                auto size = entry.file_size();
                std::cout << "\033[33mРазмер: \033[0m" << size << " байт";
                if (size < 1024) {
                    std::cout << " (" << size << " B)";
                } else if (size < 1024 * 1024) {
                    std::cout << " (" << size / 1024 << " KB)";
                } else {
                    std::cout << " (" << size / (1024 * 1024) << " MB)";
                }
                std::cout << "\n";
            } catch (...) {}
        }
        
        std::cout << "\033[33mТип: \033[0m";
        if (entry.is_directory()) std::cout << "Директория";
        else if (entry.is_symlink()) std::cout << "Символическая ссылка";
        else if (entry.is_regular_file()) std::cout << "Обычный файл";
        else if (entry.is_block_file()) std::cout << "Блочное устройство";
        else if (entry.is_character_file()) std::cout << "Символьное устройство";
        else if (entry.is_fifo()) std::cout << "FIFO";
        else if (entry.is_socket()) std::cout << "Сокет";
        else std::cout << "Неизвестно";
        std::cout << "\n\n";
        
        // Для текстовых файлов показываем количество строк
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::string line;
                int line_count = 0;
                while (std::getline(file, line)) {
                    line_count++;
                }
                std::cout << "\033[33mКоличество строк: \033[0m" << line_count << "\n";
                
                // Показываем максимальную длину строки
                file.clear();
                file.seekg(0);
                size_t max_line_length = 0;
                while (std::getline(file, line)) {
                    max_line_length = std::max(max_line_length, line.length());
                }
                std::cout << "\033[33mМакс. длина строки: \033[0m" << max_line_length << " символов\n\n";
            }
        }
        
        std::cout << "\033[32mНажмите любую клавишу для продолжения...\033[0m";
        std::cout.flush();
        
        char temp;
        read(STDIN_FILENO, &temp, 1);
    }

public:
    FileBrowser(const std::string& path = ".") {
        current_path = fs::absolute(path).string();
        selected_index = 0;
        scroll_offset = 0;
        viewing_file = false;
        file_scroll_offset = 0;
        file_horizontal_offset = 0;
        show_line_numbers = true;
        search_mode = false;
        search_case_sensitive = false;
        current_search_result = -1;
        read_directory();
    }

    void run() {
        try {
            enable_raw_mode();
            
            while (true) {
                if (viewing_file) {
                    display_file();
                    handle_file_input();
                } else {
                    display_directory();
                    handle_directory_input();
                }
            }
        } catch (int) {
            // Выход по 'q'
        }
        
        disable_raw_mode();
        clear_screen();
        std::cout << "\033[32mДо свидания!\033[0m\n";
    }
};

int main(int argc, char* argv[]) {
    std::string path = ".";
    if (argc > 1) {
        path = argv[1];
    }
    
    try {
        FileBrowser browser(path);
        browser.run();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
