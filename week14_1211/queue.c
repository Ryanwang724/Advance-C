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
    int row,location;    //將target->location(0~NUM_BYTE_BUF)轉成row跟location
    int rows = NUM_BYTE_BUF / 8 + 1;
    location = target->location % 8;
    row = rows - (target->location / 8) - 1;

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

    clear_bit(byte_buf_mask, row, location, data_type);
}

tQueueNode *find_target_node(tQueue *queue, int id)
{
    int i;
    tQueueNode *target = queue->front;
    while(target != NULL)
    {
        if(target->id == id)
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
        printf ("%d(%d, %d) ", target->id, target->location, target->data_type); // id location size
        target = target->next;
    }
    printf("\n\n");
}