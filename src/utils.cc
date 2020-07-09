#include "utils.h"

void Utils::Logging::info(const char *reporter, const char *message)
{
#ifdef INFO
    cout << "[INFO] [" << Misc::get_current_time() << "] " << reporter << ": " << message << endl;
#endif
}

void Utils::Logging::warning(const char *reporter, const char *message)
{
    cout << "\033[1;33m"
         << "[WARNING]"
         << "\033[0m"
         << " [" << Misc::get_current_time() << "] " << reporter << ": " << message << endl;
}

void Utils::Logging::error(const char *reporter, const char *message)
{
    cout << "\033[1;31m"
         << "[ERROR]"
         << "\033[0m"
         << " [" << Misc::get_current_time() << "] " << reporter << ": " << message << endl;
}

void Utils::Logging::fatal(const char *reporter, const char *message, int exit_code)
{
    cout << "\033[1;91m"
         << "[FATAL]"
         << "\033[0m"
         << " [" << Misc::get_current_time() << "] " << reporter << ": " << message << endl;
    exit(exit_code);
}

char *Utils::Misc::remove_last_char(char *char_array)
{
    int length = strlen(char_array);
    char_array[length - 1] = '\0';
    return char_array;
}

char *Utils::Misc::get_current_time()
{
    auto current_time_ = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current_time_);
    char *time_ = std::ctime(&current_time);
    char *time = remove_last_char(time_);
    return time;
}

void Utils::Misc::print_fake_header(const char *current_path)
{
    print_with_foreground_color("nemo@marshmallow-laptop", TerminalForegroundColor::BrightGreen_F);
    cout << ":";
    print_with_foreground_color(current_path, TerminalForegroundColor::BrightBlue_F);
    cout << "$ ";
}

void Utils::Misc::print_fake_user()
{
    cout << "nemo" << endl;
}

void Utils::Misc::print_fake_hostname()
{
    cout << "marshmallow-laptop" << endl;
}

void Utils::Misc::print_with_foreground_color(const char *message, TerminalForegroundColor foreground_color)
{
    cout << "\033[" << foreground_color << "m" << message << "\033[0m";
    fflush(stdout);
}

void Utils::Misc::print_with_background_color(const char *message, TerminalBackgroundColor background_color)
{
    cout << "\033[" << background_color << "m" << message << "\033[0m";
}

void Utils::Misc::print_with_foreground_and_background_color(const char *message, TerminalForegroundColor foreground_color, TerminalBackgroundColor background_color)
{
    cout << "\033[" << foreground_color << ";" << background_color << "m" << message << "\033[0m";
}

void Utils::Misc::clear_screen()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\033c");
    fflush(stdout);
}

void Utils::Misc::print_logo()
{
    cout << "   _____                                            \n"
            "  /  _  \\   ____ __________ ________   ____ ______  \n"
            " /  /_\\  \\ /    \\\\___   /  |  \\____ \\ /  _ \\____  \\ \n"
            "/    |    \\   |  \\/    /|  |  /  |_> >  <_> )  |_> >\n"
            "\\____|__  /___|  /_____ \\____/|   __/ \\____/|   __/ \n"
            "        \\/     \\/      \\/     |__|          |__|    \n";
}

void Utils::Misc::print_what()
{
    cout << "          _____                _____                    _____                    _____          \n"
            "         /\\    \\              /\\    \\                  /\\    \\                  /\\    \\         \n"
            "        /::\\    \\            /::\\    \\                /::\\    \\                /::\\    \\        \n"
            "       /::::\\    \\           \\:::\\    \\              /::::\\    \\               \\:::\\    \\       \n"
            "      /::::::\\    \\           \\:::\\    \\            /::::::\\    \\               \\:::\\    \\      \n"
            "     /:::/\\:::\\    \\           \\:::\\    \\          /:::/\\:::\\    \\               \\:::\\    \\     \n"
            "    /:::/__\\:::\\    \\           \\:::\\    \\        /:::/__\\:::\\    \\               \\:::\\    \\    \n"
            "   /::::\\   \\:::\\    \\          /::::\\    \\      /::::\\   \\:::\\    \\              /::::\\    \\   \n"
            "  /::::::\\   \\:::\\    \\        /::::::\\    \\    /::::::\\   \\:::\\    \\    ____    /::::::\\    \\  \n"
            " /:::/\\:::\\   \\:::\\    \\      /:::/\\:::\\    \\  /:::/\\:::\\   \\:::\\____\\  /\\   \\  /:::/\\:::\\    \\ \n"
            "/:::/  \\:::\\   \\:::\\____\\    /:::/  \\:::\\____\\/:::/  \\:::\\   \\:::|    |/::\\   \\/:::/  \\:::\\____\\\n"
            "\\::/    \\:::\\  /:::/    /   /:::/    \\::/    /\\::/   |::::\\  /:::|____|\\:::\\  /:::/    \\::/    /\n"
            " \\/____/ \\:::\\/:::/    /   /:::/    / \\/____/  \\/____|:::::\\/:::/    /  \\:::\\/:::/    / \\/____/ \n"
            "          \\::::::/    /   /:::/    /                 |:::::::::/    /    \\::::::/    /          \n"
            "           \\::::/    /   /:::/    /                  |::|\\::::/    /      \\::::/____/           \n"
            "           /:::/    /    \\::/    /                   |::| \\::/____/        \\:::\\    \\           \n"
            "          /:::/    /      \\/____/                    |::|  ~|               \\:::\\    \\          \n"
            "         /:::/    /                                  |::|   |                \\:::\\    \\         \n"
            "        /:::/    /                                   \\::|   |                 \\:::\\____\\        \n"
            "        \\::/    /                                     \\:|   |                  \\::/    /        \n"
            "         \\/____/                                       \\|___|                   \\/____/         \n";
}

string Utils::Misc::cut_string_to_length(string source, unsigned int length, const char *caller_name, const char *source_string_name)
{
    string result;
    if (source.length() > length)
    {
        result = source.substr(0, length);
        string message = "The length of " + string(source_string_name) + " is larger than " + to_string(length) + ". Cut off.";
        Logging::warning(caller_name, message.c_str());
    }
    else
    {
        result = source;
    }
    return result;
}
