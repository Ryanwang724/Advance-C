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
void add(tNumStorHead *list, int input);
void delete(tNumStorHead *list);

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
    printf("\n\n");
}

void get_input(tNumStorHead *list)
{
    int addOrDelete = 0;
    int addNum = 0;

    while(1)
    {
        printf("1. Add a number or 2. Delete a number: ");
        scanf("%d",&addOrDelete);
        if(addOrDelete == 1)
        {
            printf(" Add a number: ");
            scanf("%d",&addNum);
            add(list, addNum);
        }
        else if(addOrDelete == 2)
        {
            delete(list);
        }
        else
        {
            printf("  No such choose ! \n");
        }
    }
}

void delete(tNumStorHead *list)
{
    int location = -1;    //target location
    int i = 0;            //loop control

    while(location > list->counts || location <= 0)  //check if it is in range
    {
        printf("  Specify a target location: ");
        scanf("%d",&location);
        if(location > list->counts || location <= 0) //out of range
        {
            printf("  Error location !\n");
        }
    }

    if(list->counts == 1 && location == 1)   //if only one node
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else if(location == 1)      //最前面
    {
        list->head = list->head->next;
        list->head->previous = NULL;
    }
    else if(list->counts != 1 && location == list->counts)     //最後面
    {
        list->tail = list->tail->previous;
        list->tail->next = NULL;
    }
    else         //中間
    {
        tNumStorage *ptr;
        ptr = list->head;
        for(i=0;i<location-1;i++)  //move to target location
        {
            ptr = ptr->next;
        }
        ptr->previous->next = ptr->next;
        ptr->next->previous = ptr->previous;
    }
    list->counts--;
    print_list(list);
}

void add(tNumStorHead *list, int input)
{
    tNumStorage *newNode;
    newNode = (tNumStorage *) malloc (sizeof(tNumStorage));
    newNode->number = input;
    newNode->next = NULL;
    newNode->previous = NULL;

    int location = -1;  //target location
    int BA = -1;        //Before After
    int i = 0;         //loop control

    if(list->counts == 0)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        while(location > list->counts || location <= 0)  //check if it is in range
        {
            printf("  Specify a target location: ");
            scanf("%d",&location);
            if(location > list->counts || location <= 0) //out of range
            {
                printf("  Error location !\n");
            }
        }

        while(BA != 1 && BA != 2)  
        {
            printf("  1. Before or 2. After the location %d: ",location);
            scanf("%d",&BA);
            if(BA != 1 && BA != 2) 
            {
                printf("  Error location !(1.Before or 2. After)");
            }
        }

        if(BA == 1 && location == 1) //最前面
        {
            newNode->next = list->head;
            list->head->previous = newNode;
            list->head = newNode;
        }
        else if(BA == 2 && location == list->counts) //最後面
        {
            newNode->previous = list->tail;
            list->tail->next = newNode;
            list->tail = newNode;
        }
        else //中間
        {
            tNumStorage *ptr;
            ptr = list->head;
            for(i=0;i<location-1;i++)  //move to target location
            {
                ptr = ptr->next;
            }
            if(BA == 1)   //before target location
            {
                newNode->next = ptr;
                newNode->previous = ptr->previous;
                ptr->previous->next = newNode;
                ptr->previous = newNode;
            }
            else if(BA == 2)   //after target location
            {
                newNode->previous = ptr;
                newNode->next = ptr->next;
                ptr->next = newNode;
                ptr->next->previous = newNode;
            }
        }
    }
    list->counts++;
    print_list(list);
}