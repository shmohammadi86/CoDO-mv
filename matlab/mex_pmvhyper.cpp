#include <math.h>
#include "mvhyper.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	int i, j;
    int row, col;
	
	if(nrhs != 3) {
		mexErrMsgTxt("Syntax: mex_pmvhyper(set_sizes, overlap_size, population_size)\n");					
		
	}

	if(!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0])) {
		mexErrMsgTxt("List of set sizes should be double\n");
	}

	int m = (int ) mxGetM(prhs[0]);
	int n = (int ) mxGetN(prhs[0]);

	int min_sizes = (m < n? m:n);
	int max_sizes =  (m > n? m:n);
	
	if(min_sizes != 1 || max_sizes < 2) {
		mexErrMsgTxt("List of set sizes should be a vector with size at least 2\n");		
	}	
	mexPrintf("pmvhyper with %d sets\n", max_sizes);

	double *L_dbl = (double *) mxGetPr(prhs[0]);
	int *L = (int *)mxCalloc(max_sizes, sizeof(int));
	for (i = 0; i < max_sizes; i++) {
		L[i] = (int)roundl(L_dbl[i]);
	}
	
	for (i = 0; i < max_sizes; i++) {
		mexPrintf("\tSet %d size = %d\n", i, L[i]);
	}

	
	if( !mxIsDouble(prhs[1]) || mxIsComplex(prhs[1]) ) {
		mexErrMsgTxt("overlap_size should be an integer\n");					
	}
	int overlap_size = (int ) mxGetScalar(prhs[1]);
	int x = overlap_size - 1;
	mexPrintf("Overlap size = %d\n", overlap_size);
		
	
	if( !mxIsDouble(prhs[2]) || mxIsComplex(prhs[2]) ) {
		mexErrMsgTxt("population size should be an integer\n");					
	}
	int pop_size = (int ) mxGetScalar(prhs[2]);
	mexPrintf("population size = %d\n", pop_size);
	

	double pval=1;
	int lower = 0;
	int logp = 0;
	C_pmvhyper(&x, &max_sizes, L, &pop_size, &pval, &lower, &logp);
	
	mexPrintf("PRAY!\n");
	if(pval > 1) pval = 0; // Overflow!
	plhs[0] = mxCreateDoubleScalar(pval);	
	/*plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	*mxGetPr(plhs[0]) = pval;*/
	mxFree(L);
}
