#include <stdio.h>
#include <stdlib.h>

typedef struct num_list
{
    int counts;
    int *p_list;
}tNumList;

void fill_list(tNumList *list);
void bubble_sort(tNumList *list);
void merge(tNumList *list1, tNumList *list2);
void print_list(tNumList *list);

int main(void)
{
    tNumList *list1, *list2;
    list1 = (tNumList *) malloc (sizeof(tNumList));
    list2 = (tNumList *) malloc (sizeof(tNumList));

    printf("Please enter how many numbers in list1: ");
    fill_list(list1);
    bubble_sort(list1);
    
    printf("Please enter how many numbers in list2: ");
    fill_list(list2);
    bubble_sort(list2);

    printf("\n");
    printf("sorted list1: ");
    print_list(list1);
    printf("sorted list2: ");
    print_list(list2);
    printf("merged list: ");
    merge(list1,list2);
    return 0;
}

void fill_list(tNumList *list)
{
    int i;
    scanf("%d",&list->counts);
    list->p_list = (int *) malloc (sizeof(int)*list->counts);
    printf("    Please input %d numbers: ",list->counts);
    for(i=0;i<list->counts;i++)
    {
        scanf("%d",&list->p_list[i]);
    }
}

void bubble_sort(tNumList *list)
{
    int i,j,temp;
    for(i=0;i<list->counts-1;i++)
    {
        for(j=0;j<list->counts-i-1;j++)
        {
            if(list->p_list[j]>list->p_list[j+1])
            {
                temp = list->p_list[j];
                list->p_list[j] = list->p_list[j+1];
                list->p_list[j+1] = temp;
            }  
        }
    }
}

void merge(tNumList *list1, tNumList *list2)
{
    int i = 0;
    int j = 0;
    while(i!=list1->counts && j!=list2->counts)
    {
        if(list1->p_list[i]>list2->p_list[j])
        {
            printf("%d ",list2->p_list[j]);
            j++;
        }
        else 
        {
            printf("%d ",list1->p_list[i]);
            i++;
        }
    }
    if(i<list1->counts)
    {
        while(i!=list1->counts)
        {
            printf("%d ",list1->p_list[i]);
            i++; 
        }
    }
    else
    {
        while(j!=list2->counts)
        {
            printf("%d ",list2->p_list[j]);
            j++;
        }
    }
}

void print_list(tNumList *list)
{
    int i;
    for(i=0;i<list->counts;i++)
    {
        printf("%d ",list->p_list[i]);
    }
    printf("\n");
}