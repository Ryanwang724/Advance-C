#include "list.h"
#include "ui.h"

tNumStorHead* initial_list()
{
    tNumStorHead *list;
    list = (tNumStorHead *) malloc (sizeof(tNumStorHead));

    list->counts = 0;
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}
void list_print(tNumStorHead *list)
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
void list_add_node(tNumStorHead *list, int location, int BA, int addNum)
{
    tNumStorage *newNode;
    newNode = (tNumStorage *) malloc (sizeof(tNumStorage));
    newNode->number = addNum;
    newNode->next = NULL;
    newNode->previous = NULL;

    int i = 0;         //loop control

    if(list->counts == 0)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
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
}
void list_del_node(tNumStorHead *list, int location)
{
    int i = 0;            //loop control

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
}