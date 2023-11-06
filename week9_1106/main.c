#include <stdio.h>
typedef unsigned int Int32;

int check_range_loop (Int32 input, Int32 high, Int32 low);
int check_range_no_loop (Int32 input, Int32 high, Int32 low);

int main(void)
{
    Int32 input;
    Int32 high;
    Int32 low;
    int next = 1;

    while(next == 1)
    {
        printf("Please specify the input: ");
        scanf("%x",&input);
        do
        {
            printf("    Please specify the high: ");
            scanf("%d",&high);
        } while (high > 31 || high < 0);
        do
        {
            printf("    Please specify the low : ");
            scanf("%d",&low);
        } while (low > 31 || low < 0 || low > high);
        printf("check_range_loop result: %d\n",check_range_loop(input,high,low));
        printf("check_range_no_loop result: %d\n\n",check_range_no_loop(input,high,low));
        next = 0;
        printf("Input 1 to keep trying the next round: ");
        scanf("%d",&next);
    }
}

int check_range_loop(Int32 input, Int32 high, Int32 low)
{
    int i;
    Int32 mask = 0x00000000;

    for(i=(int)high;i>(int)low-1;i--)
    {        
        mask |= (1 << i);
    }
#ifdef DEBUG
    printf("mask :0x%08x\n",mask);
    printf("input:0x%08x\n",input);
    printf("in&ma:0x%08x\n",input & mask);
#endif
    if((input & mask) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int check_range_no_loop(Int32 input, Int32 high, Int32 low)
{
    Int32 mask;

    if(high == 31 && low == 0)  // avoid overflow
    {
        mask = 0xffffffff;
    }
    else
    {
        mask = ((1 << (high - low + 1)) - 1) << low;
    }
#ifdef DEBUG
    printf("mask :0x%08x\n",mask);
    printf("input:0x%08x\n",input);
    printf("in&ma:0x%08x\n",input & mask);
#endif  
    if((input & mask) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}