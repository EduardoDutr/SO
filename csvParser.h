#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pi.h"
// Declaração avançada da estrutura NormalThreadAttributes
struct _normalThreadAttributes_;
typedef struct _normalThreadAttributes_ NormalThreadAttributes;

typedef struct _requisition_
{
  int quantity;
  int delay;

} Requisition;

int readRequisitionFromCsv(FILE* file,Requisition* requisition);
void writeResultInFile(NormalThreadAttributes* normalThreadAttributes, char* piDigits);
void createRandomRequisitionsInFile(int quantityOfRequisitions);

#endif // CSVPARSER_H

