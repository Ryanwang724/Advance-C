#include "space.h"

unsigned char buffer[UNIT_ELEMENT_SIZE*NUM_BYTE_BUF];

int rows = NUM_BYTE_BUF / 8 + 1;
int is_multiple_of_eight = (NUM_BYTE_BUF % 8 ==0? 1:0); //check 
int remain = NUM_BYTE_BUF % 8; //餘數 or 第一row要印多少bit

unsigned char **byte_buf_mask;
storageLocation result;

void create_mask (void)
{
    byte_buf_mask = (unsigned char **)malloc(rows*sizeof(unsigned char *));
    for(int i=0;i<rows;i++)
    {
        byte_buf_mask[i] = (unsigned char *) malloc(sizeof(unsigned char));
    }

    for(int i=0;i<rows;i++)   //initial
    {
        *byte_buf_mask[i] = 0;
    }
}

void free_mask (void)
{
    for (int i = 0; i < rows; i++) 
    {
        free(byte_buf_mask[i]);
    }
    free(byte_buf_mask);
}  

void print_buffer_status (void)
{
    int i,j;
    unsigned char mask = 0x80;

    printf ("      byte_buf_mask: ");
    
    for(i = 0; i<rows; i++)
    {
        if(i==0 && remain != 0)  //first rows and NUM_BYTE_BUF not a multiple of 8
        {
            mask = mask >> (8 - remain); //將mask移到指定位置開始
            for (j = 0; j< remain; j++)
            {
                printf ("%d ", (*byte_buf_mask[i] & mask) >> ((8 - remain - 1)-j)); //印出first row
                mask = mask >> 1;
            }
            printf(", ");
            mask = 0x80; //reset mask
        }
        else
        {
            for (j = 0; j< 8; j++)
            {
                printf ("%d ", (*byte_buf_mask[i] & mask) >> (7-j));
                mask = mask >> 1;
            }
            printf(", ");
            mask = 0x80; //reset mask
        }
    }
    printf ("\n");
}

int check_buf_is_all_full(void)     //0 is not full; 1 is all full
{
    int flag = 0;  //0 is not full; 1 is all full

    if(is_multiple_of_eight != 1)  //NUM_BYTE_BUF 不被8整除，對first row特殊處理
    {
        if(*byte_buf_mask[0] != (unsigned char)pow(2,remain))
        {
            return 0;
        }
        for(int i=1;i<rows;i++)
        {
            if(*byte_buf_mask[i] != 255)
            {
                return 0;
            }
            return 1;
        }
    }
    else  //NUM_BYTE_BUF 被8整除
    {
        for(int i=0;i<rows;i++)
        {  
            if(*byte_buf_mask[i] != 255)
            {
                return 0;
            }
        }
        return 1;
    }
}

void our_malloc(int type, void **target, int *mem_location)
{
    int location;
    if (check_buf_is_all_full() == 1){
        return;
    }
    else
    {
        result = find_location(byte_buf_mask, type);
// #ifdef DEBUG        
//         printf("location: %d, row: %d",result.location,result.row);
// #endif
        printf("location: %d, row: %d\n",result.location,result.row);
        set_bit(byte_buf_mask, result.row, result.location, type);       
        location = (rows-1-result.row)*8 + result.location;
        *target = &buffer[location*UNIT_ELEMENT_SIZE];
        *mem_location = location;
    }
}

storageLocation find_location(unsigned char **mask, int data_type)
{
    int space = 0;
    int currentRow = rows - 1;
    unsigned char test = 0x01;
    storageLocation result;
    int cnt = 0;

    while(currentRow >= 0)
    {
        for(int bitIndex=0;bitIndex<8;bitIndex++)
        {
            if((test & *mask[currentRow]) == 0)
            {
                space++;  //計算空間數
                if(space == data_type)
                {
                    while((bitIndex - data_type + 1) < 0)
                    {
                        cnt++;
                        bitIndex += 8;
                    }
                    result.row = currentRow + cnt; //space start location 的 row
                    result.location = bitIndex - data_type + 1;
                    return  result;  //return space start location
                }
            }
            else
            {
                space = 0;  //reset counter
            }
            test = test << 1;
        }
        currentRow--;
        test = 0x01;
    }
    result.row = -1; //space start location 的 row
    result.location = -1;
    return result;
}

void set_bit(unsigned char **mask, int row, int location, int data_type)
{
    unsigned char set = 0x01;

    if(location != 0)
    {
        set = set << location-1;  //go to target bit
    }
    for(int i=0;i<data_type;i++)
    {
        *mask[row] = *mask[row] | set;
        set = set << 1;
        if(set == 0x80)
        {
            set = 0x01; //reset
            row = row - 1;
        }
    }
}

void clear_bit(unsigned char **mask, int row, int location, int data_type)
{
    unsigned char set = 0x01;

    set = set << location-1;  //go to target bit
    for(int i=0;i<data_type;i++)
    {
        set = set << 1;
        *mask[row] = *mask[row] & (~set);
        if(set == 0x80)
        {
            set = 0x01; //reset
            row = row - 1;
        }
    }
}