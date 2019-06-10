//
// Created by Petru on 14.04.2019.
//

#include "PSC_lines.h"

using namespace PSCL;

PSC_lines::PSC_lines(const std::vector<std::vector<double>> &inputData) {
    success = true;
    if (inputData.empty()) {
        success = false;
        return;
    }
    if (inputData[10].empty()) {
        success = false;
        return;
    }
    if (inputData.size() < 19) {
        /*
         * The number 19 is chosen as 10-19 are the inputs for lines from Matlab
         * They are expected to be the last set of inputs and in order
         *
         * */
        success = false;
        return;
    }
    rowSize = (int) inputData[10].size();
    colSize = 13;

    for (int i = 0; i < inputData[10].size(); i++) {
        data.emplace_back(LinesData{});

        data[i].from = inputData[10][i];
        data[i].to = inputData[11][i];
        data[i].nominalVoltage = inputData[12][i];
        data[i].lineLength = inputData[13][i];
        data[i].resistance0 = inputData[14][i];
        data[i].reactance0 = inputData[15][i];
        data[i].conductance0 = inputData[16][i];
        data[i].susceptance0 = inputData[17][i];
        data[i].nrOfConductors = inputData[18][i];

        data[i].realImpedance = data[i].lineLength * data[i].resistance0;
        data[i].imagImpedance = data[i].lineLength * data[i].reactance0;

        data[i].realAdmittance = data[i].lineLength * data[i].conductance0;
        data[i].imagAdmittance = data[i].lineLength * data[i].susceptance0;

    }
}

PSC_lines::LinesData::LinesData() {
    from = 0.0;
    to = 0.0;
    nominalVoltage = 0.0;
    lineLength = 0.0;
    resistance0 = 0.0;
    reactance0 = 0.0;
    conductance0 = 0.0;
    susceptance0 = 0.0;
    nrOfConductors = 0.0;
    realImpedance = 0.0;
    imagImpedance = 0.0;
    realAdmittance = 0.0;
    imagAdmittance = 0.0;
}

const double PSC_lines::GetRowCol(const int &rowNr, const int &colNr) {
    LinesData &row{data[rowNr]};

    double colData{0.0};
    switch (colNr) {
        case 0:
            colData = row.from;
            break;
        case 1:
            colData = row.to;
            break;
        case 2:
            colData = row.nominalVoltage;
            break;
        case 3:
            colData = row.lineLength;
            break;
        case 4:
            colData = row.resistance0;
            break;
        case 5:
            colData = row.reactance0;
            break;
        case 6:
            colData = row.conductance0;
            break;
        case 7:
            colData = row.susceptance0;
            break;
        case 8:
            colData = row.nrOfConductors;
            break;
        case 9:
            colData = row.realImpedance;
            break;
        case 10:
            colData = row.imagImpedance;
            break;
        case 11:
            colData = row.realAdmittance;
            break;
        case 12:
            colData = row.imagAdmittance;
            break;
        default:
            return (-6 * 10e10);
    }
    return colData;

}

const std::vector<double> PSC_lines::GetRow(const int &rowNumber) {
    std::vector<double> row;

    row.push_back(data[rowNumber].from);
    row.push_back(data[rowNumber].to);
    row.push_back(data[rowNumber].nominalVoltage);
    row.push_back(data[rowNumber].lineLength);
    row.push_back(data[rowNumber].resistance0);
    row.push_back(data[rowNumber].reactance0);
    row.push_back(data[rowNumber].conductance0);
    row.push_back(data[rowNumber].susceptance0);
    row.push_back(data[rowNumber].nrOfConductors);
    row.push_back(data[rowNumber].realImpedance);
    row.push_back(data[rowNumber].imagImpedance);
    row.push_back(data[rowNumber].realAdmittance);
    row.push_back(data[rowNumber].imagAdmittance);

    return row;
}

const std::vector<double> PSC_lines::GetCol(const int &colNumber) {
    std::vector<double> col;
    int i{0};
    while (col.size() <= rowSize) {
        col.push_back(GetRowCol(i, colNumber));
        i++;
    }

    return col;

}

const std::vector<std::vector<double>> PSC_lines::GetData() {
    std::vector<std::vector<double>> tData;

    for (const auto &i : data) {
        std::vector<double> tVec;

        tVec.push_back(i.from);
        tVec.push_back(i.to);
        tVec.push_back(i.nominalVoltage);
        tVec.push_back(i.lineLength);
        tVec.push_back(i.resistance0);
        tVec.push_back(i.reactance0);
        tVec.push_back(i.conductance0);
        tVec.push_back(i.susceptance0);
        tVec.push_back(i.nrOfConductors);
        tVec.push_back(i.realImpedance);
        tVec.push_back(i.imagImpedance);
        tVec.push_back(i.realAdmittance);
        tVec.push_back(i.imagAdmittance);

        tData.emplace_back(tVec);
    }

    return tData;
}


PSC_lines &PSC_lines::operator=(const PSC_lines &from) {

    if (!data.empty()) {
        data.clear();
    }

    data = from.GetInternalData();
    rowSize = from.RowSize();
    colSize = from.ColSize();

    return *this;
}
