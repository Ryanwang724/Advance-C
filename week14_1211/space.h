#ifndef __SPACE__
#define __SPACE__

#include "main.h"
#include <math.h>

#define NUM_BYTE_BUF           8
#define UNIT_ELEMENT_SIZE      32

typedef struct {
    int row;
    int location;
}storageLocation;

extern unsigned char **byte_buf_mask;
extern storageLocation result;

void create_mask (void);
void free_mask (void);
int print_buffer_status(void);
int check_buf_is_all_full(void);
void our_malloc(int type, void **target, int *mem_location);
storageLocation find_location(unsigned char **mask, int data_type);
void set_bit(unsigned char **mask, int row, int location, int data_type);
void clear_bit(unsigned char **mask, int row, int location, int data_type);

#endif