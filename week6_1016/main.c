#include <stdio.h>
#include <stdlib.h>

typedef struct num_storage
{
    int number;
    struct num_storage *next;
    struct num_storage *previous;
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
void delete_last(tNumStorHead *list);

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
    tNumStorage *node_ptr;

    printf("\n");
    printf("counts: %d \n", list->counts);
    node_ptr = list->head;
    printf(" From head --> ");
    while(node_ptr != NULL)
    {
        printf("%d ", node_ptr->number);
        node_ptr = node_ptr->next;
    }
    node_ptr = list->tail;
    printf("\n From tail --> ");
    while(node_ptr != NULL)
    {
        printf("%d ", node_ptr->number);
        node_ptr = node_ptr->previous;
    }
    printf("\n");
}

void get_input(tNumStorHead *list)
{
    int input = 0;
    while(input != -1)
    {
        printf("Input a number (-1 to exit, -2 to delete last): ");
        scanf("%d",&input);
        if(input == -2)
        {
            delete_last(list);
        }
        else if(input != -1)
        {
            sort_list(list, input);
        }
    }
}

void sort_list(tNumStorHead *list, int input)
{
    tNumStorage *newNode;
    newNode = (tNumStorage *) malloc (sizeof(tNumStorage));
    newNode->number = input;
    newNode->next = NULL;
    newNode->previous = NULL;    

    if(list->counts == 0)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else if(input < list->head->number)
    {
        newNode->next = list->head;
        list->head->previous = newNode;
        list->head = newNode;
    }
    else if(input > list->tail->number)
    {
        newNode->previous = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    else if(list->counts == 1 && input == list->head->number)
    {
        list->head->next = newNode;
        newNode->previous = list->head;
        list->tail = newNode;
    }
    else
    {
        tNumStorage *now = list->head;
        while(now->next != NULL && now->next->number < input)
        {
            now = now->next;
        }
        newNode->previous = now;  
        newNode->next = now->next;
        now->next->previous = newNode;
        now->next = newNode;
    }
    list->counts++;
    printf("  list->counts: %d\n",list->counts);
    print_list(list);
}

void delete_last(tNumStorHead *list)
{  
    if(list->counts > 1)
    {
        list->tail = list->tail->previous;
        list->tail->next = NULL;
    }
    else if(list->counts == 1)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else if(list->counts == 0)
    {
        printf("    There is nothing to delete\n\n");
        return;
    }
    list->counts--;
    printf("  list->counts: %d\n",list->counts);
    print_list(list);
}