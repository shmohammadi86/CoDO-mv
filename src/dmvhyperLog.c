#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>
#include "mvhyper.h"
void C_dmvhyper_logVal(int *x, int *nL, int *L, int *n, double *p, int *logp, double *logVal){
/*
x:     number of elements overlap between all subsets
nL:    number of subsets
L:     subset sizes
n:     background size
p:     output probability
logp:  return log probability
*/
	
	//printf("\tC_dmvhyper_logVal:: x = %d, nL = %d, L[0] = %d, L[1] = %d, n = %d\n", *x, *nL, L[0], L[1], *n);

	int i, j, k, l;
	int i0=0;
	int aSize=maxArray(L,*nL) - *x + 1;

	double *f0 = (double *)calloc(aSize, sizeof(double));
	if(f0 == NULL) {
		fprintf(stderr, "dmv: f0 memory allocation failed.\n"); fflush(stdout);
	}
	double *f1 = (double *)calloc(aSize, sizeof(double));
	if(f1 == NULL) {
		fprintf(stderr, "dmv: f1 memory allocation failed.\n"); fflush(stdout);
	}

	double temp;

	int minL=minArray(L,*nL);

	if(*nL == 2){
		*p=C_dhyper_logVal(*x, L[0],*n - L[0],L[1],*logp,logVal);
		free(f0);
		free(f1);		
		return;
	}
	
	*p=0;
	//from inner-most to outer-most
	for(i=1; i <= *nL - 1; i++){
		if(i==1){
			l = *x;
			f1[0]=C_dhyper_logVal(*x,l,*n - l,L[*nL - 1],i0,logVal);
			for(l= *x +1; l <= min2(minL, *n + *x -L[*nL - 1]); l++){
				f1[l- *x] = f1[l- *x -1] * ((double)(*n - l+1-L[*nL - 1] + *x)/(double)(l - *x))  * ((double)l/(double)(*n - l+1));
			}
			continue;
		}
		memcpy ( f0, f1, aSize * sizeof(double) );
		if(*nL - i >= 2){
			for(k = *x;k <= minL;k++){ //calculate f_l(k)
				f1[k - *x]=0;
				l=max2(*x, k+L[*nL - i] - *n);
				temp = C_dhyper_logVal(l,L[*nL - i],*n - L[*nL - i],k,i0,logVal);
				f1[k - *x] += temp * f0[l - *x];
				for(l=l+1;l <= k; l++){ //sum over l for each k
					temp = temp * ((double)(L[*nL - i]-l+1)/(double)l) * ((double)(k-l+1) /(double)(*n - L[*nL - i]-k+l));
					f1[k - *x] += temp * f0[l - *x];
				}
			}
			continue;
		}
		//final integration
		j=max2(*x,L[1]+L[0] - *n);
		temp=C_dhyper_logVal(j,L[1],*n - L[1],L[0],i0,logVal);
		*p += temp * f1[j - *x];
		for(j=j+1;j <= minL;j++){
			temp=temp * ((double)(L[1]-j+1)/(double)j) * ((double)(L[0]-j+1) /(double)(*n - L[1]-L[0]+j));
			*p += temp * f1[j - *x];
		}
	}
	
	free(f0);
	free(f1);
	
	if(*logp>0) *p=log(*p);
	return;
}

double C_dhyper_logVal(int x, int w, int b, int n, int logp, double *logVal){
//probability of getting x white balls out of n draws from an urn with w white balls and b black balls
	double result;
	if(x>w || x>n || b+x<n){
		result=0;
		if(logp==1) result=log(result);
	}else{
		result=C_logChoose_logVal(w,x,logVal)+C_logChoose_logVal(b,n-x,logVal);
		result=result-C_logChoose_logVal(w+b,n,logVal);
		if(logp==0) result=exp(result);
	}
	return(result);
}
