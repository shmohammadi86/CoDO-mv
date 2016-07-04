#include <stdio.h>
#include <math.h>
#include "mvhyper.h"

void CoDO(int x, int nL, int *L, int n, int m_overlap, int m_union, double *p){
/*
x:         number of elements overlap between all subsets
nL:        number of subsets
L:         subset sizes
n:         background size
m_overlap: Number of edges in intersection
m_union:   Number of edges in union
p:         output probability
*/
	register int i;
	
	double logVal[n];
	int minL = min(L,nL);	
	for(i = 1; i <= n ; i++){
		logVal[i-1] = log((double)i);
	}
	
	long int total_size = 0;
	for(i = 0; i < nL; i++) {
		total_size += L[i];
	}
	total_size -= (nL-1)*x;
	int total_pairs = (int) ( total_size*(total_size - 1) / 2.0 ) ;
	
	int densityL[2], n_densityL = 2;;
	densityL[0] = m_union; // sample size in hygecdf
	double density_pval;
	int success_size = m_overlap - 1;
	
	int lower = 0, logp = 0;
	double pval = 0.0, p0=0.0;
	
	for(i = x; i <= minL; i++){
		C_dmvhyper_logVal(&i, &nL, L, &n, &p0, &logp, logVal);		
		
		
		if(i <= 1) 
			density_pval = 1;
		else {
			densityL[1] = i*(i-1) / 2; // pos size in hygecdf
			
			// sample size = L[0] = m_union, pos size = L[1] = C(i, 2), success_size = m_overlap - 1
			C_pmvhyper(&success_size, &n_densityL, densityL, &total_pairs, &density_pval, &lower, &logp);			
		}
		
		
		pval += p0*density_pval;		
		if(1 < pval) {
			*p = 1.0;
			return;
		}
	}
	
	(*p) = pval;
}
