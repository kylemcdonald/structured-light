/* 
 * File:   RPixelQueue.h
 * Author: jenicek
 *
 * Created on July 29, 2010, 11:05 AM
 */

#ifndef _RPIXELQUEUE_H
#define	_RPIXELQUEUE_H

#include "Global.h"

// Queue element structure
typedef struct pqElement
{
    CvPoint point;
    struct pqElement *next;
}T_PQElem;

// Queue structure
typedef struct pixelQueue
{
    T_PQElem *head;
    T_PQElem *tail;
    long maxElems;
    long numElems;
}T_PixelQueue;

// Initialize queue
void PQInit(T_PixelQueue *pq, long maxElems);

// Insert item into the queue
int PQAdd(T_PixelQueue *pq, CvPoint point);

// Remove intem from the queue
int PQRemove(T_PixelQueue *pq, CvPoint *point);

// Check whether the queue is empty
int PQIsEmpty(T_PixelQueue *pq);

#endif	/* _RPIXELQUEUE_H */

