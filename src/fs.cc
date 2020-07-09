#include "fs.h"

void MarshmallowFS::FS::shell()
{
    bool last_operation_result = true;
    while (1)
    {
#ifdef DEBUG
        cout << "[DEBUG] atri: preparing to print new line of header" << endl;
#endif
        if (!last_operation_result)
        {
            cout << RED_BOLD(" X ");
        }
        last_operation_result = true;
        Misc::print_fake_header(pwd_());
#ifdef DEBUG
        cout << "[DEBUG] atri: new line of header printed" << endl;
#endif
        string operation_line;
        vector<string> operations;
        std::getline(std::cin, operation_line);
        istringstream ss(operation_line);
        do
        {
            // Read a word
            string word;
            ss >> word;
            // Print the read word
            operations.push_back(word);
            // While there is more to read
        } while (ss);
        operations.pop_back();
        string operation = operations[0];
        int argument_count = operations.size();
#ifdef DEBUG
        cout << "[DEBUG] Argument count: " << argument_count << endl;
#endif
        if (argument_count == 0)
        {
            continue;
        }
        if (operation == "clear")
        {
            Misc::clear_screen();
        }
        else if (operation == "exit")
        {
            break;
        }
        else if (operation == "mkfs.MFS" or operation == "format")
        {
            last_operation_result = format();
            continue;
        }
        else if (operation == "touch")
        {
            if (argument_count == 1)
            {
                cout << "touch: missing file operand" << endl;
                last_operation_result = false;
            }
            else
            {
                for (int i = 1; i < argument_count; i++)
                {
                    touch(operations[i]);
                }
                last_operation_result = true;
            }
        }
        else if (operation == "mkdir")
        {
            if (argument_count == 1)
            {
                cout << "mkdir: missing operand" << endl;
                last_operation_result = false;
            }
            else
            {
                last_operation_result = true;
                bool temp = true;
                for (int i = 1; i < argument_count; i++)
                {
                    if (!mkdir(operations[i]))
                        temp = false;
                }
                last_operation_result = temp;
            }
        }
        else if (operation == "rm")
        {
            if (argument_count == 1)
            {
                cout << "rm: missing operand" << endl;
                last_operation_result = false;
            }
            else
            {
                last_operation_result = true;
                bool temp = true;
                for (int i = 1; i < argument_count; i++)
                {
                    if (!rm(operations[i]))
                        temp = false;
                }
                last_operation_result = temp;
            }
        }
        else if (operation == "help" || operation == "?")
        {
            help();
        }
        else if (operation == "pwd")
        {
            pwd();
        }
        else if (operation == "date")
        {
            printf("%s\n", Utils::Misc::get_current_time());
        }
        else if (operation == "ls")
        {
            ls();
        }
        else if (operation == "cd")
        {
            if (argument_count == 1)
            {
                // do nothing, though in real life it go into ~
            }
            else if (argument_count == 2)
            {
                last_operation_result = cd(operations[1]);
                continue;
            }
            else
            {
                cout << "cd:  unrecognized count of arguments." << endl;
            }
        }
        else if (operation == "find")
        {
            if (argument_count == 2)
            {
                last_operation_result = find(operations[1]);
            }
            else
            {
                cout << "find:  unrecognized count of arguments." << endl;
            }
        }
        else if (operation == "info")
        {
            info();
        }
        else if (operation == "sync" || operation == "save")
        {
            save_image();
        }
        else if (operation == "whoami")
        {
            Misc::print_fake_user();
        }
        else if (operation == "hostname")
        {
            Misc::print_fake_hostname();
        }
        else if (operation == "what")
        {
            Misc::print_what();
        }
        else if (operation == "cat")
        {
            if (argument_count == 2)
            {
                last_operation_result = cat(operations[1]);
            }
            else
            {
                last_operation_result = false;
                Logging::error("atri", "Unsupported argument_count for command cat.");
            }
        }
        else if (operation == "echo")
        {
            // echo 1 2 3 > test.txt
            // 0    1 2 3 4 5
            if (argument_count >= 4 && ((operations[argument_count - 2] == ">") || (operations[argument_count - 2] == ">>")))
            {
                string buffer = "";
                for (int i = 1; i < argument_count - 2; i++)
                {
                    buffer += operations[i];
                    if (i + 1 < argument_count - 2)
                        buffer += " ";
                }
                last_operation_result = write(buffer, operations[argument_count - 1], (operations[argument_count - 2] == ">") ? false : true);
            }
            else
            {
                for (int i = 1; i < argument_count; i++)
                {
                    cout << operations[i] << " ";
                }
                cout << endl;
            }
        }
        else
        {
            last_operation_result = false;
            string message = "command not found: ";
            message += operation;
            Logging::error("atri", message.c_str());
        }
    }
}

bool MarshmallowFS::FS::is_image_exist()
{
    const char *file_path = file_name.c_str();
    FILE *test;
    if ((test = fopen(file_path, "r+b")) == NULL) // read-only, binary
    {
        Logging::warning("is_image_exist", "given image name not exists.");
        return false;
    }
    Logging::info("is_image_exist", "given image exists.");
    fclose(test);
    return true;
}

bool MarshmallowFS::FS::is_image_vaild()
{
    const char *file_path = file_name.c_str();
    FILE *test;
    test = fopen(file_path, "r+b"); //read-only, binary
    char image_sig[14];
    image_sig[13] = '\0';
    fread(image_sig, sizeof(char), IMAGE_SIG_SIZE, test);
    if (strcmp(image_sig, IMAGE_SIG) != 0)
    {
        Logging::error("is_image_vaild", "image signature check failed.");
        return false;
    }
    Logging::info("is_image_vaild", "image signature check passed.");
    rewind(test);
    fseek(test, 0L, SEEK_END);
    long sz = ftell(test);
    if (sz != IMAGE_SIZE)
    {
        Logging::error("is_image_vaild", "image size check failed.");
        return false;
    }
    Logging::info("is_image_vaild", "image size check passed.");
    Logging::info("is_image_vaild", "image is (maybe) vaild.");
    fclose(test);
    return true;
}

bool MarshmallowFS::FS::format()
{
    cout << "Are you sure you want to format/re-init this disk image?" << endl;
    cout << "ALL DATA will be LOST and UNRECOVERABLE!" << endl;
    cout << "If you are sure, type \"yes\": ";
    string input;
    cin >> input;
    getchar();
    if (input == "yes")
    {
        init_image();
        return true;
    }
    return false;
}

void MarshmallowFS::FS::init_image()
{
    const char *file_path = file_name.c_str();
    FILE *out_image = fopen(file_path, "w+b");
    memset(image_contents, 0, sizeof(uint8_t) * IMAGE_SIZE);
    fwrite(image_contents, sizeof(uint8_t), IMAGE_SIZE, out_image);
    rewind(out_image);
    // sig
    fwrite(IMAGE_SIG, sizeof(char), IMAGE_SIG_SIZE, out_image);
    fwrite("   ", sizeof(char), 16 - IMAGE_SIG_SIZE, out_image);
    // label
    fwrite("MarshmallowDisk", sizeof(char), 16, out_image);
    // block allocation table for block #1: superblock
    fwrite("\x05\x04", sizeof(uint8_t), 2, out_image);
    freopen(file_path, "r+b", out_image);
    fread(image_contents, sizeof(uint8_t), IMAGE_SIZE, out_image);
    // load disk
    disk = (VituralDisk *)image_contents;
    // root directory
    DirectoryNode *root = (DirectoryNode *)(&(disk->BLOCK[0]));
    root->type = ItemType::root_directory;
    root->name[0] = '/';
    root->child_node_count = 0;
    // back to root
    current_block_pos_in_image = 1;
    freopen(file_path, "w+b", out_image);
    fwrite(image_contents, sizeof(uint8_t), IMAGE_SIZE, out_image);
    fclose(out_image);
    Logging::info("init_image", "image inited and saved to disk.");
}

void MarshmallowFS::FS::load_image()
{
    const char *file_path = file_name.c_str();
    FILE *in_image;
    in_image = fopen(file_path, "r+b"); // read-only, binary
    fread(image_contents, sizeof(uint8_t), IMAGE_SIZE, in_image);
    disk = (VituralDisk *)image_contents;
    Logging::info("load_image", "image loaded from disk.");
    fclose(in_image);
}

void MarshmallowFS::FS::save_image()
{
    const char *file_path = file_name.c_str();
    FILE *out_image = fopen(file_path, "w+b");
    fwrite(image_contents, sizeof(uint8_t), IMAGE_SIZE, out_image);
    Logging::info("save_image", "image saved to disk.");
    fclose(out_image);
}

void MarshmallowFS::FS::info()
{
    char label[17] = "1234567890123456";
    for (int i = 0; i < 16; i++)
    {
        label[i] = disk->superblock.label[i];
    }
    printf("Disk Label: %s\n", label);
    int empty_count = 0;
    int directory_count = 0;
    int file_count = 0;
    int file_data_block_count = 0;
    int superblock_count = 0;
    int root_directory_count = 0;
    for (int i = 0; i < 2048; i++)
    {
        switch (disk->superblock.block_allocation_table[i])
        {
        case ItemType::general_file:
            file_count++;
            break;
        case ItemType::not_a_node:
            empty_count++;
            break;
        case ItemType::general_file_data_block:
            file_data_block_count++;
            break;
        case ItemType::normal_directory:
            directory_count++;
            break;
        case ItemType::root_directory:
            root_directory_count++;
            break;
        case ItemType::superblock:
            superblock_count++;
            break;
        }
    }
    printf("Block allocation info: \n");
    printf("Empty: %d\n", empty_count);
    printf("Root Directory: %d\n", root_directory_count);
    printf("Normal Directory: %d\n", directory_count);
    printf("File: %d\n", file_count);
    printf("File Data: %d\n", file_data_block_count);
    printf("Superblock: %d\n", superblock_count);
    printf("*command 'rm' only deletes entry.*\n");
}

void MarshmallowFS::FS::pwd()
{
    printf("%s\n", pwd_());
}

const char *MarshmallowFS::FS::pwd_()
{
    GenericBlock *current_block = (GenericBlock *)(&(disk->BLOCK[current_block_pos_in_image - 1]));
    string result = "";
    DirectoryNode *current_directory;
    switch (current_block->type)
    {
    case ItemType::general_file:
        Logging::error("pwd_", "You ran into a file.");
        break;
    case ItemType::not_a_node:
        Logging::error("pwd_", "Where are you? It's even not a node.");
        break;
    case ItemType::general_file_data_block:
        Logging::error("pwd_", "You ran into a file data block.");
        break;
    case ItemType::normal_directory:
        current_directory = get_current_directory();
        while (current_directory->type != ItemType::root_directory)
        {
            const char *current_directory_name = (const char *)current_directory->name;
            string temp(current_directory_name);
#ifdef DEBUG
            printf("[DEBUG] pwd_: visiting %s\n", current_directory_name);
#endif
#ifdef DEBUG
            cout << "[DEBUG] pwd_: visiting (string ver.) " << temp << endl;
#endif
            temp = "/" + temp;
            result = temp + result;
#ifdef DEBUG
            cout << "[DEBUG] pwd_: current result: " << result << endl;
#endif
            current_directory = get_directory_at_block_pos(current_directory->father_directory_node_block_index);
        }
        strcpy(pwd_buffer, result.c_str());
        return pwd_buffer;
    case ItemType::root_directory:
        return "/";
    default:
        Logging::error("pwd_", "wtf where are you??");
        break;
    }
    return "where are you??";
}

void MarshmallowFS::FS::touch(string file_name)
{
    DirectoryNode *current_directory = get_current_directory();
    string file_name_to_use = Misc::cut_string_to_length(file_name, ITEM_NAME_SIZE - 1, "touch", "file_name");
    if (check_duplicate_item(file_name_to_use))
    {
        string message = "File ‘" + file_name_to_use + "’ exists.";
        Logging::info("touch", message.c_str());
        return;
    }
    int block_pos_to_use = -1;
    for (int index = 0; index < 2048; index++)
    {
        if (disk->superblock.block_allocation_table[index] == ItemType::not_a_node)
        {
            block_pos_to_use = index;
            break;
        }
    }
    current_directory->child_node_block_index[current_directory->child_node_count] = block_pos_to_use;
    (current_directory->child_node_count)++;
    disk->superblock.block_allocation_table[block_pos_to_use] = (uint8_t)ItemType::general_file;
    GenericBlock *current_block = get_raw_block_at_block_pos(block_pos_to_use);
    clean(current_block);
    File *current_file = get_file_at_block_pos(block_pos_to_use);
    initialize_a_file(current_file, file_name_to_use);
}

void MarshmallowFS::FS::initialize_a_file(File *file_to_initialize, string file_name)
{
    file_to_initialize->type = ItemType::general_file;
    file_to_initialize->father_directory_node_block_index = current_block_pos_in_image;
    file_to_initialize->data_block_end_sig = DATA_END_SIG;
    file_to_initialize->size = 0;
    int file_name_length = file_name.length();
    for (int i = 0; i < file_name_length; i++)
    {
        file_to_initialize->name[i] = file_name[i];
    }
}

bool MarshmallowFS::FS::mkdir(string directory_name)
{
    DirectoryNode *current_directory = get_current_directory();
    string directory_name_to_use = Misc::cut_string_to_length(directory_name, ITEM_NAME_SIZE - 1, "mkdir", "directory_name");
    if (check_duplicate_item(directory_name_to_use))
    {
        cout << "mkdir: cannot create directory ‘" << directory_name_to_use << "’: File exists" << endl;
        return false;
    }
    int block_pos_to_use = -1;
    for (int index = 0; index < 2048; index++)
    {
        if (disk->superblock.block_allocation_table[index] == ItemType::not_a_node)
        {
            block_pos_to_use = index;
            break;
        }
    }
    current_directory->child_node_block_index[current_directory->child_node_count] = block_pos_to_use;
    (current_directory->child_node_count)++;
    disk->superblock.block_allocation_table[block_pos_to_use] = (uint8_t)ItemType::normal_directory;
    GenericBlock *current_block = get_raw_block_at_block_pos(block_pos_to_use);
    clean(current_block);
    DirectoryNode *new_directory = get_directory_at_block_pos(block_pos_to_use);
    new_directory->type = ItemType::normal_directory;
    new_directory->father_directory_node_block_index = current_block_pos_in_image;
    new_directory->child_node_count = 0;
    int directory_name_length = directory_name_to_use.length();
    for (int i = 0; i < directory_name_length; i++)
    {
        new_directory->name[i] = directory_name_to_use[i];
    }
    return true;
}

bool MarshmallowFS::FS::cd(string directory_name)
{
    DirectoryNode *current_directory = get_current_directory();

    if (directory_name == "../" || directory_name == "..")
    {
        if (current_directory->type != ItemType::root_directory)
        {
            current_block_pos_in_image = current_directory->father_directory_node_block_index;
        }
        return true;
    }
    else if (directory_name == ".")
    {
        return true;
    }

    string directory_name_to_use = Misc::cut_string_to_length(directory_name, ITEM_NAME_SIZE - 1, "cd", "directory_name");
    uint32_t target_directory_block_pos = find_block_pos_(directory_name_to_use);
    if (target_directory_block_pos != 1919810)
    {
        GenericBlock *current_block = get_raw_block_at_block_pos(target_directory_block_pos);
        if ((current_block->type != ItemType::normal_directory) && (current_block->type != ItemType::root_directory))
        {
            cout << "cd: not a directory: " << directory_name_to_use << endl;
            return false;
        }
        current_block_pos_in_image = target_directory_block_pos;
#ifdef DEBUG
        cout << "[DEBUG] cd: entered directory " << directory_name_to_use << " with block position " << current_block_pos_in_image << "(" << current_block_pos_in_image - 1 << ")" << endl;
#endif
        return true;
    }
    else
    {
        cout << "cd: no such file or directory: " << directory_name_to_use << endl;
    }
    return false;
}

bool MarshmallowFS::FS::rm(string item_name)
{
    DirectoryNode *current_directory = get_current_directory();
    string item_name_to_use = Misc::cut_string_to_length(item_name, ITEM_NAME_SIZE - 1, "rm", "item_name");
    int node_size = current_directory->child_node_count;
    for (int i = 0; i < node_size; i++)
    {
        if (current_directory->child_node_block_index[i] == DELETED_FILE_BLOCK_INDEX)
        {
            continue;
        }
        GenericBlock *current_block = get_raw_block_at_block_pos(current_directory->child_node_block_index[i]);
        string temp_name = (const char *)current_block->name;
        if (temp_name == item_name_to_use)
        {
            disk->superblock.block_allocation_table[current_directory->child_node_block_index[i]] = (uint8_t)ItemType::not_a_node;
            current_directory->child_node_block_index[i] = DELETED_FILE_BLOCK_INDEX;
            return true;
        }
    }
    cout << "rm: no such file or directory: " << item_name_to_use << endl;
    return false;
}

bool MarshmallowFS::FS::write(string buffer, string file_name, bool is_append)
{
    touch(file_name);

    string file_content_to_write = Misc::cut_string_to_length(buffer, 4060, "write", "contents to write");
    string file_name_to_use = Misc::cut_string_to_length(file_name, ITEM_NAME_SIZE - 1, "write", "file_name");

    GenericBlock *current_block = find_(file_name_to_use);

    if (current_block != NULL)
    {
        string temp_name = (const char *)current_block->name;
        if (temp_name == file_name_to_use)
        {
            // these four make compiler happy
            uint16_t current_file_size;
            uint16_t size_to_write;
            File *current_file;
            const char *data_to_write;

            switch (current_block->type)
            {
            case ItemType::normal_directory:
                Logging::error("write", "Target is a directory. Stop.");
                return false;
            case ItemType::general_file:
                current_file = (File *)current_block;
                if (!is_append)
                {
                    current_file->size = 0;
                }
                current_file_size = current_file->size;
                size_to_write = file_content_to_write.length();
                data_to_write = file_content_to_write.c_str();
                for (int current_write_pos = 0; current_write_pos < size_to_write; current_write_pos++)
                {
                    current_file->data[current_file_size + current_write_pos] = data_to_write[current_write_pos];
                    (current_file->size)++;
                }
                return true;
            default:
                Logging::error("write", "Unknown type. Stop.");
                return false;
            }
        }
    }
    else
    {
        string message = file_name_to_use + ": No such file or directory";
        Logging::error("write", message.c_str());
    }
    return false;
}

bool MarshmallowFS::FS::cat(string file_name)
{
    // DirectoryNode *current_directory = get_current_directory();
    string file_name_to_use = Misc::cut_string_to_length(file_name, ITEM_NAME_SIZE - 1, "cat", "file_name");
    GenericBlock *current_block = find_(file_name_to_use);
    if (current_block != NULL)
    {
        //these two make compiler happy
        uint16_t current_file_size;
        File *current_file;

        switch (current_block->type)
        {
        case ItemType::normal_directory:
            Logging::error("cat", "Target is a directory. Stop.");
            return false;
        case ItemType::general_file:
            current_file = (File *)current_block;
            current_file_size = current_file->size;
            for (int current_file_pos = 0; current_file_pos < current_file_size; current_file_pos++)
            {
                printf("%c", current_file->data[current_file_pos]);
            }
            cout << endl;
            return true;
        default:
            Logging::error("cat", "Unknown type. Stop.");
            return false;
        }
    }
    else
    {
        string message = file_name_to_use + ": No such file or directory";
        Logging::error("cat", message.c_str());
    }
    return false;
}

void MarshmallowFS::FS::help()
{
    cout << "Use \"ls\" to list current directory's subdirectories and files.\n"
         << "Use \"cd\" to change into a visible directory.\n"
         << "Use \"touch\" to create a file.\n"
         << "Use \"echo\" to echo something. You can also redirect its output to a file.\n"
         << "Use \"cat\" to print out a file's contents.\n"
         << "Use \"mkdir\" to create a directory.\n"
         << "Use \"rm\" to remove an item.\n"
         << "Use \"pwd\" to check out where are you now.\n"
         << "Use \"mkfs.MFS\" or \"format\" to format the disk.\n"
         << "Use \"info\" to check out current disk usage.\n"
         << "Use \"clear\" to clear screen.\n"
         << "Use \"save\" or \"sync\" to sync data to disk.\n"
         << "Use \"what\" to...\n";
}

void MarshmallowFS::FS::clean(GenericBlock *block)
{
    for (int i = 0; i < RAW_BLOCK_DATA_SIZE; i++)
    {
        block->data[i] = 0x00;
    }
    for (int i = 0; i < ITEM_NAME_SIZE; i++)
    {
        block->name[i] = 0x00;
    }
    block->type = not_a_node;
}

void MarshmallowFS::FS::ls()
{
    DirectoryNode *current_directory = get_current_directory();
    int node_size = current_directory->child_node_count;
    vector<pair<string, ItemType>> file_list;
    for (int i = 0; i < node_size; i++)
    {
        if (current_directory->child_node_block_index[i] == DELETED_FILE_BLOCK_INDEX)
        {
            continue;
        }
        GenericBlock *current_block = get_raw_block_at_block_pos(current_directory->child_node_block_index[i]);
        string temp_name = (const char *)current_block->name;
        ItemType temp_type = static_cast<ItemType>(current_block->type);
        file_list.push_back(make_pair(temp_name, temp_type));
    }
#ifdef DEBUG
    printf("[DEBUG] Item count: %lu\n", file_list.size());
#endif
    if (file_list.size() == 0)
    {
        return;
    }
    for (vector<pair<string, ItemType>>::iterator item = file_list.begin(); item != file_list.end(); item++)
    {
        print_item_with_color((*item).first, (*item).second);
        cout << " ";
    }
    cout << '\n';
}

MarshmallowFS::GenericBlock *MarshmallowFS::FS::find_(string file_name)
{
    DirectoryNode *current_directory = get_current_directory();
    int node_size = current_directory->child_node_count;
    for (int i = 0; i < node_size; i++)
    {
        if (current_directory->child_node_block_index[i] == DELETED_FILE_BLOCK_INDEX)
        {
            continue;
        }
        GenericBlock *current_block = get_raw_block_at_block_pos(current_directory->child_node_block_index[i]);
        string temp_name = (const char *)current_block->name;
        if (temp_name == file_name)
        {
            return current_block;
        }
    }
    return NULL;
}

bool MarshmallowFS::FS::find(string item_name)
{
    string item_name_to_use = Misc::cut_string_to_length(item_name, ITEM_NAME_SIZE - 1, "rm", "item_name");
    GenericBlock *current_block = find_(item_name_to_use);
    if (current_block != NULL)
    {
        cout << item_name_to_use << endl;
        return true;
    }
    return false;
}

uint32_t MarshmallowFS::FS::find_block_pos_(string file_name)
{
    DirectoryNode *current_directory = get_current_directory();
    int node_size = current_directory->child_node_count;
    for (int i = 0; i < node_size; i++)
    {
        if (current_directory->child_node_block_index[i] == DELETED_FILE_BLOCK_INDEX)
        {
            continue;
        }
        GenericBlock *current_block = get_raw_block_at_block_pos(current_directory->child_node_block_index[i]);
        string temp_name = (const char *)current_block->name;
        if (temp_name == file_name)
        {
            return current_directory->child_node_block_index[i];
        }
    }
    return 1919810;
}

MarshmallowFS::DirectoryNode *MarshmallowFS::FS::get_current_directory()
{
    DirectoryNode *current_directory = (DirectoryNode *)(&(disk->BLOCK[current_block_pos_in_image - 1]));
    return current_directory;
}

MarshmallowFS::DirectoryNode *MarshmallowFS::FS::get_directory_at_block_pos(int block_pos)
{
    DirectoryNode *current_directory = (DirectoryNode *)(&(disk->BLOCK[block_pos - 1]));
    return current_directory;
}

MarshmallowFS::File *MarshmallowFS::FS::get_file_at_block_pos(int block_pos)
{
    File *current_file = (File *)(&(disk->BLOCK[block_pos - 1]));
    return current_file;
}

MarshmallowFS::FileData *MarshmallowFS::FS::get_file_data_at_block_pos(int block_pos)
{
    FileData *current_file_data = (FileData *)(&(disk->BLOCK[block_pos - 1]));
    return current_file_data;
}

MarshmallowFS::GenericBlock *MarshmallowFS::FS::get_raw_block_at_block_pos(int block_pos)
{
    GenericBlock *current_block_data = (GenericBlock *)(&(disk->BLOCK[block_pos - 1]));
    return current_block_data;
}

void MarshmallowFS::FS::print_item_with_color(string item_name, MarshmallowFS::ItemType item_type)
{
    if (item_type == ItemType::normal_directory)
    {
        Misc::print_with_foreground_and_background_color(item_name.c_str(), TerminalForegroundColor::Blue_F, TerminalBackgroundColor::BrightGreen_B);
    }
    else
    {
        Misc::print_with_foreground_color(item_name.c_str(), TerminalForegroundColor::BrightGreen_F);
    }
}

bool MarshmallowFS::FS::check_duplicate_item(string name_to_check)
{
    // walk through all item under this directory
    DirectoryNode *current_directory = get_current_directory();
    int node_size = current_directory->child_node_count;
    for (int i = 0; i < node_size; i++)
    {
        if (current_directory->child_node_block_index[i] == DELETED_FILE_BLOCK_INDEX)
        {
            continue;
        }
        GenericBlock *current_block = get_raw_block_at_block_pos(current_directory->child_node_block_index[i]);
        string temp_name = (const char *)current_block->name;
        if (name_to_check == temp_name)
        {
            return true;
        }
    }
    return false;
}