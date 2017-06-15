double **resize(int** mat ,size_t height,size_t width)
{
    double ** res = malloc(20*sizeof(double*));
    for (size_t i =0; i<20; i++) {
        res[i]=malloc(20*sizeof(double));
    }
    for(size_t j=0 ;j<20;j++)
    {
        for (size_t k=0 ;k<20;k++) {
            res[j][k]=(double) (mat[(i*height)/20][(j*width)/20]);
        }
    }
    
    return res ;
}
