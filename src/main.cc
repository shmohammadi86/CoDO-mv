#include <stdio.h>

void CoDO(int x, int nL, int *L, int n, int m_overlap, int m_union, double *p);

int main() {
	const int nZ = 6, nA = 59, nB = 26, n = 334;
	const int mU = 1137, mZ = 9;
	int L[2] = {nA, nB};
	
	
	double pval;
	CoDO(nZ, 2, L, n, mZ, mU, &pval);
	
	printf("Overlap size = %d, A Size = %d, B Size = %d, n = %d\n", nZ, nA, nB, n);
	printf("Overlap edges = %d, union edges = %d\n", mZ, mU);
	printf("pval = %e\n", pval);
	
	return 0;	
}
