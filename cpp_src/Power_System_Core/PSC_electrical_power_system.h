//
// Created by Petru on 19.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_PSC_ELECTRICAL_POWER_SYSTEM_H
#define NEWTON_RAPHSON_MATLAB_MEX_PSC_ELECTRICAL_POWER_SYSTEM_H

#include <algorithm>
#include "PSC_lines.h"
#include "PSC_nodes.h"

#include "../INFO/Logger.h"
#include "PSC_electrical_transformer.h"

class PSC_electrical_power_system {
    PSCL::PSC_lines lines;
    PSCN::PSC_nodes nodes;
    PSCT::PSC_electrical_transformer trafo;

    std::vector<std::vector<std::complex<double>>> admittanceMatrix;

public:
    bool success;

    explicit PSC_electrical_power_system(const std::vector<std::vector<double>> &inData) : lines{inData},
                                                                                           nodes{inData},
                                                                                           trafo{inData} {
        success = true;
        Logger::Logger LOG;
        std::string info;

        if (!lines.success) {
            success = false;
            info = "Lines OBJ could not be constructed!";
            LOG.Write(info);
            return;
        }
        if (!nodes.success) {
            success = false;
            info = "Lines OBJ could not be constructed!";
            LOG.Write(info);
            return;
        }
        if (!trafo.success) {
            success = false;
        }


        /***  admittance matrix ***/
        int baseApparentPower{100}; // MVA, standard

        std::vector<std::vector<std::complex<double> > > Ynn;

        std::vector<double> fromVct;
        std::vector<double> toVct;
        for (const auto &row : lines.GetInternalData()) {
            fromVct.push_back(row.from);
            toVct.push_back(row.to);
        }
        //this needs to chance to this.LinesRowSize() ?
        auto max1 = *std::max_element(std::begin(fromVct), std::end(fromVct));
        auto max2 = *std::max_element(std::begin(toVct), std::end(toVct));
        auto nrOfNodes = std::max(max1, max2);
        for (int i = 0; i < nrOfNodes; i++) {
            Ynn.emplace_back(std::vector<std::complex<double> >(static_cast<unsigned long>(nrOfNodes)));
        }
        //need  to implement whether the line is connected or not!
        for (const auto &row : lines.GetInternalData()) {
            int i = (int) row.from - 1;//[1;n] to go to [0;n-1]
            int j = (int) row.to - 1;//[1;n] to go to [0;n-1]


            std::complex<double> yL =
                    1.0 / std::complex<double>{row.realImpedance, row.imagImpedance};
            std::complex<double> yT = std::complex<double>{row.realAdmittance, row.imagAdmittance}; // why doesn't it give correct input if i  not 1/x?

            Ynn[i][i] += (yL + yT);
            Ynn[j][j] += (yL + yT);
            Ynn[i][j] += (-yL);
            Ynn[j][i] += (-yL);
        }
        //****newly added, need check!
        if (!inData[19].empty()) {
            for (const auto &row : trafo.GetInternalData()) {
                std::complex<double> yL = 1.0 / std::complex<double>{row.realLongitImpedance, row.imagLongitImpedance};
                std::complex<double> yT = std::complex<double>{row.realTransvAdmittance,row.imagTransvAdmittance};

                int i = (int) row.from - 1;//[1;n] to go to [0;n-1]
                int j = (int) row.to - 1;//[1;n] to go to [0;n-1]

                Ynn[i][i] += (yL);
                Ynn[j][j] += (yL+yT)*std::pow(row.trafoReport,2);
                Ynn[i][j] += (-yL*row.trafoReport);
                Ynn[j][i] += (-yL*row.trafoReport);
            }

        }


        admittanceMatrix = Ynn;
        /***  admittance matrix end***/

        info = "Electrical Power System OBJ successfully constructed!";
        LOG.Write(info);
    };

    ~PSC_electrical_power_system() = default;

    PSC_electrical_power_system &operator=(const PSC_electrical_power_system &sys) {
        lines = sys.lines;
        nodes = sys.nodes;

        admittanceMatrix = sys.admittanceMatrix;

        return *this;
    };

    int LinesRowSize() { return lines.RowSize(); };

    int LinesColSize() { return lines.ColSize(); };

    int NodesRowSize() { return nodes.RowSize(); };

    int NodesColSize() { return nodes.ColSize(); };

    const std::vector<double> GetLinesRow(const int &rowNr) { return lines.GetRow(rowNr); };

    const std::vector<double> GetLinesCol(const int &colNr) { return lines.GetCol(colNr); };

    const double GetLinesRowCol(const int &rowNr, const int &colNr) {
        return lines.GetRowCol(rowNr, colNr);
    };

    const std::vector<double> GetNodesRow(const int &rowNr) { return nodes.GetRow(rowNr); };

    const std::vector<double> GetNodesCol(const int &colNr) { return nodes.GetCol(colNr); };

    const double GetNodesRowCol(const int &rowNr, const int &colNr) {
        return nodes.GetRowCol(rowNr, colNr);
    };

    const std::vector<std::vector<double> > GetLinesData() { return lines.GetData(); };

    const std::vector<std::vector<double> > GetNodesData() { return nodes.GetData(); };

    const std::vector<std::vector<std::complex<double>>> GetAdmittanceMatrix() { return admittanceMatrix; };

    const std::vector<std::vector<double>> GetRealAdmittanceMatrix() {

        std::vector<std::vector<double>> real;
        for (const auto &i : admittanceMatrix) {
            std::vector<double> row__;
            for (const auto &j : i) {
                row__.push_back(j.real());
            }
            real.push_back(row__);
        }

        return real;
    };

    const std::vector<std::vector<double>> GetImagAdmittanceMatrix() {

        std::vector<std::vector<double>> imag;
        for (const auto &i : admittanceMatrix) {
            std::vector<double> row__;
            for (const auto &j : i) {
                row__.push_back(j.imag());
            }
            imag.push_back(row__);
        }

        return imag;
    };

    void SetNodesData(const std::vector<double> &nodesData, const int &rowS, const int &colS) {
        for (auto i = 0; i < rowS; i++) {

            nodes.rGetInternalData()[i].nodeNr = nodesData[0 * rowS + i];
            nodes.rGetInternalData()[i].nodeType = nodesData[1 * rowS + i];
            nodes.rGetInternalData()[i].nominalVoltage = nodesData[2 * rowS + i];
            nodes.rGetInternalData()[i].generatedActivePower = nodesData[3 * rowS + i];
            nodes.rGetInternalData()[i].generatedReactivePower = nodesData[4 * rowS + i];
            nodes.rGetInternalData()[i].consumedActivePower = nodesData[5 * rowS + i];
            nodes.rGetInternalData()[i].consumedReactivePower = nodesData[5 * rowS + i];
            nodes.rGetInternalData()[i].imposedVoltage = nodesData[7 * rowS + i];
            nodes.rGetInternalData()[i].minReactivePower = nodesData[8 * rowS + i];
            nodes.rGetInternalData()[i].maxReactivePower = nodesData[9 * rowS + i];
            nodes.rGetInternalData()[i].imposedActivePower = nodesData[10 * rowS + i];
            nodes.rGetInternalData()[i].imposedReactivePower = nodesData[11 * rowS + i];
        }
    };
};


#endif //NEWTON_RAPHSON_MATLAB_MEX_PSC_ELECTRICAL_POWER_SYSTEM_H

