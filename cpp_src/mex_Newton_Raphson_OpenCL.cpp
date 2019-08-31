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
 * implement additional overloads for functions that only work with kernel id
 */
#include "mex_Newton_Raphson_OpenCL.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
//mxArray * pOutArgs,const mxArray * pInArgs)
{
    //misc variables

    Logger::Logger LOG;
    std::string msg;

    msg = "MEX Start!";
    LOG.Write(msg);

    //catch input error here
    if (nrhs < 19) {
        msg = "Expected number of input arguments does not match!";
        LOG.Write(msg);
        return;
    }



    std::vector<std::vector<double> > input;
    {
        for (auto i = 0; i < 19; i++) {
            std::vector<double> tempvctr;

            if (mxIsEmpty(prhs[i])) {
                msg = "Expected input argument number: " + std::to_string(i) + " to have data!\nEnding program.";
                LOG.Write(msg);
                return;
            }

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
        if(nrhs>19&&nrhs<30) {
            std::vector<bool> trafoDataCheck; // all false or all true, otherwise there was an input error.
            for (auto i = 19; i < 30; i++) {
                if (mxIsEmpty(prhs[i])) {
                    // if ptr is empty (mxIsEmpty ret true) push false in check vct to express lack of data
                    trafoDataCheck.push_back(false);
                } else {
                    trafoDataCheck.push_back(true);
                }
            }
            if (std::all_of(trafoDataCheck.begin(), trafoDataCheck.end(), [](bool x) { return !x; })) {
                // no trafo input given, continue as if none are present/needed.
                msg = "Trafo not present. Considering el. transfomer data not needed.";
                LOG.Write(msg);
                for (auto i = 19; i < 30; i++) {
                    input.emplace_back(std::vector<double>{});
                }

            } else if (std::all_of(trafoDataCheck.begin(), trafoDataCheck.end(), [](bool x) { return x; })) {
                for (auto i = 19; i < 30; i++) {
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
            } else {
                msg = "Unexpected input error. El. transformer data is not correct(Missing inputs)! Aborting program.";
                LOG.Write(msg);
                return;
            }
        }
    }

    msg = "GPU compute return!";
    LOG.Write(msg);

    std::vector<E_Matlab_Data_Structure> output;
    auto err = main(input,output);
    if(err!=0){
        msg = "Error number: " + std::to_string(err) + "from main compute function!";
        LOG.Write(msg);
        return;
    }

    if (output.empty()) {
        msg = "Forced Stop! Received output is empty!";
        LOG.Write(msg);
        return;
    }

    auto col = output[0].colSize;
    auto row = output[0].rowSize;

    mxArray *out = mxCreateDoubleMatrix(row, col, mxREAL);
    std::copy(output[0].data.begin(), output[0].data.end(), mxGetPr(out));

    plhs[0] = out;

    msg = "MEX Stop!";
    LOG.Write(msg);

}
