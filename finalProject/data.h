#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#ifdef _WIN32
#include <windows.h>
#include <sys/stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#define MAX_FILE_NAME 50
#define MAX_DIRECTORY_NAME 50

typedef struct directory
{
    char dirName[MAX_DIRECTORY_NAME];  // 資料夾名稱
    struct directory *child;           // 子資料夾的header
    struct directory *next;            // 同一層的下個資料夾
    struct directory *prev;            // 同一層的前一個資料夾    
    struct directory *parent;          // 父資料夾
    struct file *fileHead;             // 該資料夾的檔案;
}tDirectory;

typedef struct file
{
    char fileName[MAX_FILE_NAME];   // 檔案名稱
    char *content;                  // 檔案內容指標
    size_t size;                    // 檔案大小
    struct file *nextFile;          // 該資料夾內的下個檔案
    struct file *prevFile;          // 該資料夾內的前一個檔案
    size_t start_position;          
}tFile;

typedef struct system  
{
    tDirectory *root;                             // 根目錄
    tDirectory *workDir;                          // 紀錄當前資料夾
    size_t totalSize;                             // partition的總空間
    size_t remainSize;                            // partition的剩餘空間 
    char *partition;                              // partition 指標
    size_t used_part_loc;
    char currentDirectory[MAX_DIRECTORY_NAME];    // 當前所在路徑(絕對路徑)
}FileSystem;

void file_st_pos_update_recursive(tDirectory *dirPtr, size_t up_range, size_t up_size);
void file_st_pos_update(FileSystem *fs, size_t up_range, size_t up_size);
void initializeDirectory(tDirectory *tD);
FileSystem* initializeFileSystem(size_t partitionSize);
int checkName(const char *newDirName);
void initializeFile(tFile *tF);

void ls(FileSystem *fs);
void cd(FileSystem *fs, const char *path);
void rm(FileSystem *fs, tDirectory *target_dir, const char *delFileName);
void rmdirect(FileSystem *fs, tDirectory *target_dir, const char *delPathName);
void mk(FileSystem *fs, const char *newDirName);
void put(FileSystem *fs, const char *FILE_NAME);
void get(FileSystem *fs, const char *FILE_NAME);
void cat(FileSystem *fs, const char *FILE_NAME);
void status(FileSystem *fs);
void help();
void save_directory_recursive(FILE *fp, tDirectory *dirPtr, const char *currentPath);
void save_file_system_state(const char *filename, FileSystem *fs);
tDirectory *createDirectory(const char *dirName);
void addChildDirectory(tDirectory *parent, tDirectory *child);
tDirectory *findChildDirectory(tDirectory *parent, const char *dirName);
tDirectory *find_dir(FileSystem *fs, const char *path);
void mkSubdirectories(FileSystem *fs, const char *path);
void trim_newline(char *str);
void addFileToDirectory(char *inputString, tDirectory *rootDirectory ,FileSystem *fs, int cnt);
void load_file_system_state(FILE *fp, FileSystem *fs);
size_t readRemainingPartitionSize(FILE *fp);
void reverseLinkedListFile(tDirectory *targetDir);

#endif