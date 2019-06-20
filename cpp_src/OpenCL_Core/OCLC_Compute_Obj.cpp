//
// Created by Petru on 16.04.2019.
//

#include "OCLC_Compute_Obj.h"
#include "../INFO/Logger.h"


OCLC_Compute_Obj::OCLC_Compute_Obj(std::string srcFile) {
    success = true;

    platform = nullptr;
    device = nullptr;
    context = nullptr;
    program = nullptr;
    cmdQueue = nullptr;


    Logger::Logger LOG;
    std::string info;

    std::vector<cl::Platform> allPlat;
    cl::Platform::get(&allPlat);
    if (allPlat.empty()) {
        success = false;
        info = "Cannot find an OpenCL platform!";
        LOG.Write(info);
        return;
    }

    platform = new cl::Platform{allPlat[0]};

    info = "Using platform " + platform->getInfo<CL_PLATFORM_NAME>() + '\t' +
           platform->getInfo<CL_PLATFORM_VENDOR>() + '\t' +
           platform->getInfo<CL_PLATFORM_VERSION>();
    LOG.Write(info);

    std::vector<cl::Device> allDev;
    platform->getDevices(CL_DEVICE_TYPE_GPU | CL_DEVICE_TYPE_DEFAULT, &allDev);
    if (allDev.empty()) {
        success = false;
        info = "Cannot find an OpenCL device!";
        LOG.Write(info);
        return;
    }

    device = new cl::Device{allDev[0]};
    info = "Using device " + device->getInfo<CL_DEVICE_NAME>() + '\t' +
           device->getInfo<CL_DEVICE_VENDOR>() + '\t' +
           device->getInfo<CL_DEVICE_VERSION>();
    LOG.Write(info);

    context = new cl::Context(*device);
    info = "Context built!";
    LOG.Write(info);

    cl::Program::Sources gpuSource;

    std::string src;
    std::string kernelCode;
    std::ifstream file{srcFile};
    if (!file) {
        success = false;
        info = "Cannot load kernel source file!";
        LOG.Write(info);
        return;
    }
    while (std::getline(file, src)) {
        kernelCode += src;
    }
    kernelCode += "    ";

    info = "Kernel sources loaded!";
    LOG.Write(info);

    gpuSource.push_back({kernelCode.c_str(), kernelCode.length() + 1});

    info = "Kernel sources added;";

    program = new cl::Program{*context, gpuSource};

    if (program->build({*device}) != CL_SUCCESS) {
        success = false;
        info = "Program build failure!";
        LOG.Write(info);

        info.clear();
        info = std::to_string(program->getBuildInfo<CL_PROGRAM_BUILD_STATUS>(*device));
        LOG.Write(info);
        info.clear();
        info = std::to_string(program->getBuildInfo<CL_PROGRAM_BUILD_STATUS>(*device));
        LOG.Write(info);
        return;
    }

    auto numOfKernels{program->getInfo<CL_PROGRAM_NUM_KERNELS>()};
    std::string kNames{program->getInfo<CL_PROGRAM_KERNEL_NAMES>()+';'};

    std::string name;
    size_t splitPos;
    std::string delimiter{';'};
    // /* ; */ is the standard delimiter of 2 kernel names for program.getInfo<CL_PROGRAM_KERNEL_NAMES>()

    while ((splitPos = kNames.find(delimiter)) != std::string::npos) {
        name = kNames.substr(0, splitPos);
        kernelsInfo.emplace_back(name,-66);
        kNames.erase(0, splitPos + delimiter.length());
    }

    if (numOfKernels != kernelsInfo.size()) {
        success = false;
        info = "Number of kernels does not match during check!";
        LOG.Write(info);
        return;
    }

    info = "OpenCL Program ready!";
    LOG.Write(info);

    cmdQueue = new cl::CommandQueue(*context);

    info = "Command Queue built!";
    LOG.Write(info);

    info = "OpenCL Compute OBJ successfully constructed!";
    LOG.Write(info);

}

OCLC_Compute_Obj::~OCLC_Compute_Obj() {
/*
    delete cmdQueue;
    cmdQueue = nullptr;
*/
    delete program;
    program = nullptr;

    delete context;
    context = nullptr;

    delete device;
    device = nullptr;

    delete platform;
    platform = nullptr;

    for(auto& i : kernels){
        ClearBuffers(i.ID);
    }

    kernelsInfo.clear();
    kernels.clear();


}

