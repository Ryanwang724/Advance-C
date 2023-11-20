#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define N 5

typedef struct Score
{   
    int score;
    int loc;
    int used;      // to record if the space is used    
}tTypeScore;

typedef struct node
{
    tTypeScore *data_ptr;
    struct node *next;
}tNode;

typedef struct nodeInfo
{
    int count;
    tNode *head;
}tStack;

void get_score_space (tTypeScore **pp_score); //get the space from score_buf, return the allocated number
void return_score_space (int location);  //return the space of the used score_buf
tStack *create_stack(void);  //Create a stack
void handle_push_operation (tStack *stack_ptr); //push a data_ptr to a specific stack
void handle_pop_operation (tStack *stack_ptr); //pop a specific stack and return a void* data_ptr
void print_stack_content(tStack *stack_ptr); //print all content in stack
tTypeScore score_buf[N]; //preallocated memory space
int main(void) {

    tStack *stack_ptr = create_stack();
    int choose;

    while (1)
    {
        //menu 
        printf("\n(1) push or (2) pop a item to/from stack: ");
        scanf("%d", &choose);

        if (choose == 1)
        {
            handle_push_operation(stack_ptr);
        }
        else if (choose == 2)
        {
            handle_pop_operation(stack_ptr);
        }
        print_stack_content(stack_ptr);
    }
    
    return 0;
}

tStack *create_stack(void)
{
    tStack *stackManager;
    stackManager = (tStack* ) malloc (sizeof(tStack));
    stackManager->count = 0;
    stackManager->head = NULL;
    return stackManager;
}

void handle_push_operation(tStack *stack_ptr)
{
    int score;
    tNode *node;
    node = (tNode* ) malloc (sizeof(tNode));
    node->data_ptr = NULL;
    node->next = NULL;

    if(stack_ptr->count == N)
    {
        printf ("  [Error]  handle_push_operation(): space full \n");
        return;
    }
    printf("  handle_push_operation(): enter score value: ");
    scanf("%d", &score);
        
    get_score_space(&node->data_ptr);
    node->data_ptr->score = score;

    if(stack_ptr->count == 0)
    {
        stack_ptr->head = node;
    }

    tNode *ptr = stack_ptr->head;
    while(ptr->next != NULL) 
    {
        ptr = ptr->next;
    }
    ptr->next = node;
    stack_ptr->count++;
    return; 
}

void handle_pop_operation(tStack *stack_ptr)
{
    if(stack_ptr->count == 0)
    {
        printf ("  [Error]  handle_pop_operation(): nothing in stack\n");
        return;
    }
    tNode *ptr = stack_ptr->head;
    while(ptr->next != NULL) 
    {
        ptr = ptr->next;
    }
	printf("  handle_pop_operation(): poped value: %d\n",ptr->data_ptr->score);
	return_score_space(ptr->data_ptr->loc);
}

void print_stack_content(tStack *stack_ptr)
{
    tNode *ptr = stack_ptr->head;
    int i;
    printf("   print_stack_content(): stack items -> ");

    for(i=stack_ptr->count;i>0;i--)
    {
        printf(" %d(%d)",ptr->data_ptr->score,ptr->data_ptr->loc);
        ptr = ptr->next;
    }
    printf("\ntest\n");
}

void get_score_space(tTypeScore **pp_score)
{
    int i;
    for(i=0;i<N;i++)
    {
        if(score_buf[i].used != 1)  //0 unused; 1 used;
        {
            *pp_score = &score_buf[i];
            printf("     get_score_space(): giving space numbered %d\n",i);
            score_buf[i].used = 1;
            score_buf[i].loc = i;
            return;
        }
    }
}

void return_score_space (int loc)
{
    score_buf[loc].used = 0;
    printf("     return_score_space(): return space numbered %d\n", loc);
}