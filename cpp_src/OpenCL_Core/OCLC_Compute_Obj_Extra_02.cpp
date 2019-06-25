//
// Created by Petru on 24.04.2019.
//

#include "OCLC_Compute_Obj.h"

void OCLC_Compute_Obj::SetKernelParameters(const int &kernelID) {
    for(auto &i : kernels){
        if(i.ID==kernelID){
            auto idx=0;
            for(auto &j: i.input){
                i.kernel.setArg(idx,j);
                idx++;
            }
            for(auto &j: i.output){
                i.kernel.setArg(idx,j);
                ++idx;
            }
        }
    }
}

void OCLC_Compute_Obj::SetKernelParameters(const std::string &kernelName) {
    for(auto &i : kernels){
        if(i.name==kernelName){
            auto idx=0;
            for(auto &j: i.input){
                i.kernel.setArg(idx,j);
                ++idx;
            }
            for(auto &j: i.output){
                i.kernel.setArg(idx,j);
                ++idx;
            }
        }
    }

}

void OCLC_Compute_Obj::AddInputBuffer(const int &kernelID, const std::vector<std::vector<double>> &dataMatrix) {

    std::vector<double> data;
    for(const auto &i: dataMatrix){
        for(const auto &j : i){
            data.push_back(j);
        }
    }

    for(auto& i : kernels){
        if(i.ID==kernelID){
            i.input.emplace_back(cl::Buffer(*context,data.begin(),data.end(),true,false));
        }
    }
}
/******/
/*
void OCLC_Compute_Obj::AddInputBuffer(const int &kernelID, const std::vector<double> &dataMatrix) {



    for(auto& i : kernels){
        if(i.ID==kernelID){
            //i.input.emplace_back(cl::Buffer{*context,dataMatrix.begin(),dataMatrix.end(),true,false} );
        }
    }
}
*/
void OCLC_Compute_Obj::AddInputBuffer(const int &kernelID, std::vector<double> &dataMatrix) {

    for(auto& i : kernels){
        if(i.ID==kernelID){
            i.input.emplace_back( cl::Buffer{*context,dataMatrix.begin(),dataMatrix.end(),true,false});
        }
    }
}

void OCLC_Compute_Obj::AddInputBuffer(const std::string &kernelName, const std::vector<std::vector<double>> &dataMatrix) {

    std::vector<double> data;
    for(const auto &i: dataMatrix){
        for(const auto &j : i){
            data.push_back(j);
        }
    }

    for(auto& i : kernels){
        if(i.name == kernelName){
            i.input.emplace_back(cl::Buffer(*context,data.begin(),data.end(),true,false));
        }
    }
}

void OCLC_Compute_Obj::AddOutputBuffer(const int &kernelID, const size_t& size) {

    for(auto& i : kernels){
        if(i.ID==kernelID){
            i.output.emplace_back(cl::Buffer(*context,CL_MEM_READ_WRITE,sizeof(double)*size));
        }
    }
}

void OCLC_Compute_Obj::AddOutputBuffer(const std::string &kernelName,const size_t& size) {

    for(auto& i : kernels){
        if(i.name==kernelName){
            i.output.emplace_back(cl::Buffer(*context,CL_MEM_READ_WRITE,sizeof(double)*size));
        }
    }

}

void OCLC_Compute_Obj::ClearBuffers(const int &kernelID) {
    for(auto &i: kernels){
        if(i.ID==kernelID){
            i.input.clear();
            i.output.clear();
        }
    }
}

bool OCLC_Compute_Obj::KernelExists(const int &kernelID) {
    //needs rework
    for(const auto &i : kernels){
        if(i.ID==kernelID){
            return !i.input.empty() || !i.output.empty();
        }
    }

    return false;

}