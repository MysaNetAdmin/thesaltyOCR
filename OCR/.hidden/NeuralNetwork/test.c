#include <stdio.h>
#include "neuronet.h"

int main()
{
  size_t weight[3] = {3, 8, 2};
  struct net *net = makeNet(3, weight);

//double in[3] = {1,0,0};
//printf("res = %f\n", compute(net, in));

  backXOR(net, 10000);
  saveNet(net, "net.bin");
  freeNet(net);

printf("\n\nDONNEE PERDU OMG!\n\n");

  net = loadNet("net.bin");
  backXOR(net, 0);

  freeNet(net);
  return 0;
}
