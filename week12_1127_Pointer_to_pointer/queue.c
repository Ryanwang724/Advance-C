#include "queue.h"
#include "space.h"


tQueue* createQueue(void){    
    tQueue *queue;
    queue=(tQueue *) malloc (sizeof(tQueue));

    if (queue)
    {
        queue->front=NULL;
        queue->rear=NULL;  
        queue->count=0;
    }

    return queue;
}

int enqueue_node(tQueue *queue, int id, int score, int data_type)
{
    tQueueNode *newptr = NULL;
    int mem_location;

    our_malloc (data_type,(void **)&newptr, &mem_location);
    
    if (newptr == NULL)
    {
        printf("    Enqueue False!!! \n");
        return 0;
    }

    newptr->next = NULL; //set the value
    newptr->prev = NULL;
    newptr->data_type = data_type;
    newptr->id = id;
    newptr->location = mem_location;

    if(queue->count == 0)
    {
        queue->front = newptr;
        queue->rear = newptr;
    }
    else
    {
        tQueueNode *ptr = queue->front;
        while(ptr->next != NULL)
        {
            ptr = ptr->next;  //go to target location
        }
        ptr->next = newptr;
        newptr->prev = ptr;
        queue->rear = newptr;
    }
    queue->count++;
    return 1;
}

void dequeue_node(tQueue *queue, tQueueNode *target, int data_type)
{
    if(queue->count == 1)
    {
        queue->front = NULL;
        queue->rear = NULL;
    }
    else if(target->next == NULL) //last one node
    {
        queue->rear = target->prev;
        target->prev->next = NULL;
    }
    else if(target == queue->front) //first node
    {
        queue->front = target->next;
        target->next->prev = NULL;
    }
    else   //middle node
    {
        target->next->prev = target->prev;
        target->prev->next = target->next;
    }
    target->next = NULL;
    target->prev = NULL;

    queue->count--;
    our_free(data_type, target->location);
}

tQueueNode *find_target_node(tQueue *queue, int id, int data_type)
{
    int i;
    tQueueNode *target = queue->front;
    while(target != NULL)
    {
        if(target->id == id && target->data_type == data_type)
        {
            return target;
        }
        target = target->next;
    }
    return NULL;
}


void print_queue (tQueue *queue)
{
    int i;
    tQueueNode *target = queue->front;

    printf("      queue content: ");    
    for (i = 0; i < queue->count; i++)
    {
        printf ("%d(%d) ", target->id, target->data_type);
        target = target->next;
    }
    printf("\n");
}