# include "RPixelQueue.h"

// Initialize queue
void PQInit(T_PixelQueue *pq, long maxElems)
{
    pq->maxElems = maxElems;
    pq->numElems = 0;
    pq->head = pq->tail = NULL;
}

// Insert item into the queue
int PQAdd(T_PixelQueue *pq, CvPoint point)
{
    // Check whether the queue is full or not
    if(pq->numElems < pq->maxElems)
    {
        // Another element
        pq->numElems++;

        // Allocate memory for the new element
        T_PQElem *newElem = (T_PQElem*)malloc(sizeof(T_PQElem));
        if(newElem == NULL)
            return 0;
        // Fill new item with values
        newElem->point = point;
        newElem->next = NULL;

        if(pq->head == NULL)
            pq->head = newElem;
        else
            pq->tail->next = newElem;
        pq->tail = newElem;

        return 1;
    }

    return 0;
}

// Remove intem from the queue
int PQRemove(T_PixelQueue *pq, CvPoint *point)
{
    if(pq->head != NULL)
    {
        // Get the item from the queue
        (*point) = pq->head->point;
        pq->numElems--;
        // Remove the element from the queue
        T_PQElem *tmp = pq->head;
        pq->head = pq->head->next;
        free(tmp);
        if(pq->numElems == 0)
            pq->tail = NULL;

        return 1;
    }

    return 0;
}

// Check whether the queue is empty
int PQIsEmpty(T_PixelQueue *pq)
{
    if((pq->head == pq->tail) && (pq->head == NULL))
        return 1;

    return 0;
}

