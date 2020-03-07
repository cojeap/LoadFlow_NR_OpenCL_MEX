#include "oclpf.h"


 DEFUN_DLD (helloworld, args, nargout, "Hello World Help String")
{
  octave_stdout << "Hello World has "
                << args.length () << " input arguments and "
                << nargout << " output arguments.\n";
  octave_value_list retval (nargout);
  for (int i = 0; i < nargout; i++){
    retval(i) = octave_value (Matrix ());
  }
  return retval;
}
