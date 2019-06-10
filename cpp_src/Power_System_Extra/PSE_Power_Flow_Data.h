//
// Created by Petru on 14.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_PSE_POWER_FLOW_DATA_H
#define NEWTON_RAPHSON_MATLAB_MEX_PSE_POWER_FLOW_DATA_H

#include<vector>

class PSE_Power_Flow_Data {
public:
    std::vector<double> voltage;
    std::vector<double> angle;
    std::vector<double> activePower;
    std::vector<double> reactivePower;


    PSE_Power_Flow_Data();

    ~PSE_Power_Flow_Data() = default;

    PSE_Power_Flow_Data(
            std::vector<double> &volt,
            std::vector<double> &ang,
            std::vector<double> &acpow,
            std::vector<double> &repow
    );

    PSE_Power_Flow_Data& operator=(const PSE_Power_Flow_Data& pfd);
};


#endif //NEWTON_RAPHSON_MATLAB_MEX_PSE_POWER_FLOW_DATA_H
