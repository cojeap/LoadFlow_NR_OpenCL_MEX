//
// Created by Petru on 27.06.2019.
//

#include <math.h>

#include "PSC_electrical_transformer.h"


using namespace PSCT;

PSC_electrical_transformer::PSC_electrical_transformer(const std::vector<std::vector<double>> &inputData) {
    success = true;
    if (inputData.empty()) {
        success = false;
        return;
    }
    if (inputData[19].empty()) {
        success = false;
        return;
    }
    if (inputData.size() < 30) {
        /*
         * The number 19 is chosen as 19-xx are the inputs for lines from Matlab
         * They are expected to be the last set of inputs and in order
         *
         */
        success = false;
        return;
    }

    rowSize = (int) inputData[19].size();
    colSize = 17;

    /** longitudinal parameters **/
    double longitResistance;
    double longitAdmittance;
    double longitReactance;
    /** transversal parameters **/
    double transvConductance;
    double transvAdmittance;
    double transvSusceptance;


    for (int i = 0; i < inputData[19].size(); i++) {
        data.emplace_back(TrafoData{});

        data[i].from = inputData[19][i];

        data[i].to = inputData[20][i];
        data[i].voltage_from = inputData[21][i];
        data[i].voltage_to = inputData[22][i];
        data[i].apparentPower = inputData[23][i];
        data[i].deltaP0 = inputData[24][i];
        data[i].current0 = inputData[25][i];
        data[i].deltaPsc = inputData[26][i];
        data[i].voltageSC = inputData[27][i];
        data[i].nrOfTrafo = inputData[28][i];
        data[i].plot = inputData[29][i];
        data[i].deltaUp = inputData[30][i];

        /****/
        double baseVoltage_from = data[i].voltage_from;
        double baseVoltage_to = data[i].voltage_to;

        double UB1,UB2; //should this be used in all formulas?

        if(baseVoltage_from>=baseVoltage_to) {
            UB1 = baseVoltage_from;
            UB2 = baseVoltage_to;
        } else {
            UB1 = baseVoltage_to;
            UB2 = baseVoltage_from;
        }

        double baseImpedance_from = std::pow(baseVoltage_from,2)/100.0;
        double baseImpedance_to = std::pow(baseVoltage_to,2)/100.0;

        double baseAdmittance_from = 1.0/baseImpedance_from;
        double baseAdmittance_to = 1.0/baseImpedance_to;

        double nrOfTrafoMatters = data[i].nrOfTrafo*baseImpedance_from;;

        /***/
        longitResistance = data[i].deltaPsc*std::pow(data[i].voltage_from,2)/std::pow(data[i].apparentPower,2)/1000.0;
        longitAdmittance = data[i].voltageSC/100.0*std::pow(data[i].voltage_from,2)/data[i].apparentPower;
        //check on resistance and admittance needed
        longitReactance = std::sqrt(std::pow(longitAdmittance,2)-std::pow(longitResistance,2));

        data[i].realLongitImpedance =longitResistance/nrOfTrafoMatters;
        data[i].imagLongitImpedance =longitReactance/nrOfTrafoMatters;

        /***/
        transvConductance = data[i].deltaP0/1000.0*std::pow(data[i].voltage_from,2);
        transvAdmittance = data[i].current0*data[i].apparentPower/100.0*std::pow(data[i].voltage_from,2);

        transvSusceptance = std::sqrt(std::pow(transvAdmittance,2)-std::pow(transvConductance,2));

        data[i].realTransvAdmittance=transvConductance*nrOfTrafoMatters;
        data[i].imagTransvAdmittance=transvSusceptance*nrOfTrafoMatters;

        /***/
        data[i].trafoReport = data[i].voltage_from/data[i].voltage_to*(1.0+data[i].plot*data[i].deltaUp/100.0);
    }


}

PSC_electrical_transformer::TrafoData::TrafoData() {
    from = 0.0;
    to = 0.0;
    voltage_from = 0.0;
    voltage_to = 0.0;
    apparentPower = 0.0;
    deltaP0 = 0.0;
    current0 = 0.0;
    deltaPsc = 0.0;
    voltageSC = 0.0;
    nrOfTrafo = 0.0;
    plot = 0.0;
    deltaUp = 0.0;
    trafoReport=0.0;
    realLongitImpedance = 0.0;
    imagLongitImpedance = 0.0;
    realTransvAdmittance = 0.0;
    imagTransvAdmittance = 0.0;
}