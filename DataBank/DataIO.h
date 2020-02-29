//
// Created by Petru on 03.10.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_DATAIO_H
#define NEWTON_RAPHSON_MATLAB_MEX_DATAIO_H

#include <vector>
#include <tuple>



template<typename ElementType>
class DataIO {

    template<ElementType>
    struct InternalData{
        int rows;
        int cols;
        std::vector<double> data;
        int sizeOf();
    };

    int size;
    std::vector<ElementType> data;

public:
    explicit DataIO(std::vector<double>& input);

    std::tuple<int,int,std::vector<double>> GetData_MatlabFormat(ElementType toGet);

};



#endif //NEWTON_RAPHSON_MATLAB_MEX_DATAIO_H
