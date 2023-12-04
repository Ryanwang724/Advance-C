#include "space.h"

unsigned char buffer[SMALL_ELEMENT_SIZE*NUM_SMALL_BYTE_BUF + NUM_LARGE_BYTE_BUF*LARGE_ELEMENT_SIZE];

unsigned char byte_large_buf_mask = 0;
unsigned char byte_small_buf_mask = 0;

void print_buffer_status (void)
{
    int i;
    unsigned char mask = 0x80;

    printf ("      byte_small_buf_mask: ");
    for (i = 0; i< NUM_SMALL_BYTE_BUF; i++)
    {
        printf ("%d ", (byte_small_buf_mask & mask) >> (7-i));
        mask = mask >> 1;
    }
    mask = 0x80;  // reset mask
    printf ("\n      byte_large_buf_mask: ");
    for (i = 0; i< NUM_LARGE_BYTE_BUF; i++)
    {
        printf ("%d ", (byte_large_buf_mask & mask) >> (7-i));
        mask = mask >> 1;
    }

    printf ("\n");
}

void our_malloc(int type, void **target, int *mem_location)
{
    int location;

    if (byte_small_buf_mask == FULL && byte_large_buf_mask == FULL){
        return;
    }
    else if (type == TYPE_SMALL)
    {
        if(byte_small_buf_mask == FULL)  //放到大的buf
        {
            location = test_single_location(byte_large_buf_mask, NUM_LARGE_BYTE_BUF);
            set_single_bit(&byte_large_buf_mask, location);
            *target = &buffer[LARGE_START+location*SMALL_ELEMENT_SIZE];
            *mem_location = NUM_LARGE_BYTE_BUF + location;
        }
        else
        {
            location = test_single_location(byte_small_buf_mask, NUM_SMALL_BYTE_BUF);
            set_single_bit(&byte_small_buf_mask, location);
            *target = &buffer[location*SMALL_ELEMENT_SIZE];
            *mem_location = location;
        }
    }
    else
    {
        if(byte_large_buf_mask == FULL)  //放到小的buf
        {
            location = test_dual_location(byte_small_buf_mask, NUM_SMALL_BYTE_BUF);
            if (location >= 0)
            {
                set_dual_bit(&byte_small_buf_mask, location);
                *target = &buffer[location*SMALL_ELEMENT_SIZE];
                *mem_location = location;
            }
            else
            {
                return;
            }
        }
        else
        {
            location = test_single_location(byte_large_buf_mask, NUM_LARGE_BYTE_BUF);
            if (location >= 0)
            {
                set_single_bit(&byte_large_buf_mask, location);
                *target = &buffer[LARGE_START+location*LARGE_ELEMENT_SIZE];
                *mem_location = NUM_LARGE_BYTE_BUF + location;
            }
            else
            {
                return;
            }
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
        clear_single_bit(&byte_small_buf_mask, mem_location);
    }
    else if (type == TYPE_LARGE)
    {
        clear_single_bit(&byte_large_buf_mask, mem_location-NUM_SMALL_BYTE_BUF);
    }
}