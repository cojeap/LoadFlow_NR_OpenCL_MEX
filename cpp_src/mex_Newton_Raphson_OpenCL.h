//
// Created by Petru on 30.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_MEX_NEWTON_RAPHSON_OPENCL_H
#define NEWTON_RAPHSON_MATLAB_MEX_MEX_NEWTON_RAPHSON_OPENCL_H

#include <vector>
#include <string>
#include <iostream>

#include <CL/cl.hpp>
#include <mex.h>

#include "Power_System_Core/PSC_electrical_power_system.h"
#include "Power_System_Extra/PSE_Power_Flow_Data.h"
#include "OpenCL_Core/OCLC_Compute_Obj.h"
#include "Extra/E_Matlab_Data_Structure.h"
#include "INFO/Logger.h"

int main(const std::vector<std::vector<double>>& input, std::vector<E_Matlab_Data_Structure>& output);

#endif //NEWTON_RAPHSON_MATLAB_MEX_MEX_NEWTON_RAPHSON_OPENCL_H
