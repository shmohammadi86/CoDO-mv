#include <stdio.h>
#include <math.h>
#include "mvhyper.h"

// You should call this function with x=success-1 to match matlab's syntax
void CoDO(int *x, int *nL, int *L, int *n, double *p, int *m_total, int *m_overlap){
/*
x:         number of elements overlap between all subsets
nL:        number of subsets
L:         subset sizes
n:         background size
p:         output probability
m_total:   Number of edges in union
m_overlap: Number of edges in intersection
*/
	const double tiny = 1.0E-320;
	register int i;
	
	double logVal[*n];
	int minL=min(L,*nL);	
	for(i=1; i<= *n ; i++){
		logVal[i-1]=log((double)i);
	}
	
	int total_pairs = (int) ((*m_total)*(*m_total - 1) / 2.0);
	
	int densityL[2], n_densityL = 2;;
	densityL[0] = *m_total; // sample size in hygecdf
	double density_pval;
	int success_size = *m_overlap - 1;
	
	int lower = 0, logp = 0;
	double pval = 0.0, p0=0.0, p0_last=1.0;
	
	for(i = *x + 1; i <= minL; i++){
		C_dmvhyper_logVal(&i, nL, L, n, &p0, &logp, logVal);
		
		if(p0 <= tiny) break;
		if( (p0/p0_last) <= 0.01 ) break;  //No improve in precision
		p0_last=p0;
	
		if(i <= 1) 
			density_pval = 1;
		else {
			densityL[1] = i*(i-1) / 2; // pos size in hygecdf
			
			// sample size = L[0] = m_total, pos size = L[1] = C(i, 2), success_size = m_overlap - 1
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
