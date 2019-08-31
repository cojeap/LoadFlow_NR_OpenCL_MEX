/*
 * Created by Petru on 27.06.2019.
 *
 * Needs review.
 */

#ifndef NEWTON_RAPHSON_MATLAB_MEX_PSC_ELECTRICAL_TRANSFORMER_H
#define NEWTON_RAPHSON_MATLAB_MEX_PSC_ELECTRICAL_TRANSFORMER_H

#include <vector>

namespace PSCT {
    class PSC_electrical_transformer {
        struct TrafoData{
            TrafoData();
            ~TrafoData()=default;

            double from;
            double to;
            double voltage_from;
            double voltage_to;
            double apparentPower;
            double deltaP0;
            double current0;
            double deltaPsc;
            double voltageSC;
            double nrOfTrafo;
            double plot;
            double deltaUp;

            double trafoReport;
            /** longitudinal parameters **/
            double realLongitImpedance;
            double imagLongitImpedance;
            /** transversal parameters **/

            double realTransvAdmittance;
            double imagTransvAdmittance;
        };

        std::vector<TrafoData> data;

        int rowSize;
        int colSize;

    public:

        bool success;

        explicit PSC_electrical_transformer(const std::vector<std::vector<double>> &inputData);
        ~PSC_electrical_transformer()=default;

        PSC_electrical_transformer &operator=(const PSC_electrical_transformer &from);

        const int RowSize() { return rowSize; };

        const int RowSize() const { return rowSize; };

        const int ColSize() { return colSize; };

        const int ColSize() const { return colSize; };

        const std::vector<double> GetRow(const int &rowNumber);

        const std::vector<double> GetCol(const int &colNumber);

        const double GetRowCol(const int &rowNr, const int &colNr);

        const std::vector<std::vector<double>> GetData();


        std::vector<TrafoData> GetInternalData() { return data; };

        std::vector<TrafoData> GetInternalData() const { return data; };
    };
}

#endif //NEWTON_RAPHSON_MATLAB_MEX_PSC_ELECTRICAL_TRANSFORMER_H
