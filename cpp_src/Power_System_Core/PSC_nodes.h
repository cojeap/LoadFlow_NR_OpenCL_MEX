//
// Created by Petru on 13.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_PSC_NODES_H
#define NEWTON_RAPHSON_MATLAB_MEX_PSC_NODES_H

#include <vector>
namespace PSCN{
class PSC_nodes {
    struct NodesData {
        NodesData();

        ~NodesData() = default;

        double nodeNr;//0
        double nodeType;//1
        double nominalVoltage;//2
        double generatedActivePower;//3
        double generatedReactivePower;//4
        double consumedActivePower;//5
        double consumedReactivePower;//6
        double imposedVoltage;//7
        double minReactivePower;//8
        double maxReactivePower;//9
        double imposedActivePower;//10
        double imposedReactivePower;//11

    };

    std::vector<NodesData> data;

    int rowSize;
    int colSize;


public:
    bool success;

    explicit PSC_nodes(const std::vector<std::vector<double>> &inputData);

    ~PSC_nodes() = default;

    PSC_nodes &operator=(const PSC_nodes &from);

    const int RowSize() { return rowSize; };

    const int RowSize() const { return rowSize; };

    const int ColSize() { return colSize; };

    const int ColSize() const { return colSize; };

    const std::vector<double> GetRow(const int &rowNumber);

    const std::vector<double> GetCol(const int &colNumber);

    const double GetRowCol(const int &rowNr, const int &colNr);

    const std::vector<std::vector<double> > GetData();

    std::vector<NodesData> GetInternalData() { return data; };

    std::vector<NodesData> GetInternalData() const { return data; };

    std::vector<NodesData> & rGetInternalData() {return data;};

};
}

#endif //NEWTON_RAPHSON_MATLAB_MEX_PSC_NODES_H
