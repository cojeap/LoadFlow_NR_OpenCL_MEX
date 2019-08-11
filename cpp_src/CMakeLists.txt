cmake_minimum_required(VERSION 3.13)

project(Newton_Raphson_Matlab_MEX)

set(CMAKE_CXX_STANDARD 17)

set(MATLAB_ROOT "C:/MATLAB/2019a")

find_package(OpenCL REQUIRED)
find_package(MATLAB REQUIRED)


include_directories(${Matlab_INCLUDE_DIRS})
include_directories(${OpenCL_INCLUDE_DIRS})


add_executable(Newton_Raphson_Matlab_MEX
        mex_Newton_Raphson_OpenCL.cpp
        Power_System_Core/PSC_nodes.cpp
        Power_System_Core/PSC_nodes.h
        Power_System_Core/PSC_lines.cpp
        Power_System_Core/PSC_lines.h
        Power_System_Extra/PSE_Power_Flow_Data.cpp
        Power_System_Extra/PSE_Power_Flow_Data.h
        Power_System_Core/PSC_electrical_power_system.h
        OpenCL_Core/OCLC_Compute_Obj.cpp
        OpenCL_Core/OCLC_Compute_Obj_Extra_01.cpp
        OpenCL_Core/OCLC_Compute_Obj.h
        INFO/Logger.h
        Extra/E_Matlab_Data_Structure.h
        OpenCL_Core/OCLC_Compute_Obj_Extra_02.cpp
        main.cpp
        mex_Newton_Raphson_OpenCL.h Power_System_Core/PSC_electrical_transformer.cpp Power_System_Core/PSC_electrical_transformer.h)

target_link_libraries(Newton_Raphson_Matlab_MEX OpenCL::OpenCL)
target_link_libraries(Newton_Raphson_Matlab_MEX ${Matlab_LIBRARIES})