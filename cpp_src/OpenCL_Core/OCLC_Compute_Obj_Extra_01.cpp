//
// Created by Petru on 19.04.2019.
//

#include "OCLC_Compute_Obj.h"

std::vector<std::string> OCLC_Compute_Obj::GetKernelNames() {

    std::vector<std::string> kernNames;

    for (const auto &i : kernelsInfo) {
        kernNames.push_back(i.first);
    }

    return kernNames;
}

void OCLC_Compute_Obj::MakeKernel(const std::string &kernelName, const int &kernelID) {

    kernels.emplace_back(OCLC_kernel(cl::Kernel(*program, kernelName.c_str()), kernelName, kernelID));
}

void OCLC_Compute_Obj::SetRange(const int &kernelID,
                                const cl::NDRange &offset,
                                const cl::NDRange &globalRange,
                                const cl::NDRange &localRange) {
    OCLC_NDRange range{offset, globalRange, localRange};

    for (auto &i : kernels) {
        if (i.ID == kernelID) {
            i.ranges = range;
        }
    }
}


void OCLC_Compute_Obj::Run(const int &kernelID, std::vector<double>& result) {

    for (auto &i: kernels) {
        if (i.ID == kernelID) {
            cmdQueue->enqueueNDRangeKernel(i.kernel,
                                           i.ranges.offset,
                                           i.ranges.globalRange,
                                           i.ranges.localRange);
            for(auto &j: i.output){
                cmdQueue->enqueueReadBuffer(j,CL_TRUE,0,sizeof(double)*result.size(),&result[0]);
            }
        }
    }

}

void OCLC_Compute_Obj::Run(const int &kernelID, std::vector<std::vector<double>>& result) {

    for (auto &i: kernels) {
        if (i.ID == kernelID) {
            cmdQueue->enqueueNDRangeKernel(i.kernel,
                                           i.ranges.offset,
                                           i.ranges.globalRange,
                                           i.ranges.localRange);
            auto idx = 0;
            for(auto &j: i.output){
                cmdQueue->enqueueReadBuffer(j,CL_TRUE,0,sizeof(double)*result[idx].size(),&result[idx][0]);
                idx++;
            }
        }
    }

}

void OCLC_Compute_Obj::Finish() {
    cmdQueue->finish();
}

/*
void OCLC_Compute_Obj::SetKernelNDRange(
        const std::string &kernelName,
        const cl::NDRange &offset,
        const cl::NDRange &globalRange,
        const cl::NDRange &localRange) {
    for (const auto &i : kernels) {
        if (i.name.compare(kernelName)) {
            cmdQueue->enqueueNDRangeKernel(i.kernel, offset, globalRange, localRange);
        }
    }
}


void OCLC_Compute_Obj::EnqueueNDRangeKernel(
        const int &kernelID,
        const cl::NDRange &offset,
        const cl::NDRange &globalRange,
        const cl::NDRange &localRange) {
    for (const auto &i : kernels) {
        if (i.ID == kernelID) {
            cmdQueue->enqueueNDRangeKernel(i.kernel, offset, globalRange, localRange);
        }
    }
}


void OCLC_Compute_Obj::SetInputBuffer(const std::string &kernelName, const std::vector<cl::Buffer> &clBuffers) {
    if(clBuffers.empty()){
        success=false;
        std::string info = "Cannot set input buffers for: " + kernelName;
        Logger::Logger LOG;
        LOG.Write(info);
        return;
    }
    for(auto& i : kernels){
        if(i.name.compare(kernelName)){
            for(auto & j: clBuffers){
                auto argNr = i.kernel.getInfo<CL_KERNEL_NUM_ARGS>();
                argNr+=1;
                i.kernel.setArg(argNr,j);
            }
        }
    }
}

void OCLC_Compute_Obj::SetInputBuffer(const int &kernelID, const std::vector<cl::Buffer> &clBuffers) {
    if(clBuffers.empty()){
        success=false;
        std::string info = "Cannot set input buffers for: " +  std::to_string(kernelID);
        Logger::Logger LOG;
        LOG.Write(info);
        return;
    }
    for(auto& i : kernels){
        if(i.ID==kernelID){
            for(auto & j: clBuffers){
                auto argNr = i.kernel.getInfo<CL_KERNEL_NUM_ARGS>();
                argNr+=1;
                i.kernel.setArg(argNr,j);
            }
        }
    }
}

void OCLC_Compute_Obj::SetOutputBuffer(const std::string &kernelName, const std::vector<cl::Buffer> &clBuffers) {
    if(clBuffers.empty()){
        success=false;
        std::string info = "Cannot set output buffers for: " + kernelName;
        Logger::Logger LOG;
        LOG.Write(info);
        return;
    }
    for(auto& i : kernels){
        if(i.name.compare(kernelName)){
            for(auto & j: clBuffers){
                auto argNr = i.kernel.getInfo<CL_KERNEL_NUM_ARGS>();
                argNr+=1;
                i.kernel.setArg(argNr,j);
            }
        }
    }
}

void OCLC_Compute_Obj::SetOutputBuffer(const int &kernelID, const std::vector<cl::Buffer> &clBuffers) {
    if(clBuffers.empty()){
        success=false;
        std::string info = "Cannot set input buffers for: " + std::to_string(kernelID);
        Logger::Logger LOG;
        LOG.Write(info);
        return;
    }
    for(auto& i : kernels){
        if(i.ID==kernelID){
            for(auto & j: clBuffers){
                auto argNr = i.kernel.getInfo<CL_KERNEL_NUM_ARGS>();
                argNr+=1;
                i.kernel.setArg(argNr,j);
            }
        }
    }
}
 */