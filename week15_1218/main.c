#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4
typedef char my_int[MAX];
int carry = 0;

void my_add(my_int returnSum, my_int input1, my_int input2);
void my_number_print(my_int result);
void trans_from_string(my_int returnString, char *inputString);

int main()
{
    my_int add1;
    my_int add2;
    my_int sum;
    char inputBuffer[MAX] = {0};

    for(int i=0;i<MAX;i++)  //initial
    {
        add1[i] = 0;
        add2[i] = 0;
        sum[i] = 0;
    }

    printf("Please give a number less than %d:",MAX);
    scanf("%s",inputBuffer);
    trans_from_string(add1, inputBuffer); 
    printf("Please give another number less than %d:",MAX);
    scanf("%s",&inputBuffer);
    trans_from_string(add2, inputBuffer);
    my_add(sum, add1, add2);
    if(carry == 1)
    {
        printf("Overflow!\n");
    }
    else
    {
        my_number_print(sum);
    }

    return 0;
}

void my_add(my_int returnSum, my_int input1, my_int input2)
{
    carry = 0;
    int tempResult = 0;
    for(int i=MAX-1;i>=0;i--)
    {
        tempResult = input1[i] + input2[i] + carry;
        if(tempResult >= 10)
        {
            returnSum[i] = tempResult - 10;
            carry = 1;
        }
        else
        {
            returnSum[i] = tempResult;
            carry = 0;
        }
    }
}

void my_number_print(my_int result)
{
    int zeroFlag = 0;
    printf("The add result: ");
    for(int i=0;i<MAX;i++)
    {
        if(result[i] == 0 && zeroFlag == 0)
        {
            continue;
        }
        else
        {
            zeroFlag = 1;
            printf("%d",result[i]);
        }
    }
    printf("\n");
}

void trans_from_string(my_int returnString, char *inputString)
{
    int len = strlen(inputString);

    for(int i=0;i<len;i++)
    {
        returnString[i+MAX-len] = inputString[i] - '0';  //轉成0~9
    }
}