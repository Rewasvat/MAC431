__kernel void MatrixShift(__global const float *dM, __global const float *dR, __global int *sRow, __global int *sCol, __global int *numRows, __global int *numCols) {
 
    unsigned int i = get_global_id(0);
    unsigned int j = get_global_id(0);

	int rows = *numRows;
	int cols = *numCols;
 
	unsigned int a = (i - (*sRow)) % rows;
	unsigned int b = (j + (*sCol)) % cols;

    dR[a*cols + b] = dM[i*cols + j];

	/*row shift*/
	/*depois column-shift*/
}
