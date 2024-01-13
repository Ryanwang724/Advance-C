#ifndef UI_H
#define UI_H
#include "data.h"

void print_option_menu();
void print_command_menu();
void print_current_path(FileSystem *fs);
void appendPath(FileSystem *fs, const char *path);
void removePath(FileSystem *fs, const char *path);

#endif