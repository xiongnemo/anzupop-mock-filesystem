#ifndef FS_H
#define FS_H

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
// where is istresmstring??
#include <bits/stdc++.h>
#include "utils.h"

#define IMAGE_SIZE 0x800000
#define IMAGE_SIG "M@r5hM@lL0WF$"
#define IMAGE_SIG_SIZE 13

#define DATA_END_SIG 0xDDAA
#define DATA_LEFT_SIG 0xDDBB
#define DELETED_FILE_BLOCK_INDEX 0xFFFFFFFF

#define ITEM_NAME_SIZE 16

// 4k block
#define BLOCK_SIZE 0x1000
// Raw block's data size
#define RAW_BLOCK_DATA_SIZE (BLOCK_SIZE - 4 - 16)
// total block count, should be 0x800 (2048)
#define BLOCK_COUNT (IMAGE_SIZE / BLOCK_SIZE)
// #define DEBUG

using std::cin;
using std::cout;
using std::istringstream;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;
using namespace Utils;

namespace MarshmallowFS
{
    enum ItemType
    {
        not_a_node,
        normal_directory,
        general_file,
        general_file_data_block,
        root_directory,
        superblock
    };

    // file information and data (4k block)
    struct File
    {
        ItemType type = ItemType::general_file;     // 4 bytes
        unsigned char name[ITEM_NAME_SIZE];         // 16 bytes
        uint16_t size;                              // 2 bytes
        uint32_t father_directory_node_block_index; // 4 bytes
        uint8_t data[4060];                         // data *supposed to be 4064, but sizeof() dont think so*
        uint16_t data_block_end_sig;                // 2 bytes
        uint32_t next_data_block_index;             // 4 bytes
    };

    // pure file data (4k block)
    struct FileData
    {
        ItemType type = ItemType::general_file_data_block; // 4 bytes
        unsigned char name[ITEM_NAME_SIZE];                // 16 bytes
        uint32_t father_file_node_block_index;             // 4 bytes
        uint8_t data[4066];                                // data
        uint16_t data_block_end_sig;                       // 2 bytes
        uint32_t next_data_block_index;                    // 4 bytes
    };

    // directory information (4k block)
    struct DirectoryNode
    {
        ItemType type = ItemType::normal_directory; // 4 bytes
        unsigned char name[ITEM_NAME_SIZE];         // 16 bytes
        uint16_t size;                              // 2 bytes, no use
        uint32_t father_directory_node_block_index; // 4 bytes
        uint16_t child_node_count;                  // 2 bytes
        uint32_t child_node_block_index[1016];      // 1016 * 4 = 4064 bytes *supposed to be 4068, but sizeof() dont think so*
    };

    // generic 4k block
    struct GenericBlock
    {
        ItemType type;                     // 4 bytes
        unsigned char name[16];            // 16 bytes
        uint8_t data[BLOCK_SIZE - 4 - 16]; // data
    };

    struct SuperBlock // 4k
    {
        unsigned char sig[ITEM_NAME_SIZE];             // 16 bytes of signature, actually it's 13 bytes.
        unsigned char label[ITEM_NAME_SIZE];           // 16 bytes of disk label
        uint8_t block_allocation_table[2048];          // block allocation table
        uint8_t reserved_data[BLOCK_SIZE - 32 - 2048]; // to the end of first 4k block
    };

    struct VituralDisk // 0x800000 bytes
    {
        SuperBlock superblock;               // 4k superblock
        GenericBlock BLOCK[BLOCK_COUNT - 1]; // 2047 * 4k blocks
    };

    class FS
    {
    public:
        FS()
        {
            image_contents = new uint8_t[IMAGE_SIZE];
            current_block_pos_in_image = 1;
            disk = NULL;
        };
        // FS mock shell
        void shell();
        // This function init an empty image on the disk.
        void init_image();
        // This function read the file system image from the disk.
        void load_image();
        // This function save the current file system to an image on the disk.
        void save_image();
        // This function check if the given image file exist.
        bool is_image_exist();
        // This function check if the file is really the file with given signature.
        bool is_image_vaild();
        string file_name;

    private:
        // This function formats/re-init image in memory.
        bool format();
        // This function print currrent position, and return a char[] like "/sys/bin"
        void pwd();
        // This function get currrent position, and return a char[] like "/sys/bin"
        const char *pwd_();
        // This function list all files and directories under this folder, incluing . and ..
        void ls();
        // This function print the image's info.
        void info();
        // This function creates an empty file.
        void touch(string file_name);
        // This function creates an empty directory.
        bool mkdir(string directory_name);
        // This function enters an directory.
        bool cd(string directory_name);
        // This function removes an item's ENTRY
        bool rm(string item_name);
        // This function writes buffer to a file with name file_name.
        // If file does not exist, we will create it.
        bool write(string buffer, string file_name, bool is_append);
        // This function print file contents to stdout.
        bool cat(string file_name);
        // This function prints help.
        void help();
        // This function cleans a block.
        void clean(GenericBlock *block);
        // return the pointer to current directory where current_block_pos_in_image is
        DirectoryNode *get_current_directory();
        // return the pointer to the directory where block_pos is.
        DirectoryNode *get_directory_at_block_pos(int block_pos);
        // return the pointer to the file where block_pos is.
        File *get_file_at_block_pos(int block_pos);
        // return the pointer to tile data where block_pos is.
        FileData *get_file_data_at_block_pos(int block_pos);
        // return the pointer to raw block where block_pos is.
        GenericBlock *get_raw_block_at_block_pos(int block_pos);
        // helper function
        void print_item_with_color(string item_name, MarshmallowFS::ItemType item_type);
        // return true if duplicate item found with name name_to_check in current directory
        bool check_duplicate_item(string name_to_check);
        // buffer for pwd
        char pwd_buffer[256];
        uint8_t *image_contents;
        uint32_t current_block_pos_in_image;
        VituralDisk *disk;
    };
} // namespace MarshmallowFS

#endif