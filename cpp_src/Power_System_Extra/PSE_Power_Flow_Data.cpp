//
// Created by Petru on 14.04.2019.
//

#include "PSE_Power_Flow_Data.h"

PSE_Power_Flow_Data::PSE_Power_Flow_Data() : voltage{std::vector<double>{}},
                                             angle{std::vector<double>{}},
                                             activePower{std::vector<double>{}},
                                             reactivePower{std::vector<double>{}} {}

PSE_Power_Flow_Data::PSE_Power_Flow_Data(std::vector<double> &volt,
                                         std::vector<double> &ang,
                                         std::vector<double> &acpow,
                                         std::vector<double> &repow) : voltage{volt},
                                                                       angle{ang},
                                                                       activePower{acpow},
                                                                       reactivePower{repow} {}

PSE_Power_Flow_Data &PSE_Power_Flow_Data::operator=(const PSE_Power_Flow_Data &pfd){
    voltage = pfd.voltage;
    angle = pfd.angle;
    activePower = pfd.activePower;
    reactivePower = pfd.reactivePower;

    return *this;
}
//
