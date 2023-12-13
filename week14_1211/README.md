# W14 designed program flow

## byte_buf_mask放置方式
假設NUM_BYTE_BUF為23:
![mask放置方式](./readmeImg/mask放置方式.png)

## 變數說明
```C
int rows = NUM_BYTE_BUF / 8 + 1 - (NUM_BYTE_BUF % 8 ==0? 1:0);  
//計算需要幾個row，若NUM_BYTE_BUF為8的倍數，則須額外-1
typedef struct {
    int row;
    int location;
}storageLocation;
//創建新的結構，去存find_location()回傳的目標row跟location
```

## function 說明

### void create_mask (void)
完成malloc後對byte_buf_mask進行初始化，要判斷 NUM_BYTE_BUF 是不是8的倍數，如果不是，要對第0 row特殊處理，以23為例，餘數remain為7，所以第0 row的最高位元不給使用。  
255 - pow(2,remain)+1  
11111111 - 10000000 + 00000001 = 10000000
```C
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
```

### int print_buffer_status (void)

```C
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
```