#include <stdio.h>
//#include "NeuralNetwork/neuronet.h"

#ifndef TRAIN_NET_
#define TRAIN_NET_

double *convertfromexemples(char *path);

char *conc(char *str1, char *str2, char *sep);

void parcour(char *Dir, size_t iteration);

#endif //TRAIN_NET_
