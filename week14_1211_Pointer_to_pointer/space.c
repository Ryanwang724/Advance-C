#include "space.h"

unsigned char buffer[UNIT_ELEMENT_SIZE*NUM_BYTE_BUF];

int is_multiple_of_eight = (NUM_BYTE_BUF % 8 ==0? 1:0); //check NUM_BYTE_BUF 是不是8的倍數
int rows = NUM_BYTE_BUF / 8 + 1 - (NUM_BYTE_BUF % 8 ==0? 1:0);  //計算需要幾個row
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

    for(int i=0;i<rows;i++)   // initial
    {
        if(i==0 && is_multiple_of_eight != 1) // 如果不是8的倍數，對第0 row特殊處理
        {
            *byte_buf_mask[i] = 255 - pow(2,remain)+1;  //將不會用到的bit設1
        }
        else
        {
            *byte_buf_mask[i] = 0;
        }
    }
    printf ("\n");
}

void free_mask (void)
{
    for (int i = 0; i < rows; i++) 
    {
        free(byte_buf_mask[i]);
    }
    free(byte_buf_mask);
}  

int print_buffer_status (void)
{
    int i,j;
    int currentBit;
    int remainMemorySpace = 0;
    unsigned char mask = 0x80;

    printf ("      byte_buf_mask: ");
    
    for(i = 0; i<rows; i++)
    {
        if(i==0 && remain != 0)  //first rows and NUM_BYTE_BUF not a multiple of 8
        {
            mask = mask >> (8 - remain); //將mask移到指定位置開始
            for (j = 0; j< remain; j++)
            {
                currentBit = (*byte_buf_mask[i] & mask) >> (remain-j-1);
                printf ("%d ", currentBit); //印出first row
                if(currentBit == 0)
                {
                    remainMemorySpace++;  //在printf的同時計算剩餘的空間數量
                }
                mask = mask >> 1;
            }
            printf(", ");
            mask = 0x80; //reset mask
        }
        else
        {
            for (j = 0; j< 8; j++)
            {
                currentBit = (*byte_buf_mask[i] & mask) >> (7-j);
                printf ("%d ", currentBit);
                if(currentBit == 0)
                {
                    remainMemorySpace++; //在printf的同時計算剩餘的空間數量
                }
                mask = mask >> 1;
            }
            printf(", ");
            mask = 0x80; //reset mask
        }
    }
    printf ("\n");
    return remainMemorySpace;
}

int check_buf_is_all_full(void)     //return 0 is not full; 1 is all full
{
    int flag = 0;  //0 is not full; 1 is all full

    if(is_multiple_of_eight != 1)  //如果 NUM_BYTE_BUF 不被8整除，對first row特殊處理
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
        result = find_location(byte_buf_mask, type);  //找到在哪一row的location
        if(result.location != -1 && result.row != -1)
        {
            set_bit(byte_buf_mask, result.row, result.location, type);       
            location = (rows-1-result.row)*8 + result.location;  //location範圍:0~NUM_BYTE_BUF
            *target = &buffer[location*UNIT_ELEMENT_SIZE];
            *mem_location = location;
        }
        else
        {
            return;
        }
    }
}

storageLocation find_location(unsigned char **buf, int data_type)
{
    int space = 0;
    int currentRow = rows - 1;
    unsigned char mask = 0x01;
    storageLocation result;
    int cnt = 0;

    while(currentRow >= 0)  //從最大row開始(位置:0~7)
    {
        for(int bitIndex=0;bitIndex<8;bitIndex++)
        {
            if((mask & *buf[currentRow]) == 0)
            {
                space++;  //計算空間數
                if(space == data_type)
                {
                    while((bitIndex - data_type + 1) < 0)
                    {
                        cnt++;
                        bitIndex += 8;
                    }
                    result.row = currentRow + cnt; //連續空間開始的 row
                    result.location = bitIndex - data_type + 1; //該row的location(0~7)
                    return  result;
                }
            }
            else
            {
                space = 0;  //reset counter
            }
            mask = mask << 1;
        }
        currentRow--;
        mask = 0x01;
    }
    result.row = -1;
    result.location = -1;
    return result;
}

void set_bit(unsigned char **mask, int row, int location, int data_type)
{
    unsigned char set = 0x01;

    set = set << location;  //go to target bit
    
    for(int i=0;i<data_type;i++) //要做幾次將bit設1
    {
        *mask[row] = *mask[row] | set;
        if(set == 0x80)
        {
            set = 0x01; //reset
            row = row - 1;
        }
        else
        {
            set = set << 1;
        }
    }
}

void clear_bit(unsigned char **mask, int row, int location, int data_type)
{
    unsigned char set = 0x01;

    set = set << location;  //go to target bit

    for(int i=0;i<data_type;i++)
    {
        *mask[row] = *mask[row] & (~set);
        if(set == 0x80)
        {
            set = 0x01; //reset
            row = row - 1;
        }
        else
        {
            set = set << 1;
        }
    }
}