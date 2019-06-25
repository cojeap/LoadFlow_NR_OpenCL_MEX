//
// Created by Petru on 16.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_OCLC_COMPUTE_OBJ_H
#define NEWTON_RAPHSON_MATLAB_MEX_OCLC_COMPUTE_OBJ_H

#include<string>
#include<vector>
#include<fstream>

#include<CL/CL.hpp>
#include <memory>


class OCLC_Compute_Obj {

    struct OCLC_NDRange {
        cl::NDRange offset;
        cl::NDRange globalRange;
        cl::NDRange localRange;

        OCLC_NDRange() : offset{cl::NullRange},
                         globalRange{cl::NullRange},
                         localRange{cl::NullRange} {};

        explicit OCLC_NDRange(const cl::NDRange &offset = cl::NullRange,
                              const cl::NDRange &globalRange = cl::NullRange,
                              const cl::NDRange &localRange = cl::NullRange) : offset{offset},
                                                                               globalRange{globalRange},
                                                                               localRange{localRange} {};

        OCLC_NDRange &operator=(const OCLC_NDRange &ranges) {
            offset = ranges.offset;
            globalRange = ranges.globalRange;
            localRange = ranges.localRange;

            return *this;
        };

        ~OCLC_NDRange() = default;
    };

    struct OCLC_kernel {

        cl::Kernel kernel;

        std::string name;
        int ID;

        std::vector<cl::Buffer> input;
        std::vector<cl::Buffer> output;

        OCLC_NDRange ranges;

        OCLC_kernel(cl::Kernel kernel,
                    const std::string &kernelName,
                    int kernelID
        ) : kernel{std::move(kernel)},
            name{kernelName},
            ID{kernelID},
            ranges{cl::NullRange, cl::NullRange, cl::NullRange} {};

        ~OCLC_kernel()=default;

    };

    cl::Platform *platform;
    cl::Device *device;
    cl::Context *context;
    cl::Program *program;
    cl::CommandQueue *cmdQueue;

    std::vector<std::pair<std::string, int> > kernelsInfo;
    std::vector<OCLC_kernel> kernels;

    /***/


public:

    bool success;

    explicit OCLC_Compute_Obj(std::string srcFile);

    OCLC_Compute_Obj &operator=(const OCLC_Compute_Obj &obj) = delete;

    ~OCLC_Compute_Obj();

    std::vector<std::string> GetKernelNames();

    void MakeKernel(const std::string &kernelName,
                    const int &kernelID);

    void AddInputBuffer(const int &kernelID,
                        const std::vector<std::vector<double>> &dataMatrix);

    void AddInputBuffer(const std::string &kernelName,
                        const std::vector<std::vector<double>> &dataMatrix);

/*
    void AddInputBuffer(const int &kernelID,
            const std::vector<double> &dataMatrix);
*/
    void SetKernelParameters(const int &kernelID);

    void SetKernelParameters(const std::string &kernelName);

    void AddOutputBuffer(const int &kernelID,
                         const size_t &siz);

    void AddOutputBuffer(const std::string &kernelName,
                         const size_t &size);

    void SetRange(const int &kernelID,
                  const cl::NDRange &offset,
                  const cl::NDRange &globalRange,
                  const cl::NDRange &localRange);

    void Run(const int &kernelID, std::vector<double> &result);

    void Run(const int &kernelID, std::vector<std::vector<double>> &result);

    void ClearBuffers(const int &kernelID);

    bool KernelExists(const int &kernelID);

    void Finish();

    void AddInputBuffer(const int &kernelID, std::vector<double> &dataMatrix);

};


#endif //NEWTON_RAPHSON_MATLAB_MEX_OCLC_COMPUTE_OBJ_H
