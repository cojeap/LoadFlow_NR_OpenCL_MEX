/* This is a C++ MEX file for MATLAB.
Define differential equations for restricted three-body problem.
YP = YPRIME(T,Y) 
Y is a column or row vector with 4 elements.
Y(1) and Y(3) = coordinates of the third body.
Y(2) and Y(4) = velocity of the third body.
T is ignored.
MATLAB usage:
[t,y]=ode23(@yprime,[0 1],[0 1 2 3])


* Copyright 2017-2018 The MathWorks, Inc.
*
*/

#include "mex.hpp"
#include "mexAdapter.hpp"

class MexFunction : public matlab::mex::Function {

    matlab::data::ArrayFactory factory;

public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
        checkArguments(outputs, inputs);
        const size_t numRows = inputs[1].getDimensions()[0];
        const size_t numColumns = inputs[1].getDimensions()[1];
        const matlab::data::TypedArray<double> Y = std::move(inputs[1]);
        matlab::data::TypedArray<double> Yp = factory.createArray<double>({ numRows, numColumns });
        yPrime(Yp, Y);
        outputs[0] = std::move(Yp);
    }

    void yPrime(matlab::data::TypedArray<double>& Yp, const matlab::data::TypedArray<double>& Y) {
        const	double	mu = 1 / 82.45;
        const	double	mus = 1 - 1 / 82.45;
        const std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
        const double r1 = sqrt((Y[0] + mu)*(Y[0] + mu) + Y[2] * Y[2]);
        const double r2 = sqrt((Y[0] - mus)*(Y[0] - mus) + Y[2] * Y[2]);
        
        if (r1 == 0.0 || r2 == 0.0) {
            matlabPtr->feval(u"warning", 0, 
                std::vector<matlab::data::Array>({ factory.createScalar("Division by zero!\n") }));
        }

        Yp[0] = double(Y[1]);      
        Yp[1] = 2 * Y[3] + Y[0] - mus*(Y[0] + mu) / (r1*r1*r1) - mu*(Y[0] - mus) / (r2*r2*r2);
        Yp[2] = double(Y[3]);
        Yp[3] = -2 * Y[1] + Y[2] - mus*Y[2] / (r1*r1*r1) - mu*Y[2] / (r2*r2*r2);
    }

    void checkArguments(matlab::mex::ArgumentList & outputs, matlab::mex::ArgumentList & inputs) {
        const size_t numRows = inputs[1].getDimensions()[0];
        const size_t numColumns = inputs[1].getDimensions()[1];
        const std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
        
        if (inputs.size() != 2) {
            matlabPtr->feval(u"error", 0,
                std::vector<matlab::data::Array>({ factory.createScalar("Two input required") }));
        }

        if (outputs.size() > 1) {
            matlabPtr->feval(u"error", 0,
                std::vector<matlab::data::Array>({ factory.createScalar("Maximum one output supported") }));
        }
        if (inputs[1].getType() != matlab::data::ArrayType::DOUBLE ||
            inputs[1].getNumberOfElements() != 4 ||
            (std::max(numRows,numColumns) != 4) || (std::min(numRows,numColumns) != 1)) {
             matlabPtr->feval(u"error",0, 
                 std::vector<matlab::data::Array>({ factory.createScalar(
                    "Input must be 4-element vector of non complex double elements") }));
        }
    };
 };