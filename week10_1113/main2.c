#include <stdio.h>
#include <stdlib.h>

#define TOTAL 200

int H2D (char *buf, int length)
{
    int i,j;
    int result = 0;
    int power = 1;
    for(i=0;i<length;i++)
    {
        power = 1;                                     //initial power
        for(j=0;j<length-i-1;j++)
        {
            power *= 16;
        }
        if(*(buf+i) >= 'A' && *(buf+i) <= 'F')         //A~F
        {
            result += (*(buf+i) - 'A' + 10) * power;
        }
        else if(*(buf+i) >= 'a' && *(buf+i) <= 'f')    //a~f
        {
            result += (*(buf+i) - 'a' + 10) * power;
        }
        else if(*(buf+i) >= '0' && *(buf+i) <= '9')    //0~9
        {
            result += (*(buf+i) - '0') * power;
        }
    }
    printf("HEX to decimal result is: ");
    return result;
}

int B2D (char *buf, int length)
{
    int i,j;
    int result = 0;
    int power = 1;
    for(i=0;i<length;i++)
    {
        power = 1;                                     //initial power
        for(j=0;j<length-i-1;j++)
        {
            power *= 2;
        }
        if(*(buf+i) >= '0' && *(buf+i) <= '1')         //0 or 1
        {
            result += (*(buf+i) - '0') * power;
        }
    }
    printf("binary to decimal result is: ");
    return result;
}

void convert(char *buf, int length, int (*func)(char *, int))
{
    int result;
    result = func(buf,length);
    printf("%d\n\n",result);
}

int main (void)
{
    int i, length;
    char HorB;
    char *buf = (char*) malloc( TOTAL * sizeof(char));
    int (*func[2])(char *, int) = {H2D, B2D};
    
    while (1)
    {   
        length = 0;                                    //initial length
        printf("Please enter HEX or binary string to decimal: ");
        scanf("%c",&HorB);                             //use first char to decide (HEX or binary) convert to decimal.
        if(HorB == 'h' || HorB == 'H')
        {
            scanf("%[a-fA-F0-9]%n",buf,&length);       //if HEX ,accepted input is [a~f][A~F][0-9].
            convert(buf,length,H2D);
        }
        else if(HorB == 'b' || HorB == 'B')
        {
            scanf("%[01]%n",buf,&length);              //if binary ,accepted input is [0~1].
            convert(buf,length,B2D);
        }
        else
        {
            printf("Please use h(H) or b(B) start !\n");
        }
        while(getchar() != '\n')                       //clear stdin  buffer
            continue;
    }
}