#include "ui.h"
#include "list.h"

void ui_main_menu(tNumStorHead *list)
{
    int addOrDelete = 0;

    printf("1. Add a number or 2. Delete a number: ");
    scanf("%d",&addOrDelete);
    if(addOrDelete == 1)
    {
        ui_add_menu(list);
    }
    else if(addOrDelete == 2)
    {
        if(list->counts == 0)
        {
            printf("   There is nothing to delete!\n\n");
        }
        else
        {
            ui_get_del_location(list);
        }
    }
    else
    {
        printf("  No such choose ! \n");
    }
}

void ui_add_menu(tNumStorHead *list)
{
    int addNum = 0;

    printf(" Add a number: ");
    scanf("%d",&addNum);
    ui_get_add_location(list, addNum);
}
void ui_get_add_location(tNumStorHead *list, int addNum)
{
    int location = 0;    //target location
    int BA = 0;          //Before After
    
    if(list->counts != 0)
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
    }
    
    list_add_node(list, location, BA, addNum);
    list_print(list);
}

void ui_get_del_location(tNumStorHead *list)
{
    int location = 0;    //target location

    while(location > list->counts || location <= 0)  //check if it is in range
    {
        printf("  Specify a target location: ");
        scanf("%d",&location);
        if(location > list->counts || location <= 0) //out of range
        {
            printf("  Error location !\n");
        }
    }
    list_del_node(list,location);
    list_print(list);
}