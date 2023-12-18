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

int tqueue_enqueue(tQueue *queue, int id, int score, int type)
{
    tQueueNode *queue_node;
    void *newptr = NULL;
    int mem_location;

    queue_node = (tQueueNode *)malloc(sizeof(tQueueNode));
    our_malloc (type, (void *)&newptr, &mem_location);

    if (newptr == NULL)
    {
        printf("    Enqueue Failed!!! \n");
        return 0;
    }

    queue_node->next = NULL; //set the value
    queue_node->prev = NULL;
    queue_node->type = type;
    queue_node->content = newptr;

    if(type == TYPE_SMALL)
    {
        ((tQueueSmall *)newptr)->id = id;
        ((tQueueSmall *)newptr)->location = mem_location;
        ((tQueueSmall *)newptr)->score = score;
    }
    else if(type == TYPE_LARGE)
    {
        ((tQueueLarge *)newptr)->id = id;
        ((tQueueLarge *)newptr)->location = mem_location;
        ((tQueueLarge *)newptr)->score[0] = score;
    }

    if(queue->count == 0)
    {
        queue->front = queue_node;
        queue->rear = queue_node;
    }
    else
    {
        queue->rear->next = queue_node;
        queue_node->prev = queue->rear;
        queue->rear = queue_node;
    }
    queue->count++;
    return 1;
}

void dequeue_node(tQueue *queue, tQueueNode *target, int type)
{
    if(type == TYPE_SMALL)
    {
        printf("target type: %d, location: %d, id: %d\n",target->type, ((tQueueSmall *)target->content)->location, ((tQueueSmall *)target->content)->id);
    }
    else if(type == TYPE_LARGE)
    {
        printf("target type: %d, location: %d, id: %d\n",target->type, ((tQueueLarge *)target->content)->location, ((tQueueLarge *)target->content)->id);
    }

    if(target->next != NULL && target->next->type == TYPE_SMALL)
    {
        printf("target next type: %d, location: %d, id: %d\n",target->next->type, ((tQueueSmall *)target->next->content)->location, ((tQueueSmall *)target->next->content)->id);
    }
    else if(target->next != NULL && target->next->type == TYPE_LARGE)
    {
        printf("target next type: %d, location: %d, id: %d\n",target->next->type, ((tQueueLarge *)target->next->content)->location, ((tQueueLarge *)target->next->content)->id);
    }

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
    if(type == TYPE_SMALL)
    {
        our_free(type, ((tQueueSmall *)target->content)->location);
    }
    else if(type == TYPE_LARGE)
    {
        our_free(type, ((tQueueLarge *)target->content)->location);
    }
}

tQueueNode *find_target_node(tQueue *queue, int id, int type)
{
    int i;
    tQueueNode *target = queue->front;
    if(type == TYPE_SMALL)
    {
        while(target != NULL)
        {
            if(((tQueueSmall *)target->content)->id == id)
            {
                return target;
            }
            target = target->next;
        }
    }
    else if(type == TYPE_LARGE)
    {
        while(target != NULL)
        {
            if(((tQueueLarge *)target->content)->id == id)
            {
                return target;
            }
            target = target->next;
        }
    }
    return NULL;
}


void print_queue (tQueue *queue)
{
    int i;
    tQueueNode *target = queue->front;

    printf("      type mixed queue: ");    
    for (i = 0; i < queue->count; i++)
    {
        if(target->type == TYPE_SMALL)
        {
            printf ("%d,%d(%d,%d) ", ((tQueueSmall *)target->content)->id, ((tQueueSmall *)target->content)->score, target->type, ((tQueueSmall *)target->content)->location);
        }
        else if(target->type == TYPE_LARGE)
        {
            printf ("%d,%d(%d,%d) ", ((tQueueLarge *)target->content)->id, ((tQueueLarge *)target->content)->score[0], target->type, ((tQueueLarge *)target->content)->location);
        }
        target = target->next;
    }
    printf("\n");
}