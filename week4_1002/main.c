#include <stdio.h>
#include <stdlib.h>

typedef struct num_storage
{
    int number;
    struct num_storage *next;
}tNumStorage;

typedef struct num_stor_head
{
    int counts;
    struct num_storage *head;
    struct num_storage *tail;
}tNumStorHead;

void initial_list(tNumStorHead *list); 
void get_input(tNumStorHead *list); 
void print_list(tNumStorHead *list); 
void sort_list(tNumStorHead *list, int input);

int main(void)
{
    int i;
    tNumStorHead *list;
    list = (tNumStorHead *) malloc (sizeof(tNumStorHead));
    initial_list(list);
    get_input(list);
}

void initial_list(tNumStorHead *list)
{
    list->counts = 0;
    list->head = NULL;
    list->tail = NULL;
}

void print_list(tNumStorHead *list)
{
    tNumStorage *node_ptr = list->head;

    printf("  The sorted list: ");
    while(node_ptr != NULL)
    {
        printf("%d ", node_ptr->number);
        node_ptr = node_ptr->next;
    }
    printf("\n\n");
}

void get_input(tNumStorHead *list)
{
    int newNum;
    printf("Input a number: ");
    scanf("%d",&newNum);
    while(newNum != -1)
    {
        sort_list(list,newNum);
        printf("Input a number: ");
        scanf("%d",&newNum);
    }
}

void sort_list(tNumStorHead *list, int input)
{
    tNumStorage *newNode;
    newNode = (tNumStorage *) malloc (sizeof(tNumStorage));
    newNode->number = input;
    newNode->next = NULL;

    if(list->counts == 0)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else 
    {
        if(input < list->head->number)
        {
            newNode->next = list->head;
            list->head = newNode;
        }
        else
        {
            tNumStorage *now = list->head;
            while(now->next != NULL && now->next->number < input)
            {
                now = now->next;
            }
            newNode->next = now->next;
            now->next = newNode;
        }
    }
    list->counts++;
    printf("  list->counts: %d\n",list->counts);
    print_list(list);
}