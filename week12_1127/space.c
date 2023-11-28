#include "space.h"

unsigned char buffer[ELEMENT_SIZE*NUM_BYTE_BUF];
unsigned char byte_buf_mask = 0;

void print_buffer_status (void)
{
    int i;
    unsigned char mask = 0x80;

    printf ("      byte_buf_mask: ");
    for (i = 0; i< NUM_BYTE_BUF; i++)
    {
        printf ("%d ", (byte_buf_mask & mask) >> (7-i));
        mask = mask >> 1;
    }

    printf ("\n");
}

void our_malloc(int type, void **target, int *mem_location)
{
    int location;

    if (byte_buf_mask == FULL){
        return;
    }
    else if (type == TYPE_SMALL)
    {
        location = test_single_location(byte_buf_mask, NUM_BYTE_BUF);
        set_single_bit(&byte_buf_mask, location);
        *target = &buffer[location*ELEMENT_SIZE];
        *mem_location = location;
    }
    else
    {
        location = test_dual_location(byte_buf_mask, NUM_BYTE_BUF);
        if (location >= 0)
        {
            set_dual_bit(&byte_buf_mask, location);
            *target = &buffer[location*ELEMENT_SIZE];
            *mem_location = location;
        }
        else
        {
            return;
        }
    }    
}

int test_single_location(unsigned char mask, int mask_length)
{
    int loc; 
    unsigned char test = 0x01;
    for(loc=0;loc<mask_length;loc++)
    {
        if((test & mask) == 0)
        {
            return loc;  //0~mask_length-1
        }
        test = test << 1;
    }
}

void set_single_bit(unsigned char *mask, int location)
{
    unsigned char set = 0x01;
    int i; //loop control
    for(i=0;i<location;i++)  //go to target bit
    {
        set = set << 1; 
    }
    *mask = *mask | set;
}

int test_dual_location(unsigned char mask, int mask_length)
{
    int loc; 
    unsigned char test = 0x03;
    for(loc=0;loc<mask_length-1;loc++)
    {
        if((test & mask) == 0)
        {
            return loc;  //loc & loc+1
        }
        test = test << 1;
    }
    return -1;
}

void set_dual_bit(unsigned char *mask, int location)
{
    unsigned char set = 0x03;
    int i; //loop control
    for(i=0;i<location;i++)  //go to target bit
    {
        set = set << 1; 
    }
    *mask = *mask | set;
}

void clear_single_bit(unsigned char *mask, int location)
{
    unsigned char clear = 0x01;
    int i; //loop control
    for(i=0;i<location;i++)  //go to target bit
    {
        clear = clear << 1; 
    }
    *mask = *mask & (~clear);
}

void clear_dual_bit(unsigned char *mask, int location)
{
    unsigned char clear = 0x03;
    int i; //loop control
    for(i=0;i<location;i++)  //go to target bit
    {
        clear = clear << 1; 
    }
    *mask = *mask & (~clear);
}

void our_free(int type, int mem_location)
{
    if (type == TYPE_SMALL)
    {
        unsigned char clear = 0x01;
        for(int i=0;i<mem_location;i++)  //go to target bit
        {
            clear = clear << 1; 
        }
        byte_buf_mask = byte_buf_mask & (~clear);
    }
    else if (type == TYPE_LARGE)
    {
        unsigned char clear = 0x03;
        for(int i=0;i<mem_location;i++)  //go to target bit
        {
            clear = clear << 1; 
        }
        byte_buf_mask = byte_buf_mask & (~clear);
    }
}