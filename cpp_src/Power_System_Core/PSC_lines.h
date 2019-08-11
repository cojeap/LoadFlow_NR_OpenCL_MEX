//
// Created by Petru on 14.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_PSC_LINES_H
#define NEWTON_RAPHSON_MATLAB_MEX_PSC_LINES_H

#include <vector>
#include <complex>

namespace PSCL {

    class PSC_lines {
        struct LinesData {
            LinesData();

            ~LinesData() = default;

            double from;//0
            double to;//1
            double nominalVoltage;//2
            double lineLength;//3
            double resistance0;//4
            double reactance0;//5
            double conductance0;//6
            double susceptance0;//7
            double nrOfConductors;//8
            double realImpedance;//9
            double imagImpedance;//10
            double realAdmittance;//11
            double imagAdmittance;//12
        };

        std::vector<LinesData> data;

        int rowSize;
        int colSize;


    public:

        bool success;

        explicit PSC_lines(const std::vector<std::vector<double>> &inputData);

        ~PSC_lines() = default;

        PSC_lines &operator=(const PSC_lines &from);

        const int RowSize() { return rowSize; };

        const int RowSize() const { return rowSize; };

        const int ColSize() { return colSize; };

        const int ColSize() const { return colSize; };

        const std::vector<double> GetRow(const int &rowNumber);

        const std::vector<double> GetCol(const int &colNumber);

        const double GetRowCol(const int &rowNr, const int &colNr);

        const std::vector<std::vector<double>> GetData();

        std::vector<LinesData> GetInternalData() { return data; };

        std::vector<LinesData> GetInternalData() const { return data; };
    };

}
#endif //NEWTON_RAPHSON_MATLAB_MEX_PSC_LINES_H
