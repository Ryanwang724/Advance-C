#include <stdio.h>
#include <stdlib.h>

#define TOTAL 200

int comp_inc (char x, char y)
{




}

int comp_dec (char x, char y)
{




}

void bubble_sort(char *buf, int length, ____callback_____)
{
    int i, j;
    char tmp;

    for (i = 0; i< (length - 1); i++)
    {
        for (j = i; j< length; j++)
        {
            if (xxxxxxxxxxxxxxxxxxxx)
            {
                tmp = buf[i];
                buf[i] = buf[j];
                buf[j] = tmp;   
            }
        }
    }
}

void print_result (char *buf)
{
    int i;
    printf("    Result: %s \n", buf);
}

int main (void)
{
    int i, flag, length;
    char *buf = __________________;
    ______________________________;

    while (1)
    {        
        //menu 
        printf("Please enter (0) increasing or (1) descesing sort: ");
        scanf("%d", &flag);
        if (flag < 0 || flag > 1)
        {
            printf("ERROR: no such option! \n\n");
            continue;
        }
        
        printf("  Please enter a string: ");
        scanf(____________________);

        //here you can modify to function pointer array
        bubble_sort(buf, length-1, _________________);
        print_result(buf);
    }
}

