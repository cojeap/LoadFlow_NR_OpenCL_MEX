//
// Created by Petru on 03.10.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_OCLMANAGER_H
#define NEWTON_RAPHSON_MATLAB_MEX_OCLMANAGER_H

#define __CL_ENABLE_EXCEPTIONS

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else

#include <CL/cl.hpp>

#endif

#include <exception>
#include <fstream>
#include <memory>

class ExceptHandler : public std::exception{
    std::string msg;
    int err_code;
public:
    explicit ExceptHandler(const char *str) {
        msg = std::string{str};
        err_code = 0;
    };

    ExceptHandler(const char *str, const int &err_nr) {
        msg = std::string{str};
        err_code = err_nr;
    };

    explicit ExceptHandler(const std::string &str) {
        msg = str;
        err_code = 0;
    };

    ExceptHandler(const std::string &str, const int &err_nr) {
        msg = str;
        err_code = err_nr;
    }

    std::string what() { return msg; };
    int err() { return err_code; };
};

enum class KernelIndex : int {

};

class OCLManager {
    std::unique_ptr<cl::Platform> platform;
    std::unique_ptr<cl::Device> device;
    std::unique_ptr<cl::Context> context;
    std::unique_ptr<cl::Program> program;
    std::unique_ptr<cl::CommandQueue> cmdQueue;

public:
    OCLManager() = delete;

    explicit OCLManager(const std::string& srcFileName) {


        if (srcFileName.empty()) {
            throw ExceptHandler("Kernel Source file name has not been passed!",-7387);
        }
        std::ifstream file{srcFileName};
        if (!file) {
            throw ExceptHandler("Kernel Source file cannot be loaded!",-1);
        }


        /* Find all platforms and devices*/

        auto platCounter{0};
        auto devCounter{0};

        std::vector<cl::Platform> allPlat;
        std::vector<std::vector<cl::Device>> allDev;
        auto platformReturnCode = cl::Platform::get(&allPlat);
        if (allPlat.empty()) {
            throw ExceptHandler("Could not find any OpenCL platform.",-7387);
        }

        std::ofstream log{"OpenCL_constructor_log.txt"};

        log << "__________" << std::endl;
        for (const auto &plat : allPlat) {
            log << "Platform:  " << platCounter << "\n";
            log << "CL_PLATFORM_VENDOR:  " << plat.getInfo<CL_PLATFORM_VENDOR>() << "\n";
            log << "CL_PLATFORM_NAME:  " << plat.getInfo<CL_PLATFORM_NAME>() << "\n";
            log << "CL_PLATFORM_VERSION:  " << plat.getInfo<CL_PLATFORM_VERSION>() << "\n";
            log << "CL_PLATFORM_PROFILE:  " << plat.getInfo<CL_PLATFORM_PROFILE>() << "\n";
            log << "CL_PLATFORM_EXTENSIONS:  " << plat.getInfo<CL_PLATFORM_EXTENSIONS>() << "\n";

            std::vector<cl::Device> platDev;
            plat.getDevices(CL_DEVICE_TYPE_ALL, &platDev);
            if (!platDev.empty()) {
                for (const auto &dev : platDev) {
                    log << "Device:  " << devCounter << "\n";
                    log << "CL_DEVICE_VENDOR:  " << dev.getInfo<CL_DEVICE_VENDOR>() << "\n";
                    log << "CL_DEVICE_VENDOR_ID:  " << dev.getInfo<CL_DEVICE_VENDOR_ID>() << "\n";
                    log << "CL_DEVICE_NAME:  " << dev.getInfo<CL_DEVICE_NAME>() << "\n";
                    log << "CL_DEVICE_MAX_COMPUTE_UNITS:  " << dev.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";

                    log << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:  " << dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
                    log << "\n";
                    log << "CL_DEVICE_MAX_WORK_ITEM_SIZES:  ";
                    for (const auto &i : dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()) {
                        log << i << "\t";
                    }
                    log << "\n";

                    log << "CL_DEVICE_MAX_CLOCK_FREQUENCY:  " << dev.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << "\n";
                    log << "CL_DEVICE_LOCAL_MEM_SIZE:  " << dev.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << "\n";
                    log << "CL_DEVICE_GLOBAL_MEM_SIZE:  " << dev.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << "\n";
                    devCounter++;
                }
            }
            log << "__________" << std::endl;
            allDev.push_back(platDev);

            platCounter++;
        }
        {
            int counter{0};
            for (auto &i: allDev) {
                if(i.empty()) {
                    counter++;
                }
            }
            if(counter>0){
                throw ExceptHandler("Could not find any OpenCL device",-7387);
            }
        }

        /*Initialise platform*/
        platform = std::move(std::make_unique<cl::Platform>(allPlat[0]));
        if(!platform){
            throw ExceptHandler("Could not initialise the OpenCL platform!",-2);
        }

        /*Initialise device*/
        device = std::move(std::make_unique<cl::Device>(allDev[0][0]));
        if(!device){
            throw ExceptHandler("Could not initialise the OpenCL device",-3);
        }

        /*Initialise context*/
        context = std::move(std::make_unique<cl::Context>(*device));
        if(!context){
            throw ExceptHandler("Could not initialise the OpenCL context",-4);
        }

        /*Load kernels,initialise the program and build it*/
        cl::Program::Sources gpuSource;

        std::string src;
        std::string kernelCode;
        /*FILE*/
        while (std::getline(file, src)) {
            kernelCode += src;
        }
        kernelCode += "    ";

        gpuSource.push_back({kernelCode.c_str(), kernelCode.length() + 1});

        log << kernelCode << "\n";

        program = std::move(std::make_unique<cl::Program>(*context, gpuSource));
        if(!program){
            throw ExceptHandler("Could not initialise the OpenCL program.",-5);
        }

        log << "__________" << std::endl;

        if (program->build({*device}) != CL_SUCCESS) {
            log << "Error::PROGRAM_BUILD:  " << program->getBuildInfo<CL_PROGRAM_BUILD_STATUS>(*device) << "\n";
            log << program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(*device) << "\n";
        }
        log << "__________" << std::endl;

        /*Initialise command queue*/
        cmdQueue = std::move(std::make_unique<cl::CommandQueue>(*context));
        if(!program){
            throw ExceptHandler("Could not initialise the OpenCL command queue.",-6);
        }

    };

    ~OCLManager() = default;
};





#endif //NEWTON_RAPHSON_MATLAB_MEX_OCLMANAGER_H
