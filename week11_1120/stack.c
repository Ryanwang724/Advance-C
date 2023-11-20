#include "stack.h"

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
    else
    {
        tNode *ptr = stack_ptr->head;
        while(ptr->next != NULL) 
        {
            ptr = ptr->next;
        }
        ptr->next = node;
    }
    stack_ptr->count++;
    //free(ptr);
    return; 
}

void handle_pop_operation(tStack *stack_ptr)
{
    if(stack_ptr->count == 0)
    {
        printf ("  [Error]  handle_pop_operation(): nothing in stack\n");
        return;
    }
    else if(stack_ptr->count == 1)
    {
        printf("  handle_pop_operation(): poped value: %d\n",stack_ptr->head->data_ptr->score);
        return_score_space(stack_ptr->head->data_ptr->loc);
        free(stack_ptr->head);
        stack_ptr->head = NULL;
    }
    else
    {
        tNode *ptr = stack_ptr->head;
        while(ptr->next != NULL && ptr->next->next != NULL) 
        {
            ptr = ptr->next;
        }
        printf("  handle_pop_operation(): poped value: %d\n",ptr->next->data_ptr->score);
        return_score_space(ptr->next->data_ptr->loc);
        free(ptr->next); // 釋放彈出節點的內存
        ptr->next = NULL; // 更新倒數第二個節點的next指針
    }
    stack_ptr->count--;
    //free(ptr);
}

void print_stack_content(tStack *stack_ptr)
{
    tNode *ptr = stack_ptr->head;
    int i,j;
    printf("\n   print_stack_content(): stack items -> ");

    for(i=stack_ptr->count;i>0;i--) 
    {
        for(j=1;j<i;j++)
        {
            ptr = ptr->next;  //go to target location
        }
        printf(" %d(%d)",ptr->data_ptr->score,ptr->data_ptr->loc);
        ptr = stack_ptr->head;  //reset ptr
    }
    printf("\n");
    //free(ptr);
}

