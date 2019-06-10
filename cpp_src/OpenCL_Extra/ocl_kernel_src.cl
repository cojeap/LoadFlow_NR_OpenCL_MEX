__kernel void pf_newton_raphson_init(
        __global double *in,
        __global double *out) {

    int col = get_global_id(0);
    int row = get_global_id(1);
    int gsize = get_global_size(0);

    int i = row;
    int gidx = (row * gsize + col);

    if (i == 0 || i == 1) {
        out[gidx] = (in[gidx]);
    }
    if (i == 3 || i == 4 || i == 5 || i == 6 || i == 8 || i == 9) {
        out[gidx] = in[gidx] / 100.0;
    }
    if (i == 2 || i == 7) {
        out[gidx] = in[gidx] / in[2 * gsize + col];
    }
    if (i == 10) {
        out[gidx] = (in[3 * gsize + col] - in[5 * gsize + col]) / 100;
    }
    if (i == 11) {
        out[gidx] = (in[4 * gsize + col] - in[6 * gsize + col]) / 100;
    }
    barrier(CLK_GLOBAL_MEM_FENCE);
}

__kernel void pf_pf_newton_raphson_calculate_powers(
    __global double * voltage,
    __global double * angle,
    __global double * real,
    __global double * imag,
    __global double * activePow,
    __global double * reactivePow
){
    int col = get_global_id(0);
    int gsize = get_global_size(0);

    activePow[col] = 0.0;
    reactivePow[col] = 0.0;

    for(int i = 0; i< gsize; i++){
        double teta = angle[col]-angle[i];
        activePow[col] += voltage[col]* voltage[i] * ( real[col+i*gsize]*cos(teta)+imag[col+i*gsize]*sin(teta) );
        reactivePow[col] += voltage[col]* voltage[i] * ( real[col+i*gsize]*sin(teta)-imag[col+i*gsize]*cos(teta) );
    }



}

__kernel void pf_newton_raphson_H_K_M_L(
    __global double * nodeType,
    __global double * powerFlowData,
    __global double * real,
    __global double * imag,
    __global double * rowSize,
    __global double * H,
    __global double * K,
    __global double * M,
    __global double * L
){
    int idx = get_global_id(0);
}

__kernel void pf_newton_raphson_error_compute(
    __global double * invJAC,
    __global double * deltaPQ,
    __global double * errorPQ)
{
    int col = get_global_id(0);
    int gsize = get_global_size(0);

    errorPQ[col] = 0.0;

    for(int i=0;i<gsize;i++){
        errorPQ[col] += invJAC[col+i*gsize]*deltaPQ[i];
    }


}



__kernel void matrixInverseJordan(
        __global double *matrix,
        __global double *matrixInverse){

    int col = get_global_id(0);
    int row = get_global_id(1);
    int gsize = get_global_size(0);
    int size = get_global_size(0) / 2;

    matrixInverse[row + col * size] = matrix[row + col * size] / matrix[row + row * size];

    barrier(CLK_GLOBAL_MEM_FENCE);

    int row_idx, col_idx, curr_col;
    for (curr_col = 0; curr_col < size; curr_col++) {
        for (row_idx = 0; row_idx < size; row_idx++) {
            for (col_idx = 0; col_idx < gsize; col_idx++) {
                if (row_idx == curr_col) {
                    matrixInverse[row_idx + col_idx * size] =
                            matrixInverse[row_idx + col_idx * size] / matrixInverse[curr_col + curr_col * size];
                } else {
                    double ratio = matrixInverse[row_idx + curr_col * size];
                    matrixInverse[row_idx + col_idx * size] =
                            matrixInverse[row_idx + col_idx * size] - ratio * matrixInverse[curr_col + col_idx * size];
                }
                barrier(CLK_GLOBAL_MEM_FENCE);
            }
            barrier(CLK_GLOBAL_MEM_FENCE);
        }
        barrier(CLK_GLOBAL_MEM_FENCE);
    }
}
