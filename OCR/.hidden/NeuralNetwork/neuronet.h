//neuronet.h: neuronal network tools

# ifndef NET_NEURO_
# define NET_NEURO_

#include <stdlib.h>

//standard neurone
struct neuron
{
  double state;
  double delta;
  size_t prevLayer;
  double *links;
};

//standard neuronal network
struct net
{
  size_t nbLayer;
  size_t *nbPLayer;
  struct neuron **layers;
};

//standard net constructor
struct net* makeNet(size_t nbL, size_t *nbperL);

//name explicite
void freeNet(struct net *net);

//train the neural network
void train(struct net *net);

//compute the output from the inputs given
// 1 must be add at input[0]
size_t compute(struct net *net, double *inputs);

void saveNet(struct net *net, char *path);

struct net *loadNet(char *path);

void back(struct net *net, double *inputs, double *outputs, double step);

//take a net with (2+1) inputs and (1+1) outputs and make a XOR net
void backXOR(struct net *net, size_t iteration);
# endif /* NET_NEURO_ */
