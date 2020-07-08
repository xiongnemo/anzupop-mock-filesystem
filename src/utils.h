#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <ratio>
#include <cstring>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::time_t;
using namespace std::chrono;

#define CSI "\x1B["                // Control Sequence Introducer (ANSI spec name)
#define CLEAR CSI "0m"             // all attributes off
#define BRIGHT_BLACK_S CSI "0;90m" // somewhat MD.GRAY
#define BLACK_S CSI "0;30m"
#define BLACK_BOLD_S CSI "1;30m" // another name for GRAY
#define RED_S CSI "0;31m"
#define RED_BOLD_S CSI "1;31m"
#define GREEN_S CSI "0;32m"
#define GREEN_BOLD_S CSI "1;32m"
#define YELLOW_S CSI "0;33m"
#define YELLOW_BOLD_S CSI "1;33m"
#define BLUE_S CSI "0;34m"
#define BLUE_BOLD_S CSI "1;34m"
#define MAGENTA_S CSI "0;35m"
#define MAGENTA_BOLD_S CSI "1;35m"
#define CYAN_S CSI "0;36m"
#define CYAN_BOLD_S CSI "1;36m"
#define WHITE_S CSI "0;37m"      // another name for LT.GRAY
#define WHITE_BOLD_S CSI "1;37m" // actually white

#define GREEN_BG_BOLD_S CSI "42;1m"
#define YELLOW_BG_BOLD_S CSI "43;1m"
#define BLUE_BG_S CSI "44m"
#define BLUE_BG_BOLD_S CSI "44;1m"
#define MAGENTA_BG_S CSI "45m"
#define MAGENTA_BG_BOLD_S CSI "45;1m"
#define CYAN_BG_S CSI "46m"
#define CYAN_BG_BOLD_S CSI "46;1m"

//color wrappings
#define BLACK(x) BLACK_S x CLEAR
#define BLACK_BOLD(x) BLACK_BOLD_S x CLEAR
#define RED(x) RED_S x CLEAR
#define RED_BOLD(x) RED_BOLD_S x CLEAR
#define GREEN(x) GREEN_S x CLEAR
#define GREEN_BOLD(x) GREEN_BOLD_S x CLEAR
#define YELLOW(x) YELLOW_S x CLEAR
#define YELLOW_BOLD(x) YELLOW_BOLD_S x CLEAR
#define BLUE(x) BLUE_S x CLEAR
#define BLUE_BOLD(x) BLUE_BOLD_S x CLEAR
#define MAGENTA(x) MAGENTA_S x CLEAR
#define MAGENTA_BOLD(x) MAGENTA_BOLD_S x CLEAR
#define CYAN(x) CYAN_S x CLEAR
#define CYAN_BOLD(x) CYAN_BOLD_S x CLEAR
#define WHITE(x) WHITE_S x CLEAR
#define WHITE_BOLD(x) WHITE_BOLD_S x CLEAR

#define GREEN_BG_BOLD(x) GREEN_BG_BOLD_S x CLEAR
#define YELLOW_BG_BOLD(x) YELLOW_BG_BOLD_S x CLEAR
#define BLUE_BG(x) BLUE_BG_S x CLEAR
#define BLUE_BG_BOLD(x) BLUE_BG_BOLD_S x CLEAR
#define MAGENTA_BG(x) MAGENTA_BG_S x CLEAR
#define MAGENTA_BG_BOLD(x) MAGENTA_BG_BOLD_S x CLEAR
#define CYAN_BG(x) CYAN_BG_S x CLEAR
#define CYAN_BG_BOLD(x) CYAN_BG_BOLD_S x CLEAR

namespace Utils
{
    enum TerminalForegroundColor
    {
        Black_F = 30,
        Red_F = 31,
        Green_F = 32,
        Yellow_F = 33,
        Blue_F = 34,
        Magenta_F = 35,
        Cyan_F = 36,
        White_F = 37,
        BrightBlack_F = 90,
        BrightRed_F = 91,
        BrightGreen_F = 92,
        BrightYellow_F = 93,
        BrightBlue_F = 94,
        BrightMagenta_F = 95,
        BrightCyan_F = 96,
        BrightWhite_F = 97,
    };

    enum TerminalBackgroundColor
    {
        Black_B = 40,
        Red_B = 41,
        Green_B = 42,
        Yellow_B = 43,
        Blue_B = 44,
        Magenta_B = 45,
        Cyan_B = 46,
        White_B = 47,
        BrightBlack_B = 100,
        BrightRed_B = 101,
        BrightGreen_B = 102,
        BrightYellow_B = 103,
        BrightBlue_B = 104,
        BrightMagenta_B = 105,
        BrightCyan_B = 106,
        BrightWhite_B = 107,
    };

    namespace Logging
    {
        void info(const char *reporter, const char *message);
        void warning(const char *reporter, const char *message);
        void error(const char *reporter, const char *message);
        void fatal(const char *reporter, const char *message, int exit_code);
    }; // namespace Logging

    namespace Misc
    {
        class Gti
        {
            public:
                int gti(int argc, char **argv);
        };
        char *remove_last_char(char *char_array);
        char *get_current_time();
        void print_fake_header(const char *current_path);
        void print_fake_user();
        void print_fake_hostname();
        void print_with_foreground_color(const char *message, TerminalForegroundColor foreground_color);
        void print_with_background_color(const char *message, TerminalBackgroundColor background_color);
        void print_with_foreground_and_background_color(const char *message, TerminalForegroundColor foreground_color, TerminalBackgroundColor background_color);
        void clear_screen();
        void print_what();
        void print_logo();
    }; // namespace Misc

} // namespace Utils

#endif