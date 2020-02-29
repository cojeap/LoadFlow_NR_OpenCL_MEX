//
// Created by Petru on 16.09.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_POWERSYSTEM_H
#define NEWTON_RAPHSON_MATLAB_MEX_POWERSYSTEM_H

#include <vector>
#include <complex>

#include "../DataTypes.h"

#include "PowerSystemElement.h"



class PowerSystem {
    SystemElement<BUS> busData;
    SystemElement<BRANCH> branchData;

    std::vector< std::vector< std::complex< double > > > Ynn;

    void CalculateAdmittanceMatrix(){
        std::vector<double> realImpedance, imagImpedance, realAdmittance,imagAdmittance;

        for (auto row=0;row<branchData.nrOfRows;row++ ){

            double modifier = branchData.GetVal(row,BRANCH::NR_OF_CONDUCTORS);

            double reImp = modifier * branchData.GetVal(row,BRANCH::LINE_lENGTH)*branchData.GetVal(row,BRANCH::RESISTANCE_0);
            double imImp = modifier * branchData.GetVal(row,BRANCH::LINE_lENGTH)*branchData.GetVal(row,BRANCH::REACTANCE_0);

            double reAdm = modifier*branchData.GetVal(row,BRANCH::LINE_lENGTH)*branchData.GetVal(row,BRANCH::CONDUCTANCE_0)/2.0;
            double imAdm = modifier*branchData.GetVal(row,BRANCH::LINE_lENGTH)*branchData.GetVal(row,BRANCH::SUSCEPTANCE_0)/2.0;

            realAdmittance.push_back(reImp);
            imagImpedance.push_back(imImp);
            realAdmittance.push_back(reAdm);
            imagImpedance.push_back(imAdm);
        }

        for(auto row = 0; row<busData.nrOfRows;row++){
            Ynn.push_back(std::vector<std::complex<double>>(busData.nrOfRows,std::complex<double>{0.0,0.0}));
        }

        for(auto row = 0; row<branchData.nrOfRows;row++){
            std::complex<double> yL = 1.0 / std::complex<double>{realImpedance[row],imagImpedance[row]};
            std::complex<double> yT = std::complex<double>{realAdmittance[row],imagAdmittance[row]};

            auto i = branchData.GetVal(row,BRANCH::FROM)-1;
            auto j = branchData.GetVal(row,BRANCH::TO)-1;

            Ynn[i][i]+=(yL+yT);
            Ynn[j][j]+=(yL+yT);
            Ynn[i][j]=(-yL);
            Ynn[j][i]=(-yL);

        }
        /*
        if(trafo exists){
            do;
        }
        */

    };
public:
    PowerSystem(const std::vector<std::vector<double>>& inBusData,
                const std::vector<std::vector<double>>& inBranchData) : busData{inBusData}, branchData{inBranchData}{

        CalculateAdmittanceMatrix();

        //misc
    };

};


#endif //NEWTON_RAPHSON_MATLAB_MEX_POWERSYSTEM_H
