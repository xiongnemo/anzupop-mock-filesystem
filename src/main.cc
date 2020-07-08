#include "test.h"
#include "fs.h"
#include "utils.h"

using std::cin;

using namespace MarshmallowFS;

int main()
{
    Logging::info("main", "main started.");
    FS test_fs;
    cout << "Input image path: ";
    cin >> test_fs.file_name;
    if (test_fs.is_image_exist())
    {
        if (!test_fs.is_image_vaild())
        {
            Logging::fatal("main", "given file is not vaild", 1);
        }
        else
        {
            test_fs.load_image();
        }
        
    }
    else
    {
        test_fs.init_image();
    }
    getchar();
    Logging::info("main", "Spawning shell...");
    fflush(stdout);
    printf("Welcome to\n");
    Misc::print_logo();
    printf("mock filesystem!\n");
    test_fs.shell();
    Logging::info("main", "main exits.");
    test_fs.save_image();
    return 0;
}