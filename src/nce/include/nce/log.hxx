#pragma once

#include <iostream>
#include <cstdio>

#define LOGWARN(msg) log_warning((msg), __FILE__, __func__, __LINE__)
#define LOGERROR(msg) log_error((msg), __FILE__, __func__, __LINE__)
#define LOGINFO(msg) log_info((msg))
#define LOGHEADER(msg) log_info((msg))


namespace termcolor {
constexpr CString reset = "\x1B[0m";
constexpr CString bold = "\x1B[1m";
constexpr CString black = "\x1B[30m";
constexpr CString black_bg = "\x1B[40m";
constexpr CString red = "\x1B[31m";
constexpr CString red_bg = "\x1B[41m";
constexpr CString green = "\x1B[32m";
constexpr CString green_bg = "\x1B[42m";
constexpr CString yellow = "\x1B[33m";
constexpr CString yellow_bg = "\x1B[43m";
constexpr CString blue = "\x1B[34m";
constexpr CString blue_bg = "\x1B[44m";
constexpr CString magenta = "\x1B[35m";
constexpr CString magenta_bg = "\x1B[47m";
constexpr CString cyan = "\x1B[36m";
constexpr CString cyan_bg = "\x1B[46m";
constexpr CString white = "\x1B[37m";
constexpr CString white_bg = "\x1B[47m";
constexpr CString bright_black = "\x1B[90m";
constexpr CString bright_black_bg = "\x1B[100m";
constexpr CString bright_red = "\x1B[91m";
constexpr CString bright_red_bg = "\x1B[101m";
constexpr CString bright_green = "\x1B[92m";
constexpr CString bright_green_bg = "\x1B[102m";
constexpr CString bright_yellow = "\x1B[93m";
constexpr CString bright_yellow_bg = "\x1B[103m";
constexpr CString bright_blue = "\x1B[94m";
constexpr CString bright_blue_bg = "\x1B[104m";
constexpr CString bright_magenta = "\x1B[95m";
constexpr CString bright_magenta_bg = "\x1B[105m";
constexpr CString bright_cyan = "\x1B[96m";
constexpr CString bright_cyan_bg = "\x1B[106m";
constexpr CString bright_white = "\x1B[97m";
constexpr CString bright_white_bg = "\x1B[107m";
}
template<typename T>
void log_warning(T msg, CString file, CString function, size_t line) {
    std::cout << termcolor::bright_white << file << " (" << function << ":" << line << ")" << termcolor::bold << termcolor::magenta << " Warning: " << termcolor::reset << msg << std::endl;
}
template<typename T>
void log_error(T msg, CString file, CString function, size_t line) {
    std::cout << termcolor::bright_white << file << " (" << function << ":" << line << ")" << termcolor::bold << termcolor::red << " Error: " << termcolor::reset << msg << std::endl;
}
template<typename T>
void log_info(T msg) {
    std::cout << termcolor::bright_white << termcolor::bold << "Info: " << termcolor::reset << msg << std::endl;
}
