#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum ServiceProvider
{
    CHT, 
    FET, 
    TWN
}tServiceEnum;

typedef struct HomePhoneNumber
{
    char number[15];
    char areaCode[5];
}tHome;

typedef struct CellularPhoneNumber
{
    char number[15];
    tServiceEnum provider;
}tCellular;

typedef struct user
{
    char name[10];
    union 
    {
        tHome home;
        tCellular cellular;
    }phoneNumber;
    struct user *next;
    struct user *prev;
    int phoneType;
}tUser;

typedef struct user_head
{
    int counts;
    struct user *head;
    struct user *tail;
}tUserHead;

tUserHead* initial_user_list(void);
void print_user_info(tUserHead *list);
void add_to_list(tUserHead *list, int type, char *name, char *number, char *areaCode, int *service);

int main()
{
    char nameBuf[10];
    int type; // 1: home 2: cellular
    int service;
    char numBuf[15];
    char areaCodeBuf[5];
    
    tUserHead *list;
    list = initial_user_list();
    while (1)
    {
        printf("Enter new user name: ");
        scanf("%s",nameBuf);
        printf("Enter new user phone number type (1:home 2:cellular): ");
        scanf("%d",&type);
        if(type == 1)
        {
            printf("Enter area code: ");
            scanf("%s",areaCodeBuf);
            printf("Enter new user phone number \"home\" (without area code): ");
            scanf("%s",numBuf);
            add_to_list(list, type, nameBuf, numBuf, areaCodeBuf, NULL);
        }
        else if(type == 2)
        {
            printf("Enter service provider (CHT:0 FET: 1 TWN:2): ");
            scanf("%d",&service);
#ifdef DEBUG            
            printf("Debug - service: %d\n", service);
#endif            
            printf("Enter new user phone number \"cellular\": ");
            scanf("%s",numBuf);
            add_to_list(list, type, nameBuf, numBuf, NULL, &service);
        }
        else
        {
            printf("Invalid choose ! \n");
        }
    }
    return 0;
}

tUserHead* initial_user_list()
{
    tUserHead *list;
    list = (tUserHead *) malloc (sizeof(tUserHead));

    if(list)
    {
        list->counts = 0;
        list->head = NULL;
        list->tail = NULL;
    }
    
    return list;
}

void print_user_info(tUserHead *list)
{
    tUser *node_ptr;
    int userCnt = 1;
    printf("\n");
    printf("users counts: %d \n", list->counts);
    node_ptr = list->head;
    while(node_ptr != NULL)
    {
        printf("---------------user %d---------------\n",userCnt);
        printf("User name: %s \n", node_ptr->name);
        if(node_ptr->phoneType == 1)
        {
            printf("User phone number (home): %s-%s \n", node_ptr->phoneNumber.home.areaCode, node_ptr->phoneNumber.home.number);
        }
        else if (node_ptr->phoneType == 2)
        {
            printf("User phone number (cellular): %s ,(Service provider: ", node_ptr->phoneNumber.cellular.number);
            switch (node_ptr->phoneNumber.cellular.provider)
            {
                case CHT:
                    printf("CHT");
                    break;
                case FET:
                    printf("FET");
                    break;
                case TWN:
                    printf("TWN");
                    break;
                default:
                    printf("Unknown");
            }
            printf(" )\n");
#ifdef DEBUG            
            printf("Debug - provider: %d\n", node_ptr->phoneNumber.cellular.provider);
#endif
        }
        userCnt++;
        node_ptr = node_ptr->next;
    }
    printf("\n\n");
}

void add_to_list(tUserHead *list, int type, char *name, char *number, char *areaCode, int *service)
{
    tUser *newNode;
    newNode = (tUser *) malloc (sizeof(tUser));
    strcpy(newNode->name,name);
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->phoneType = type;
    if(type == 1)
    {
        strcpy(newNode->phoneNumber.home.areaCode,areaCode);
        strcpy(newNode->phoneNumber.home.number,number);
    }
    else if(type == 2)
    {
        newNode->phoneNumber.cellular.provider = *service;
        strcpy(newNode->phoneNumber.cellular.number,number);
#ifdef DEBUG            
        printf("Debug - add_to_list provider: %d\n", newNode->phoneNumber.cellular.provider);
        printf("Debug - add_to_list service: %d\n",*service);
#endif
    }

    if(list->counts == 0)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else 
    {
        tUser *now = list->head;
        while(now->next != NULL)
        {
            now = now->next;
        }
        now->next = newNode;
        newNode->prev = now;
        list->tail = newNode;
    }
    list->counts++;
    print_user_info(list);
}