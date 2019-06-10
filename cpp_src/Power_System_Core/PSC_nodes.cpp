//
// Created by Petru on 13.04.2019.
//

#include "PSC_nodes.h"

using namespace PSCN;

PSC_nodes::PSC_nodes(const std::vector<std::vector<double>> &inputData) {
    success = true;

    if (inputData.empty()) {
        success = false;
        return;
    }
    if (inputData[0].empty()) {
        success = false;
        return;
    }
    if (inputData.size() < 10) {
        /*
         * The number 10 is chosen as there are 10 inputs for nodes from Matlab
         * They are expected to be the first set of inputs and in order
         *
         * */
        success = false;
        return;
    }
    rowSize = (int) inputData[0].size();
    colSize = 12;

    for (int i = 0; i < inputData[0].size(); i++) {
        data.emplace_back(NodesData{});

        data[i].nodeNr = inputData[0][i];
        data[i].nodeType = inputData[1][i];
        data[i].nominalVoltage = inputData[2][i];
        data[i].generatedActivePower = inputData[3][i];
        data[i].generatedReactivePower = inputData[4][i];
        data[i].consumedActivePower = inputData[5][i];
        data[i].consumedReactivePower = inputData[6][i];
        data[i].imposedVoltage = inputData[7][i];
        data[i].minReactivePower = inputData[8][i];
        data[i].maxReactivePower = inputData[9][i];
        data[i].imposedActivePower = 0;
        data[i].imposedReactivePower = 0;
    }
}

PSC_nodes::NodesData::NodesData() {
    nodeNr = 0.0;
    nodeType = 0.0;
    nominalVoltage = 0.0;
    generatedActivePower = 0.0;
    generatedReactivePower = 0.0;
    consumedActivePower = 0.0;
    consumedReactivePower = 0.0;
    imposedVoltage = 0.0;
    minReactivePower = 0.0;
    maxReactivePower = 0.0;
    imposedActivePower = 0.0;
    imposedReactivePower = 0.0;
}

const double PSC_nodes::GetRowCol(const int &rowNr, const int &colNr) {
    NodesData &row = data[rowNr];

    double colData{0.0};
    switch (colNr) {
        case 0:
            colData = row.nodeNr;
            break;
        case 1:
            colData = row.nodeType;
            break;
        case 2:
            colData = row.nominalVoltage;
            break;
        case 3:
            colData = row.generatedActivePower;
            break;
        case 4:
            colData = row.generatedReactivePower;
            break;
        case 5:
            colData = row.consumedActivePower;
            break;
        case 6:
            colData = row.consumedReactivePower;
            break;
        case 7:
            colData = row.imposedVoltage;
            break;
        case 8:
            colData = row.minReactivePower;
            break;
        case 9:
            colData = row.maxReactivePower;
            break;
        case 10:
            colData = row.imposedActivePower;
            break;
        case 11:
            colData = row.imposedReactivePower;
            break;
        default:
            return (-6 * 10e10);
    }
    return colData;
}

const std::vector<double> PSC_nodes::GetRow(const int &rowNumber) {
    std::vector<double> row;

    row.push_back(data[rowNumber].nodeNr);
    row.push_back(data[rowNumber].nodeType);
    row.push_back(data[rowNumber].nominalVoltage);
    row.push_back(data[rowNumber].generatedActivePower);
    row.push_back(data[rowNumber].generatedReactivePower);
    row.push_back(data[rowNumber].consumedActivePower);
    row.push_back(data[rowNumber].consumedReactivePower);
    row.push_back(data[rowNumber].imposedVoltage);
    row.push_back(data[rowNumber].minReactivePower);
    row.push_back(data[rowNumber].maxReactivePower);
    row.push_back(data[rowNumber].imposedActivePower);
    row.push_back(data[rowNumber].imposedReactivePower);

    return row;
}

const std::vector<double> PSC_nodes::GetCol(const int &colNumber) {
    std::vector<double> col;
    int i{0};
    while (col.size() < rowSize) {
        col.push_back(GetRowCol(i, colNumber));
        i++;
    }
    return col;
}

const std::vector<std::vector<double> > PSC_nodes::GetData() {
/*
    std::vector<std::vector<double> > tData;

    for (const auto &i : data) {
        std::vector<double> tVec;

        tVec.push_back(i.nodeNr);
        tVec.push_back(i.nodeType);
        tVec.push_back(i.nominalVoltage);
        tVec.push_back(i.generatedActivePower);
        tVec.push_back(i.generatedReactivePower);
        tVec.push_back(i.consumedActivePower);
        tVec.push_back(i.consumedReactivePower);
        tVec.push_back(i.imposedVoltage);
        tVec.push_back(i.minReactivePower);
        tVec.push_back(i.maxReactivePower);
        tVec.push_back(i.imposedActivePower);
        tVec.push_back(i.imposedReactivePower);

        tData.emplace_back(tVec);

        return tData;
    }
*/

    std::vector<std::vector<double>> nodesData;
    for (int i = 0; i < colSize; i++) {
        nodesData.emplace_back(std::vector<double>(rowSize));
    }
    for (int i = 0; i < rowSize; i++) {
        nodesData[0][i] = data[i].nodeNr;
        nodesData[1][i] = data[i].nodeType;
        nodesData[2][i] = data[i].nominalVoltage;
        nodesData[3][i] =data[i].generatedActivePower;
        nodesData[4][i] = data[i].generatedReactivePower;
        nodesData[5][i] = data[i].consumedActivePower;
        nodesData[6][i] = data[i].consumedReactivePower;
        nodesData[7][i] =data[i].imposedVoltage;
        nodesData[8][i] = data[i].minReactivePower;
        nodesData[9][i] = data[i].maxReactivePower;
        nodesData[10][i] = data[i].imposedActivePower;
        nodesData[11][i] = data[i].imposedReactivePower;
    }

    return nodesData;

}

PSC_nodes &PSC_nodes::operator=(const PSC_nodes &from) {

    if (!data.empty()) {
        data.clear();
    }

    data = from.GetInternalData();
    rowSize = from.RowSize();
    colSize = from.ColSize();

    return *this;
}

