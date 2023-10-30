#include "ui.h"
#include "list.h"

int main(void)
{
    tNumStorHead *list;

    list = initial_list();
    while(1)
    {
        ui_main_menu(list);
    }
}