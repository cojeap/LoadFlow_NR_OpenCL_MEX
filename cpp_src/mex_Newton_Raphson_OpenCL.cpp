/*
 * Created by Petru on 13.04.2019.
 *
 * Due to the way Matlab responds to thrown exceptions a NO_THROW_EXCEPTION policy is used in user created objects only.
 * A device with double floating-point precision is expected:  [cl_khr_fp64] ! Need to implement a check
 *
 * Kernels have a given id hardcoded as follows: -66. <--- not needed anymore. need to remove
 *
 * init  ID=0;
 *
 *
 * There are issues (could be memory leaks?) due to not cleaning mxArrays during the running of this program. to be done later
 * Make a matlab obj mem manager??? ->> later priority
 *
 * Need to place a check so that if an user adds more buffers to the kernel that their num of args it will return an error(for now it silently fails and gives either erred output or none at all)
 * Need to implement an error enum class to ease debugging
 *
 *
 * implement aditional overloads for functions that only work with kernel id
 */
#include "mex_Newton_Raphson_OpenCL.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
//mxArray * pOutArgs,const mxArray * pInArgs)
{
    //misc variables
    std::string msg;

    //catch input error here
    if (nrhs != 19) {
        msg = "Expected number of input arguments does not match!";
        return;
    }

    msg = "MEX Start!";


    std::vector<std::vector<double> > input;
    {
        for (auto i = 0; i < nrhs; i++) {
            std::vector<double> tempvctr;
            auto row = mxGetM(prhs[i]); //row in array
            auto col = mxGetN(prhs[i]);
            auto to{row};
            if (row >= col)
                to = row;
            else
                to = col;
            for (auto j = 0; j < to; j++) {
                tempvctr.emplace_back(mxGetPr(prhs[i])[j]);
            }
            input.emplace_back(tempvctr);
        }
    }

    msg = "GPU compute return!";

    std::vector<E_Matlab_Data_Structure> output;
    auto err = main(input,output);
    if(err!=0){
        msg = "Error!";
        return;
    };

    if (output.empty()) {
        msg = "Forced Stop! Received output is empty!";
        return;
    }

    //output
    auto col = output[0].colSize;
    auto row = output[0].rowSize;

    mxArray *out0 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[0].data.begin(), output[0].data.end(), mxGetPr(out0));

    plhs[0] = out0;

     col = output[1].colSize;
     row = output[1].rowSize;

    mxArray *out1 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[1].data.begin(), output[1].data.end(), mxGetPr(out1));

    plhs[1] = out1;

     col = output[2].colSize;
     row = output[2].rowSize;

    mxArray *out2 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[2].data.begin(), output[2].data.end(), mxGetPr(out2));

    plhs[2] = out2;


    col = output[3].colSize;
    row = output[3].rowSize;

    mxArray *out3 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[3].data.begin(), output[3].data.end(), mxGetPr(out3));

    plhs[3] = out3;

    col = output[4].colSize;
    row = output[4].rowSize;

    mxArray *out4 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[4].data.begin(), output[4].data.end(), mxGetPr(out4));

    plhs[4] = out4;

    col = output[5].colSize;
    row = output[5].rowSize;

    mxArray *out5 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[5].data.begin(), output[5].data.end(), mxGetPr(out5));

    plhs[5] = out5;

    col = output[6].colSize;
    row = output[6].rowSize;

    mxArray *out6 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[6].data.begin(), output[6].data.end(), mxGetPr(out6));

    plhs[6] = out6;


    col = output[7].colSize;
    row = output[7].rowSize;

    mxArray *out7 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[7].data.begin(), output[7].data.end(), mxGetPr(out7));

    plhs[7] = out7;


    col = output[8].colSize;
    row = output[8].rowSize;

    mxArray *out8 = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[8].data.begin(), output[8].data.end(), mxGetPr(out8));

    plhs[8] = out8;

    msg = "MEX Stop!";

}
