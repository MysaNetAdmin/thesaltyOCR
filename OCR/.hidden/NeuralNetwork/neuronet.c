//neuronet.c: neuronal network tools

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neuronet.h"

static double rnd(double a, double b)
{  return rand()/(double)RAND_MAX * (b-a) + a; }

static void makeNeuron(struct neuron *neuro, size_t prevL)
{
  neuro->prevLayer = prevL;
  neuro->links = malloc(sizeof(double) * prevL);
  neuro->state = 1;
printf("    prevL = %zu, links =", prevL);
  for(size_t i = 0; i < prevL; i++)
  {
    neuro->links[i] = rnd(-1, 1);
printf(" %f", neuro->links[i]);
  }
}

static struct neuron *makeLayer(size_t prevL, size_t actualL)
{
  struct neuron *Layer = malloc(sizeof(struct neuron) * actualL);
printf(" Layer size: %zu\n", actualL);
  for(size_t i = 0; i < actualL; i++)
  {
    makeNeuron(Layer + i, prevL);
printf("\n");
  }
  return Layer;
}

struct net* makeNet(size_t nbL, size_t *nbperL)
{
srand(time(NULL));
  struct net *net = malloc(sizeof(struct net));
  net->nbLayer = nbL;
  net->nbPLayer = nbperL;
  net->layers = malloc(sizeof(struct neuron***) * (nbL - 1));
printf("\nnet: \n");

  for(size_t i = 0; i < nbL - 1; i++)
  {
    net->layers[i] = makeLayer(nbperL[i], nbperL[i + 1]);
printf("\n");
  }
  return net;
}

static void freelayer(struct neuron *layer, size_t nbneuron)
{
  for(size_t i = 0; i < nbneuron; i++)
  {
    free(layer[i].links);
  }
  free(layer);
}


void freeNet(struct net *net)
{
  for(size_t i = 1; net->nbLayer > i; i++)
  {
    freelayer(net->layers[i - 1], net->nbPLayer[i]);
  }
  free(net->layers);
  free(net);
}

static double sig(double x)
{  return 1/(1 + exp(-x));  }

static double sigprime(double x)
{  return x * (1 - x);  }

static void _work(struct net *net, double *inputs)
{
  for(size_t neuro = 1; neuro < net->nbPLayer[1]; neuro++)
  {
    net->layers[0][neuro].state = 0;
    for(size_t ii = 0; ii < net->nbPLayer[0]; ii++)
    {  net->layers[0][neuro].state += inputs[ii] * net->layers[0][neuro].links[ii];  }
    net->layers[0][neuro].state = sig(net->layers[0][neuro].state);
//printf("layers[0][%zu].state = %f\n", neuro, net->layers[0][neuro].state);
  }
  for(size_t l = 1; l < net->nbLayer - 1; l++)
  {
    for(size_t neuro = 1; neuro < net->nbPLayer[l + 1]; neuro++)
    {
      net->layers[l][neuro].state = 0;
      for(size_t ii = 0; ii < net->nbPLayer[l]; ii++)
      { net->layers[l][neuro].state += net->layers[l - 1][ii].state * net->layers[l][neuro].links[ii];  }
      net->layers[l][neuro].state = sig(net->layers[l][neuro].state);
//printf("layers[%zu][%zu].state = %f\n", l, neuro, net->layers[l][neuro].state);
    }    
  }
}

void back(struct net *net, double *inputs, double *output, double step)
{
  _work(net, inputs);
//printf("(%f, %f) -> %f == %f\n",inputs[1], inputs[2], output[1], compute(net, inputs));

//printf("deltas = \n");
//calcution of delta for output neuron
  for(size_t j = 1; j < net->nbPLayer[net->nbLayer - 1]; j++)
  {
    struct neuron *x = (net->layers[net->nbLayer - 2] + j);
    x->delta = sigprime(x->state) * (output[1] - x->state);
//printf("  failvalue for : layers[%zu][%zu].delta = %f\n",net->nbLayer - 2, j, x->delta);
  }

//calculation of delta for hiden neurons
  for(size_t i = net->nbLayer - 2; i > 0; i--)
  {
    for(size_t j = 1; j < net->nbPLayer[i]; j++)
    {
      double S = 0;
      for(size_t k = 0; k < net->nbPLayer[i + 1]; k++)
      {
        S += net->layers[i][k].delta * net->layers[i][k].links[j];
      }
      net->layers[i - 1][j].delta = sigprime(net->layers[i - 1][j].state) * S;
//printf("  failvalue for : layers[%zu][%zu].delta = %f\n",i - 1, j, net->layers[i - 1][j].delta);
    }
  }

//printf("\n");
//update of inputs links
  for(size_t j = 1; j < net->nbPLayer[1]; j++)
  {
    for(size_t ii = 0; ii < net->nbPLayer[0]; ii++)
    {
double x = step * net->layers[0][j].delta * inputs[ii];
//printf("layers[0][%zu].links[%zu] = %f + %f \n", j, ii, net->layers[0][j].links[ii], x);
net->layers[0][j].links[ii] += x;
//net->layers[0][j].links[ii] += step * net->layers[0][j].delta * inputs[ii];  
    }
  }

//update of other links
  for(size_t i = 1; i < net->nbLayer - 1; i++)
  {
    for(size_t j = 1; j < net->nbPLayer[i + 1]; j++)
    {
      for(size_t ii = 0; ii < net->nbPLayer[i]; ii++)

      {
double x = step * net->layers[i][j].delta * net->layers[i][ii].state;
//printf("layers[%zu][%zu].links[%zu] = %f + %f \n", i, j, ii, net->layers[i][j].links[ii], x);
net->layers[i][j].links[ii] += x;
//net->layers[i- 1][j].links[ii] += step * net->layers[i - 1][j].delta * net->layers[i - 1][ii].state;
      }
    }
  }
//printf("\n\n");
}

size_t compute(struct net *net, double *inputs)
{
  _work(net, inputs);
  double max = 0;
  size_t res = 0;
  for(size_t i = 1; i < net->nbPLayer[net->nbLayer - 1]; i++)
  {
    if(max < net->layers[net->nbLayer - 2][i].state)
    {  max = net->layers[net->nbLayer - 2][i].state; res = i;  }
  }
  return res;
}


void saveNet(struct net *net, char *path)
{
  FILE *f = fopen(path, "wb");
  if(f)
  {
    fwrite(&(net->nbLayer), sizeof(size_t), 1, f);
    fwrite(net->nbPLayer, sizeof(size_t), net->nbLayer, f);
    for(size_t l = 0; l < net->nbLayer - 1; l++)
    {
      for(size_t neuro = 1; neuro < net->nbPLayer[l + 1]; neuro++)
      {
        fwrite(net->layers[l][neuro].links, sizeof(double), net->nbPLayer[l], f);
      }
    }
  fclose(f);
  }
}

struct net *loadNet(char *path)
{
  FILE *f = fopen(path, "rb");
  if(f)
  {
    struct net *net = malloc(sizeof(struct net));
    if(!fread(&net->nbLayer, sizeof(size_t), 1, f))
    {  return NULL;  }
    net->nbPLayer = malloc(sizeof(size_t) * net->nbLayer);
    if(!fread(net->nbPLayer, sizeof(size_t), net->nbLayer, f))
    {  return NULL;  }
    net->layers = malloc(sizeof(struct net***) * (net->nbLayer - 1));
    for(size_t l = 0; l < net->nbLayer - 1; l++)
    {
      net->layers[l] = malloc(sizeof(struct neuron) * net->nbPLayer[l + 1]);
      net->layers[l][0].links = malloc(sizeof(double));
      net->layers[l][0].state = 1;
      for(size_t neuro = 1; neuro < net->nbPLayer[l + 1]; neuro++)
      {
        net->layers[l][neuro].links = malloc(sizeof(double) * net->nbPLayer[l]);
        if(!fread(net->layers[l][neuro].links, sizeof(double), net->nbPLayer[l], f))
        {  return NULL;  }
      }
    }
    fclose(f);
    return net;
  }
  else
  {  return NULL;  }
}
/*
void backXOR(struct net *net, size_t iteration)
{
  double inA[3] = {1,0,0};
  double inB[3] = {1,0,1};
  double inC[3] = {1,1,0};
  double inD[3] = {1,1,1};

  double outA[2] = {1,0};
  double outB[2] = {1,1};
  double outC[2] = {1,1};
  double outD[2] = {1,0};

printf("0,0 -> %f\n", compute(net, inA));
printf("0,1 -> %f\n", compute(net, inB));
printf("1,0 -> %f\n", compute(net, inC));
printf("1,1 -> %f\n\n", compute(net, inD));

  for(size_t i = 0; i < iteration; i++)
  {
    back(net, inA, outA, 0.4);
    back(net, inB, outB, 0.4);
    back(net, inC, outC, 0.4);
    back(net, inD, outD, 0.4);
  }
printf("0,0 -> %f\n", compute(net, inA));
printf("0,1 -> %f\n", compute(net, inB));
printf("1,0 -> %f\n", compute(net, inC));
printf("1,1 -> %f\n", compute(net, inD));
}*/
