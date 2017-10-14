//
//  xor.c
//
//  EPITA-OCR
//
//  Created by Ben Taarit Ilyes on 10/04/2017.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Rand() ((double)rand()/((double)RAND_MAX+1))
#define Pattern_num 4
#define Inputs_num 2
#define Hidden_Lay_num 2
#define Output_number 1

double sigmoid(double x)
{
    return (1.0/(1.0+exp(-x)));
}

double derivative(double x)
{
    
    return x*(1.0-x);
}

int main(){
    
    double Inputs[Pattern_num+1][Inputs_num+1] =
    {  {0, 0, 0},
       {0, 0, 0},
       {0, 1, 0},
       {0, 0, 1},
       {0, 1, 1}
                };
    double Target[Pattern_num+1][Output_number+1] = {
        {0, 0},
        {0, 0},
        {0, 1},
        {0, 1},
        {0, 0}
    };
    double pat_ran[Pattern_num+1];
    double SumH[Pattern_num+1][Hidden_Lay_num+1];
    double W_IH[Inputs_num+1][Hidden_Lay_num+1];
    double Hidden[Pattern_num+1][Hidden_Lay_num+1];
    double D_O[Output_number+1];
    double SumDOW[Hidden_Lay_num+1];
    double D_H[Hidden_Lay_num+1];
    double D_W_IH[Inputs_num+1][Hidden_Lay_num+1];
    double D_HO[Hidden_Lay_num+1][Output_number+1];
    double SumO[Pattern_num][Output_number+1];
    double W_HO[Hidden_Lay_num +1][Output_number+1];
    double Output[Pattern_num+1][Output_number+1];
    double n = 0.5;//learning rate
    double a = 0.9;//Momentum : weight change of theprevious steps
    double s_wt = 0.5;
    int np;//patern index
    int p;
    double op;
    double err;
    size_t epoch;
    
    /*Initialisations of Weigths Inputs and D_Inputs*/
    
    for (size_t j = 1; j<=Hidden_Lay_num; ++j) {
        for (size_t i = 0; i<=Inputs_num; ++i) {
            
            D_W_IH[i][j]=0.0;
            W_IH[i][j]=2.0*(Rand()-0.5)*s_wt;
        }
    }
    /*Same things for the Outputs...*/
    for (size_t k = 1; k<=Output_number; ++k) {
        for (size_t j = 0;j <= Hidden_Lay_num;++j)
        {
            D_HO[j][k] = 0.0;
            W_HO[j][k] = 2.0*(Rand()-0.5)*s_wt;
        }
    }
    /*Learning process*/
    for ( epoch = 0 ; epoch <100000; ++epoch) {
        
        for ( p = 1 ; p<=Pattern_num; ++p) {
            
            pat_ran[p] = p;
        }
        for ( p = 1 ; p<=Pattern_num; ++p) {
            
            np = p+ Rand()*(Pattern_num+1 - p);
            op = pat_ran[p];
            pat_ran[p] = pat_ran[np];
            pat_ran[np]=op;
        }
        err = 0.0;
        for (np = 1; np<=Pattern_num; np ++) {
            
            p = pat_ran[np];
            for (size_t j = 1 ; j<=Hidden_Lay_num; ++j) {
                SumH[p][j] = W_IH[0][j];
                for (size_t i = 1; i<=Inputs_num; ++i) {
                    
                    SumH[p][j]+=Inputs[p][i]*W_IH[i][j];
                }
                Hidden[p][j]=sigmoid(SumH[p][j]) ;
            }
            
            
            for (size_t k = 1; k<=Output_number; ++k) {
                SumO[p][k] = W_HO[0][k];
                for (size_t j = 1; j<=Hidden_Lay_num; ++j) {
                    SumO[p][k]+= Hidden[p][j]*W_HO[j][k];
                }
                Output[p][k] =sigmoid(SumO[p][k]);
                err += (Target[p][k] - Output[p][k])*
		(Target[p][k]-Output[p][k])/2.0;
                D_O[k]=(Target[p][k]-Output[p][k])*derivative(Output[p][k]);
            }
            
            
            /*BackPropagation ....*/

            for (size_t j = 1 ; j<=Hidden_Lay_num; ++j) {
                SumDOW[j]=0.0;
                
                for (size_t k = 1; k<= Output_number; ++k) {
                    SumDOW[j] += W_HO[j][k]*D_O[k];
                }
                D_H[j]=SumDOW[j]*derivative(Hidden[p][j]);
            }
            
            
            /*Weigths Updates in outputs hidden layers*/
            for (size_t j = 1 ; j<=Hidden_Lay_num; ++j) {
                D_W_IH[0][j] =n*D_H[j]+a*D_W_IH[0][j];
                W_IH[0][j] += D_W_IH[0][j];
                
                for (size_t i =1; i<=Inputs_num; ++i) {
                    D_W_IH[i][j]= n*Inputs[p][i] * D_H[j] + a*D_W_IH[i][j];
                    W_IH[i][j]+=D_W_IH[i][j];
                }
            }
            for (size_t k = 1 ; k<=Output_number; ++k) {
                D_HO[0][k] =n*D_O[k]+a*D_HO[0][k];
                W_HO[0][k] += D_HO[0][k];
                for (size_t j =1; j<=Hidden_Lay_num; ++j) {
                    D_HO[j][k]= n*Hidden[p][j] * D_O[k] + a*D_HO[j][k];
                    W_HO[j][k]+=D_HO[j][k];
                }
            }
            if(epoch%1000 == 0) {
	    	printf("\nEpoch %zu : Error = %f", epoch, err);
	    }
            if( err < 0.0004 ) break ;  /* stop learning when it's ok */
        }

    }
    printf("\n\n\n\n\n");
    printf("-------------------BE-G-IN---------------\n");
    printf("INPUT1   XOR INPUT2    ==> RESULT   \n\n");
    
    
    printf("%f XOR %f ==> %f , expected  : %f \n" ,
        Inputs[1][1],Inputs[1][2],Output[1][1],Target[1][1]);
    printf("%f XOR %f ==> %f , expected  : %f \n" ,
        Inputs[2][1],Inputs[2][2],Output[2][1],Target[2][1]);
    printf("%f XOR %f ==> %f , expected  : %f \n" ,
        Inputs[3][1],Inputs[3][2],Output[3][1],Target[3][1]);
    printf("%f XOR %f ==> %f , expected  : %f \n" ,
        Inputs[4][1],Inputs[4][2],Output[4][1],Target[4][1]);
    printf("-------------------E--N--D----------------");
    printf("\n");
    return 1 ;
}
