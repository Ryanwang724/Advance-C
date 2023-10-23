#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void swap(int * ,int *);

int main(int argc,char *argv[])
{
    int elementCount = atoi(*(argv+1));
    int rowCount = sqrt(atoi(*(argv+1))); 

    int *array;
    array = (int * ) malloc (sizeof(int)*elementCount);

    int i=0;                              
    for(i=0;i<elementCount;i++)
    {
        *(array+i) = atoi(*((argv+2)+i));
        printf("%2d ",*(array+i));              //print alignment
        if((i+1) % rowCount==0)
        {
            printf("\n");
        } 
    }
    printf("\n");   //---------------------------
    for(i=0;i<rowCount;i++)
    {
        swap(array+i,array+(i+rowCount*2));     //change first row and third row
    }
    for(i=0;i<elementCount;i++)
    {
        printf("%2d ",*(array+i));
        if((i+1) % rowCount==0)
        {
            printf("\n");
        }
    }
    printf("\n");   //---------------------------
    for(i=0;i<rowCount;i++)
    {
        swap(array+(i*rowCount),array+(i*rowCount+2));  //change first column and third column
    }
    for(i=0;i<elementCount;i++)
    {
        printf("%2d ",*(array+i));
        if((i+1) % rowCount==0)
        {
            printf("\n");
        }
    }
    printf("\n");   //---------------------------
    return 0;
}
void swap(int *a,int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
/*
9 1 2 3 4 5 6 7 8 9
16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
25 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
*/
