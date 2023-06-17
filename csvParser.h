#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pi.h"


typedef struct _requisition_
{
  int quantity;
  int delay;

} Requisition;

int readRequisitionFromCsv(FILE* file,Requisition* requisition);
void writeResultInFile(int id, int howManyTimesThreadWorked, int quantity, int delay, char* piDigits);
void createRandomRequisitionsInFile(int quantityOfRequisitions);

#endif // CSVPARSER_H

