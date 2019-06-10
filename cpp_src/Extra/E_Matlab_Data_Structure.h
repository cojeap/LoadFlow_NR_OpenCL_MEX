//
// Created by Petru on 22.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_E_MATLAB_DATA_STRUCTURE_H
#define NEWTON_RAPHSON_MATLAB_MEX_E_MATLAB_DATA_STRUCTURE_H

#include <vector>

struct E_Matlab_Data_Structure {

    std::vector<double> data;
    int rowSize;
    int colSize;

    E_Matlab_Data_Structure() : data{}, rowSize{0}, colSize{0} {};

    E_Matlab_Data_Structure(
            const int &rowS,
            const int &colS,
            const std::vector<double> &vecData
    ) : data{vecData},
        rowSize{rowS},
        colSize{colS} {};

    E_Matlab_Data_Structure(
            const int &rowS,
            const int &colS,
            const std::vector<std::vector<double>> &vecData
    ){
        rowSize = rowS;
        colSize = colS;

        for(const auto &i : vecData){
            for(const auto &j : i){
                data.push_back(j);
            }
        }
    };

    E_Matlab_Data_Structure(
            const int &rowS,
            const int &colS
    ){
        rowSize = rowS;
        colSize = colS;

        data = std::vector<double>(rowS*colS,0);
    };
    ~E_Matlab_Data_Structure() = default;
};


#endif //NEWTON_RAPHSON_MATLAB_MEX_E_MATLAB_DATA_STRUCTURE_H
