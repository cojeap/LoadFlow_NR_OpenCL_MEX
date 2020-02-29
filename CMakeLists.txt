#06.02.2020  DD.MM.YYYY
cmake_minimum_required (VERSION 3.8)

project ("OpenCL_PowerFlow")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")


find_package(OpenCL REQUIRED)
find_package(MATLAB REQUIRED)

include_directories(${Matlab_INCLUDE_DIRS})
include_directories(${OpenCL_INCLUDE_DIRS})


add_executable(	OpenCL_PowerFlow
				"Power_System/Power_System_Element_Types.h"
				"Power_System/Power_System_Element.h"
				"Power_System/Power_System_Element.cpp"
				"Power_System/Power_System.h"
				"Data_Bank/Data_Bank.h"
				"Data_Bank/Data_Bank.cpp"
				"oclpf.h"
				"oclpf.cpp")
				

target_link_libraries(OpenCL_PowerFlow OpenCL::OpenCL)
target_link_libraries(OpenCL_PowerFlow ${Matlab_LIBRARIES})



