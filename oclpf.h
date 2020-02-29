#ifndef OPENCL_POWERFLOW_MexFunction_H
#define OPENCL_POWERFLOW_MexFunction_H

#include <iostream>

// octave 4.4.1
#include "octave-4.4.1/octave/oct.h"

#include "Data_Bank/Data_Bank.h"
#include "Power_System/Power_System.h"

 DEFUN_DLD (helloworld, args, nargout,
                "Hello World Help String")
     {
       octave_stdout << "Hello World has "
                     << args.length () << " input arguments and "
                     << nargout << " output arguments.\n";
     
       // Return empty matrices for any outputs
       octave_value_list retval (nargout);
       for (int i = 0; i < nargout; i++)
         retval(i) = octave_value (Matrix ());
     
       return retval;
     }


#endif //OPENCL_POWERFLOW_MATLAB_INTERCONNECT_H