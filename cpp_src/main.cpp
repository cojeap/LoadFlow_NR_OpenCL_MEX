//
// Created by Petru on 30.04.2019.
//
/*
 *
 * need a better way to see what number the output index is where and when and then to use it there
 *
 *
 * reminder: write doc to all this before it's too late
 *
 * */
#include "mex_Newton_Raphson_OpenCL.h"

int main(const std::vector<std::vector<double>> &input,
         std::vector<E_Matlab_Data_Structure> &output) {

    Logger::Logger LOG;

    std::string info = "DEBUGG START MAIN FUNCTION!";
    LOG.Write(info);

    if (!output.empty()) {
        info = "Expected Matlab output to be empty!";
        LOG.Write(info);
    }
    PSC_electrical_power_system elsys{input};

    PSE_Power_Flow_Data pfData{};

    std::string clKernelSrcFile{"ocl_kernel_src.cl"};

    OCLC_Compute_Obj oclMain{clKernelSrcFile};
    if (!oclMain.success) {
        return 1;
    }
    std::vector<std::string> kernelNames{oclMain.GetKernelNames()};

    //oclMain.Run(0, output[id].data);
    int idx{(int) output.size()};
    int id{-66};

    for (const auto &i : kernelNames) {
        if (i == "pf_newton_raphson_init") {
            id = 0;

            oclMain.MakeKernel(i, id);

            oclMain.AddInputBuffer(id, elsys.GetNodesData());

            output.emplace_back(E_Matlab_Data_Structure{elsys.NodesRowSize(),
                                                        elsys.NodesColSize(),
                                                        elsys.GetNodesData()});
            oclMain.AddOutputBuffer(id, output[0].data.size());
            idx++;

            oclMain.SetKernelParameters(id);

            oclMain.SetRange(id,
                             cl::NullRange,
                             cl::NDRange{(size_t) output[0].rowSize, (size_t) output[0].colSize},
                             cl::NullRange);
        }
    }
    oclMain.Run(0, output[0].data);
    //set new elsys
    elsys.SetNodesData(output[0].data, output[0].rowSize, output[0].colSize);

    //while loop start
    int step{0};
    int itermax{20};
    bool canStop{false};
    double eps{1e-3};
    PSE_Power_Flow_Data powerFlowData;

    while (!canStop) {

        std::vector<std::vector<double>> powerVectors;
        for (auto &i: kernelNames) {
            if (i == "pf_pf_newton_raphson_calculate_powers") {

                id = 1;

                if (!oclMain.KernelExists(id)) {
                    output.emplace_back(E_Matlab_Data_Structure{elsys.NodesRowSize(), 1}); //activePow
                    idx++;
                    output.emplace_back(E_Matlab_Data_Structure{elsys.NodesRowSize(), 1});//reactivePow
                    idx++;

                    oclMain.MakeKernel(i, id);

                    if (step < 1) {
                        std::vector<double> voltage;
                        for (auto i = 0; i < elsys.NodesRowSize(); i++) {
                            if (elsys.GetNodesCol(1)[i] != 2) {
                                voltage.push_back(elsys.GetNodesCol(7)[i]);
                            } else {
                                voltage.push_back(elsys.GetNodesCol(2)[i]);
                            }
                        }
                        std::vector<double> teta(elsys.NodesRowSize(), 0);
                        powerFlowData.voltage = voltage;
                        powerFlowData.angle = teta;
                    }
                } else {
                    oclMain.ClearBuffers(id);
                    powerVectors.clear();
                }

                oclMain.AddInputBuffer(id, powerFlowData.voltage);
                oclMain.AddInputBuffer(id, powerFlowData.angle);
                oclMain.AddInputBuffer(id, elsys.GetRealAdmittanceMatrix());
                oclMain.AddInputBuffer(id, elsys.GetImagAdmittanceMatrix());

                oclMain.AddOutputBuffer(id, output[1].data.size());
                oclMain.AddOutputBuffer(id, output[2].data.size());

                powerVectors.emplace_back(output[1].data);
                powerVectors.emplace_back(output[2].data);

                oclMain.SetKernelParameters(id);

                oclMain.SetRange(id,
                                 cl::NDRange(0),
                                 cl::NDRange{(size_t) output[id].rowSize},
                                 cl::NullRange);

            }
        }

        oclMain.Run(1, powerVectors);

        output[1].data = powerVectors[0];
        output[2].data = powerVectors[1];

        powerFlowData.activePower = powerVectors[0];
        powerFlowData.reactivePower = powerVectors[1];

/*

    std::vector<double> powsdasdads;
    std::vector<double> repowasdasd;
    for (int i = 0; i < elsys.NodesRowSize(); i++) {
        double P{0}, Q{0};
        for (int j = 0; j < elsys.NodesRowSize(); j++) {
            //(G(i,k)*cos(teta(i,p)-teta(k,p))+B(i,k)*sin(teta(i,p)-teta(k,p))));
            double Yp = elsys.GetRealAdmittanceMatrix()[i][j] * cos(powerFlowData.angle[i] - powerFlowData.angle[j]) +
                    elsys.GetImagAdmittanceMatrix()[i][j] * sin(powerFlowData.angle[i] - powerFlowData.angle[j]);

            P += powerFlowData.voltage[i] * powerFlowData.voltage[j] * Yp;

            double Yq = elsys.GetRealAdmittanceMatrix()[i][j] * sin(powerFlowData.angle[i] - powerFlowData.angle[j]) -
                    elsys.GetImagAdmittanceMatrix()[i][j] * cos(powerFlowData.angle[i] - powerFlowData.angle[j]);

            Q += powerFlowData.voltage[i] * powerFlowData.voltage[j] * Yq;

        }

        powsdasdads.push_back(P);
        repowasdasd.push_back(Q);
    }
*/

        std::vector<double> deltaP{}, deltaQ{}, deltaPQ{};
        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            deltaP.push_back(elsys.GetNodesRowCol(i, 10) - powerFlowData.activePower[i]);
            deltaQ.push_back(elsys.GetNodesRowCol(i, 11) - powerFlowData.reactivePower[i]);
        }


        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] != 0) {
                deltaPQ.push_back(deltaP[i]);
            }
        }
        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] == 2) {
                deltaPQ.push_back(deltaQ[i]);
            }
        }

        //test convergenta added on 14.05

        std::vector<std::vector<double>> JAC;

        /**  H **/
        std::vector<std::vector<double> > H;

        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] != 0) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    double val{NAN};
                    double dTeta = {powerFlowData.angle[i] - powerFlowData.angle[j]};
                    if (elsys.GetNodesCol(1)[j] != 0) {
                        if (i == j) {
                            val = -powerFlowData.reactivePower[i] -
                                  elsys.GetImagAdmittanceMatrix()[i][i] * std::pow(powerFlowData.voltage[i], 2);
                        } else {
                            val = -powerFlowData.voltage[i] * powerFlowData.voltage[j] *
                                  (elsys.GetImagAdmittanceMatrix()[i][j] * cos(dTeta) -
                                   elsys.GetRealAdmittanceMatrix()[i][j] * sin(dTeta));
                        }
                        temp.push_back(val);
                    }
                }
                H.emplace_back(temp);
            }
        }

        /**  K **/
        std::vector<std::vector<double> > K;

        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] != 0) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    double val{NAN};
                    double dTeta = {powerFlowData.angle[i] - powerFlowData.angle[j]};

                    if (elsys.GetNodesCol(1)[j] == 2) {
                        if (i == j) {
                            val = powerFlowData.activePower[i] +
                                  elsys.GetRealAdmittanceMatrix()[i][i] * std::pow(powerFlowData.voltage[i], 2);
                        } else {
                            val = powerFlowData.voltage[i] * powerFlowData.voltage[j] *
                                  (elsys.GetRealAdmittanceMatrix()[i][j] * cos(dTeta) +
                                   elsys.GetImagAdmittanceMatrix()[i][j] * sin(dTeta));
                        }
                        temp.push_back(val);
                    }
                }
                K.emplace_back(temp);
            }
        }

        /** M **/
        std::vector<std::vector<double> > M;
        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] == 2) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    double val{NAN};

                    double dTeta = {powerFlowData.angle[i] - powerFlowData.angle[j]};

                    if (elsys.GetNodesCol(1)[j] != 0) {
                        if (i == j) {
                            val = powerFlowData.activePower[i] -
                                  elsys.GetRealAdmittanceMatrix()[i][i] * std::pow(powerFlowData.voltage[i], 2);
                        } else {
                            val = -powerFlowData.voltage[i] * powerFlowData.voltage[j] *
                                  (elsys.GetRealAdmittanceMatrix()[i][j] * cos(dTeta) +
                                   elsys.GetImagAdmittanceMatrix()[i][j] * sin(dTeta));
                        }
                        temp.push_back(val);
                    }
                }
                M.emplace_back(temp);
            }
        }
        /** L **/
        std::vector<std::vector<double> > L;
        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] == 2) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    double val{NAN};

                    double dTeta = {powerFlowData.angle[i] - powerFlowData.angle[j]};

                    if (elsys.GetNodesCol(1)[j] == 2) {
                        if (i == j) {
                            val = powerFlowData.reactivePower[i] -
                                  elsys.GetImagAdmittanceMatrix()[i][i] * std::pow(powerFlowData.voltage[i], 2);
                        } else {
                            val = -powerFlowData.voltage[i] * powerFlowData.voltage[j] *
                                  (elsys.GetImagAdmittanceMatrix()[i][j] * cos(dTeta) -
                                   elsys.GetRealAdmittanceMatrix()[i][j] * sin(dTeta));
                        }
                        temp.push_back(val);
                    }
                }
                L.emplace_back(temp);
            }
        }

        //bugged? K and M get swapped? row and col get swapped? ???? why // -> rewrite remaking JAC
        if (H.size() != K.size()) {
            std::string msg = "H K size mismatch";
            LOG.Write(msg);
            return -1;
        }
        for (int i = 0; i < H.size(); i++) {
            std::vector<double> row;
            for (auto &j: H[i]) {
                row.push_back(j);
            }
            for (auto &j : K[i]) {
                row.push_back(j);
            }
            JAC.push_back(row);
        }
        if (M.size() != L.size()) {
            std::string msg = "M L size mismatch";
            LOG.Write(msg);
            return -1;
        }
        for (int i = 0; i < M.size(); i++) {
            std::vector<double> row;
            for (auto &j : M[i]) {
                row.push_back(j);
            }
            for (auto &j : L[i]) {
                row.push_back(j);
            }
            JAC.push_back(row);
        }
        if (JAC.size() != JAC[0].size()) {
            std::string msg = "JAC size mismatch";
            LOG.Write(msg);
            return -1;
        }


        std::vector<double> real, imag;
        for (const auto &i :elsys.GetRealAdmittanceMatrix()) {
            for (const auto &j: i) {
                real.emplace_back(j);
            }
        }
        for (const auto &i :elsys.GetImagAdmittanceMatrix()) {
            for (const auto &j: i) {
                imag.emplace_back(j);
            }
        }
        if (step < 1) {
            output.emplace_back(E_Matlab_Data_Structure{(int) elsys.GetRealAdmittanceMatrix().size(),
                                                        (int) elsys.GetRealAdmittanceMatrix()[0].size(),
                                                        real});
            output.emplace_back(E_Matlab_Data_Structure{(int) elsys.GetImagAdmittanceMatrix().size(),
                                                        (int) elsys.GetImagAdmittanceMatrix()[0].size(),
                                                        imag});
        } else { //these don't chage, not needed!
            output[3] = E_Matlab_Data_Structure{(int) elsys.GetRealAdmittanceMatrix().size(),
                                                (int) elsys.GetRealAdmittanceMatrix()[0].size(),
                                                real};
            output[4] = E_Matlab_Data_Structure{(int) elsys.GetImagAdmittanceMatrix().size(),
                                                (int) elsys.GetImagAdmittanceMatrix()[0].size(),
                                                imag};
        }

        /* -> bugged, the matrix is sent transposed to matlab!!!
         */
        std::vector<double> vecJAC;
        /*
        for (const auto &i: JAC) {
            for (const auto &j: i) {
                vecJAC.push_back(j);
            }
        }
        */
        /**added on 14.05**/
        /*
        for (auto i = 0; i < H.size(); i++) {
            for (const auto &j : JAC[i]) {
                vecJAC.push_back(j);
            }
        }
        for (auto i = H.size(); i < JAC.size(); i++) {
            for (const auto &j : JAC[i]) {
                vecJAC.push_back(j);
            }
        }
        // same as above....
         */
        for(int i=0;i<JAC.size();i++){
            std::vector<double> col;
            for(int j=0;j<JAC.size();j++){
                col.push_back(JAC[j][i]);
            }
            for(const auto &k: col){
                vecJAC.push_back(k);
            }
        }
        /**added on 14.05 end**/

        if (step < 1) {
            output.emplace_back(E_Matlab_Data_Structure{(int) JAC.size(),
                                                        (int) JAC[0].size(),
                                                        vecJAC});
        } else {
            output[5] = E_Matlab_Data_Structure{(int) JAC.size(),
                                                (int) JAC[0].size(),
                                                vecJAC};
        }

        //inverse operation;
        auto row = JAC.size();
        auto col = JAC[0].size();


        mxArray *mxJAC = mxCreateDoubleMatrix(row, col, mxREAL);
        std::copy(vecJAC.begin(), vecJAC.end(), mxGetPr(mxJAC));

        mxArray *mxInvJAC;
        std::vector<double> singleVecJAC;
        auto err = mexCallMATLAB(1, &mxInvJAC, 1, &mxJAC, "inv");
        //in case the default call is done to callmatlab_with_trap then an error code is returned
        row = mxGetM(mxInvJAC);
        col = mxGetN(mxInvJAC);
        for (int i = 0; i < row * col; i++) {
            singleVecJAC.emplace_back(mxGetPr(mxInvJAC)[i]);
        }

        mxDestroyArray(mxJAC);
        mxDestroyArray(mxInvJAC);
        //inverse operation done

        if (step < 1) {
            output.emplace_back(E_Matlab_Data_Structure{(int) row, (int) col, singleVecJAC});
        } else {
            output[6] = E_Matlab_Data_Structure{(int) row, (int) col, singleVecJAC};
        }
        for (auto &i : kernelNames) {
            if (i == "pf_newton_raphson_error_compute") {
                id = 9;
                if (!oclMain.KernelExists(id)) {
                    oclMain.MakeKernel(i, id);
                    output.emplace_back(E_Matlab_Data_Structure{(int) JAC.size(), 1});
                    idx++;
                } else {
                    oclMain.ClearBuffers(id);
                }

                oclMain.AddInputBuffer(id, singleVecJAC);
                oclMain.AddInputBuffer(id, deltaPQ);

                oclMain.AddOutputBuffer(id, JAC.size());

                oclMain.SetKernelParameters(id);

                oclMain.SetRange(id,
                                 cl::NDRange(0),
                                 cl::NDRange{(size_t) JAC.size()},
                                 cl::NullRange);
            }
        }

        std::vector<double> calculatedErrors(JAC.size(), 0);

        oclMain.Run(id, calculatedErrors);

        output[7].data = calculatedErrors;

        //determining new values
        int idx_plus{0};
        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] != 0) {
                powerFlowData.angle[i] += calculatedErrors[idx_plus];
                idx_plus++;
            } else {
                powerFlowData.angle[i] += 0.0;
            }
        }
        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] == 2) {
                double U = powerFlowData.voltage[i];
                powerFlowData.voltage[i] += calculatedErrors[idx_plus] * U;
                idx_plus++;

            } else {
                powerFlowData.voltage[i] += 0.0;
            }
        }

        info = std::to_string(step);
        LOG.Write(info);

        step++;
        if (step >= itermax) {
            canStop = true;
        }
        if (*std::max_element(std::begin(deltaPQ), std::end(deltaPQ)) < eps) {
            canStop = true;
        }

    }
    info = "DEBUGG END MAIN FUNCTION!";
    LOG.Write(info);

    std::vector<double> pfResults;
    for (auto &i : powerFlowData.voltage) {
        pfResults.push_back(i);
    }
    for (auto &i : powerFlowData.angle) {
        pfResults.push_back(i);
    }
    for (auto &i : powerFlowData.activePower) {
        pfResults.push_back(i);
    }
    for (auto &i : powerFlowData.reactivePower) {
        pfResults.push_back(i);
    }

    E_Matlab_Data_Structure outPfResults{(int) powerFlowData.voltage.size(), 4, pfResults};
    output.emplace_back(outPfResults);


    return 0;
}