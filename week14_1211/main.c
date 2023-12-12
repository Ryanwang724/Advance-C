#include "main.h"
#include "queue.h"
#include "space.h"

int main (void)
{
    tQueue *queue;
    int operation, units;
    tQueueNode *target_node;
    int id, score=0, ret, remain = NUM_BYTE_BUF;
    queue = createQueue();
    create_mask();

    while (1)
    {

        printf("Remaining memory space %d\n",remain);
        printf("\nWhich type you are going to operate? \n");
        printf("1. Add an item\n");
        printf("2. Remove an item with a specific Id\n");
        
        scanf("%d", &operation);
        
        if (operation == 1)
        {
            printf("  Enter id: ");
            scanf("%d", &id);
            printf("  specify data type (units) you want: ");
            scanf("%d", &units);

            ret = enqueue_node(queue, id, score, units);

            if (ret == 0)
            {
                printf("    Cannot enter to the queue\n");
            }
            print_buffer_status();
        }
        else if (operation == 2)
        {
            printf ("  Enter an ID to remove ");
            scanf("%d", &id);
            target_node = find_target_node(queue, id, (operation == 3 ? TYPE_SMALL : TYPE_LARGE));
            if (target_node == NULL)
            {
                printf ("    Cannot find the target node \n");
            }
            else
            {
                dequeue_node(queue, target_node, (operation == 3 ? TYPE_SMALL : TYPE_LARGE));
            }
            print_buffer_status();

        }
        else 
        {
            printf ("    No such operation \n");   
        }
        print_queue(queue);
    }
    free_mask();
}