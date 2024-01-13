#include "data.h"
#include <stdio.h>

void print_option_menu()
{
    printf("options:\n");
    printf("  1. loads from file\n");
    printf("  2. create new partition in memory\n");
}

void print_command_menu()
{
    printf("List of commands\n");
    printf("'ls' list directory\n");
    printf("'cd' change directory\n");
    printf("'rm' remove\n");
    printf("'mkdir' make directory\n");
    printf("'rmdir' remove directory\n");
    printf("'put' put file into the space\n");
    printf("'get' get file from the space\n");
    printf("'cat' show content\n");
    printf("'status' show status of the space\n");
    printf("'help'\n");
    printf("'exit and store img'\n");
}

void print_current_path(FileSystem *fs)            // 印出當前所在路徑(絕對路徑)
{
    printf("%s $ ",fs->currentDirectory);
}

void appendPath(FileSystem *fs, const char *path)  // 當進入資料夾後，更新路徑
{
    strcat(fs->currentDirectory,path);
    strcat(fs->currentDirectory,"/");
}

void removePath(FileSystem *fs, const char *path)  // cd .. 時，移除路徑中的該資料夾名稱
{
    int pathLength = 0, bufLength = 0;
    pathLength = strlen(path);                 // 取得要離開的資料夾名稱長度
    char buf[MAX_DIRECTORY_NAME];
    strcpy(buf,fs->currentDirectory);          // 將當前路徑複製至buf處理
    bufLength = strlen(buf);                   // 計算當前絕對路徑的字串長度
    buf[bufLength - (pathLength + 1)] = '\0';  // 在指定位置放置結束標誌'\0'，截斷字串
    strcpy(fs->currentDirectory,buf);          // 調整後字串複製回系統
}