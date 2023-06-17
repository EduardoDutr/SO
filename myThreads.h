#ifndef MYTHREADS_H
#define MYTHREADS_H
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N 10
#define temporeq 0.0

#include "csvParser.h"

typedef struct _threads_
{
  int* arrayOfThreads;
  pthread_t* normalthread;

} Threads;

typedef struct _normalThreadAttributes_
{
  int* threadPositionInArrayOfThreads;
  Requisition requisition;
  int threadId;
  int howManyTimesThreadWorked;

} NormalThreadAttributes;

void normalThreadRoutine(NormalThreadAttributes* normalThreadAttributes);
void dispatcherRoutine(Threads* threads);

#endif // MYTHREADS_H

