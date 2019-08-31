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
 * nned to add  a way to check if i am making the same kernel with different ids    ex kernel1,id1; kernel1,id2
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

    //PSE_Power_Flow_Data pfData{};

    std::string clKernelSrcFile{"ocl_kernel_src.cl"};

    OCLC_Compute_Obj oclMain{clKernelSrcFile};
    if (!oclMain.success) {
        return 1;
    }
    std::vector<std::string> kernelNames{oclMain.GetKernelNames()};

    //oclMain.Run(0, output[id].data);
    int id{-66};

    for (const auto &i : kernelNames) {
        if (i == "pf_newton_raphson_init") {
            id = 0;

            oclMain.MakeKernel(i, id);

            oclMain.AddInputBuffer(id, elsys.GetNodesData());
/*
            output.emplace_back(E_Matlab_Data_Structure{elsys.NodesRowSize(),
                                                        elsys.NodesColSize(),
                                                        elsys.GetNodesData()});
 */
            oclMain.AddOutputBuffer(id, elsys.NodesRowSize() * elsys.NodesColSize());

            oclMain.SetKernelParameters(id);

            oclMain.SetRange(id,
                             cl::NullRange,
                             cl::NDRange{(size_t) elsys.NodesRowSize(), (size_t) elsys.NodesColSize()},
                             cl::NullRange);
        }
    }

    std::vector<double> relativeUnitsSystem(elsys.NodesRowSize() * elsys.NodesColSize(), 0);
    oclMain.Run(0, relativeUnitsSystem);
    //set new elsys
    elsys.SetNodesData(relativeUnitsSystem, elsys.NodesRowSize(), elsys.NodesColSize());

    //while loop start
    int step{0};
    int itermax{20};
    bool canStop{false};
    double eps{1e-3};
    PSE_Power_Flow_Data powerFlowData;

    std::vector<double> nominalGeneratorType;
    std::vector<double> currentIterGeneratorType;
    std::vector<double> nominalVoltageType;
    std::vector<double> nominalImposedQ{elsys.GetNodesCol(11)};

    while (!canStop) {

        std::vector<std::vector<double>> powerVectors;
        for (auto &i: kernelNames) {
            if (i == "pf_pf_newton_raphson_calculate_powers") {

                id = 1;

                if (!oclMain.KernelExists(id)) {
                    /*
                    output.emplace_back(E_Matlab_Data_Structure{elsys.NodesRowSize(), 1}); //activePow
                    output.emplace_back(E_Matlab_Data_Structure{elsys.NodesRowSize(), 1});//reactivePow
                    */
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

                        nominalVoltageType = powerFlowData.voltage;
                        nominalGeneratorType = elsys.GetNodesCol(1);
                        currentIterGeneratorType = elsys.GetNodesCol(1);
                    }
                } else {
                    oclMain.ClearBuffers(id);
                    powerVectors.clear();
                }

                oclMain.AddInputBuffer(id, powerFlowData.voltage);
                oclMain.AddInputBuffer(id, powerFlowData.angle);
                oclMain.AddInputBuffer(id, elsys.GetRealAdmittanceMatrix());
                oclMain.AddInputBuffer(id, elsys.GetImagAdmittanceMatrix());

                oclMain.AddOutputBuffer(id, elsys.NodesRowSize());
                oclMain.AddOutputBuffer(id, elsys.NodesRowSize());

                oclMain.SetKernelParameters(id);

                oclMain.SetRange(id,
                                 cl::NDRange(0),
                                 cl::NDRange{(size_t) elsys.NodesRowSize()},
                                 cl::NullRange);

            }
        }

        powerVectors.emplace_back(std::vector<double>(elsys.NodesRowSize(), 0));
        powerVectors.emplace_back(std::vector<double>(elsys.NodesRowSize(), 0));

        oclMain.Run(1, powerVectors);

        powerFlowData.activePower = powerVectors[0];
        powerFlowData.reactivePower = powerVectors[1];

        /** tratarea nodurilor generatoare **/

        if (step > 0) {

            currentIterGeneratorType = elsys.GetNodesCol(1);
            for (auto i = 0; i < elsys.NodesRowSize(); i++) {
                // if(elsys.GetNodesRowCol(i, 11)==(elsys.GetNodesRowCol(i, 9))- powerFlowData.reactivePower[i]){}
                if (nominalGeneratorType[i] == 1) {
                    double qcons = elsys.GetNodesRowCol(i,6);
                    double qdeb = powerFlowData.reactivePower[i]+qcons;
                    double qmin = elsys.GetNodesRowCol(i, 8)-qcons;
                    double qmax = elsys.GetNodesRowCol(i, 9)-qcons;

/*
                    if (currentIterGeneratorType[i] == 2) {
                        if (qimp == (qmax - qcons)) {
                            if (elsys.GetNodesRowCol(i, 2) >= nominalVoltageType[i]) {
                                currentIterGeneratorType[i] = 1;
                                relativeUnitsSystem[2*elsys.NodesRowSize()+i] = nominalVoltageType[i];
                            } else {
                                currentIterGeneratorType[i] = 2;
                            }
                        } else {
                            if (elsys.GetNodesRowCol(i, 2) <= nominalVoltageType[i]) {
                                currentIterGeneratorType[i] = 1;
                                relativeUnitsSystem[2*elsys.NodesRowSize()+i] = nominalVoltageType[i];
                            } else {
                                currentIterGeneratorType[i] = 2;
                            }
                        }
                    } else if(currentIterGeneratorType[i]==1){
*/
                    int idx = 11 * elsys.NodesRowSize() + i;
                    if (qdeb > qmax) {
                        relativeUnitsSystem[idx] = qmax;
                        currentIterGeneratorType[i] = 2;
                    } else if(qdeb<qmin){
                        relativeUnitsSystem[idx] = qmin;
                        currentIterGeneratorType[i] = 2;
                    } else {
                        currentIterGeneratorType[i] = 1;
                    }
                    idx = 1*elsys.NodesRowSize()+i;
                    relativeUnitsSystem[idx] = currentIterGeneratorType[i];
                }

            }
            elsys.SetNodesData(relativeUnitsSystem,elsys.NodesRowSize(),elsys.NodesColSize());
        }

        /**end tratarea nodurilor generatoare **/

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

        std::vector<std::vector<double>> JAC;

        int nrOfConsumerNodes{0};
        int nrOfGeneratorNodes{0};
        std::vector<double> nodeTypes;
        {
            for (auto i = 0; i < elsys.NodesRowSize(); i++) {
                nodeTypes.push_back(elsys.GetNodesCol(1)[i]);
            }
            for (const auto &i : nodeTypes) {
                if (i == 1) {
                    ++nrOfGeneratorNodes;
                }
                if (i == 2) {
                    ++nrOfConsumerNodes;
                }
            }
        }
        std::vector<double> pfDataVect;
        {
            for (const auto &i : powerFlowData.voltage) {
                pfDataVect.push_back(i);
            }
            for (const auto &i : powerFlowData.angle) {
                pfDataVect.push_back(i);
            }
            for (const auto &i : powerFlowData.activePower) {
                pfDataVect.push_back(i);
            }
            for (const auto &i : powerFlowData.reactivePower) {
                pfDataVect.push_back(i);
            }
        }
        /**  H **/
        std::vector<std::vector<double> > H;
        for (auto &i: kernelNames) {
            if (i == "pf_newton_raphson_H") {
                id = 2;

                if (!oclMain.KernelExists(id)) {
                    oclMain.MakeKernel(i, id);
                } else {
                    oclMain.ClearBuffers(id);
                }


                oclMain.AddInputBuffer(id, nodeTypes);
                oclMain.AddInputBuffer(id, pfDataVect);
                oclMain.AddInputBuffer(id, elsys.GetRealAdmittanceMatrix());
                oclMain.AddInputBuffer(id, elsys.GetImagAdmittanceMatrix());

                oclMain.AddOutputBuffer(id, (int) std::pow(elsys.NodesRowSize(), 2));

                oclMain.SetKernelParameters(id);

                oclMain.SetRange(id,
                                 cl::NullRange,
                                 cl::NDRange{(size_t) elsys.NodesRowSize(), (size_t) elsys.NodesRowSize()},
                                 cl::NullRange);

            }
        }

        std::vector<double> singleVecH((size_t) std::pow(elsys.NodesRowSize(), 2), 0);
        oclMain.Run(2, singleVecH);

        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] != 0) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    if (elsys.GetNodesCol(1)[j] != 0) {
                        int idx{i * elsys.NodesRowSize() + j};
                        temp.push_back(singleVecH[idx]);
                    }
                }
                H.emplace_back(temp);
            }
        }

        /*
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
        */

        /** K **/
        std::vector<std::vector<double> > K;

        for (auto &i: kernelNames) {
            if (i == "pf_newton_raphson_K") {
                id = 3;

                if (!oclMain.KernelExists(id)) {
                    oclMain.MakeKernel(i, id);
                } else {
                    oclMain.ClearBuffers(id);
                }

                oclMain.AddInputBuffer(id, nodeTypes);
                oclMain.AddInputBuffer(id, pfDataVect);
                oclMain.AddInputBuffer(id, elsys.GetRealAdmittanceMatrix());
                oclMain.AddInputBuffer(id, elsys.GetImagAdmittanceMatrix());

                oclMain.AddOutputBuffer(id, (int) std::pow(elsys.NodesRowSize(), 2));

                oclMain.SetKernelParameters(id);


                oclMain.SetRange(id,
                                 cl::NullRange,
                                 cl::NDRange{(size_t) elsys.NodesRowSize(), (size_t) elsys.NodesRowSize()},
                                 cl::NullRange);

            }
        }

        std::vector<double> singleVecK((size_t) std::pow(elsys.NodesRowSize(), 2), 0);
        oclMain.Run(3, singleVecK);

        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] != 0) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    if (elsys.GetNodesCol(1)[j] == 2) {
                        int idx{i * elsys.NodesRowSize() + j};
                        temp.push_back(singleVecK[idx]);
                    }
                }
                K.emplace_back(temp);
            }
        }

/*
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
*/
        std::vector<std::vector<double> > M;
        /** M **/
        for (auto &i: kernelNames) {
            if (i == "pf_newton_raphson_M") {
                id = 4;

                if (!oclMain.KernelExists(id)) {
                    oclMain.MakeKernel(i, id);
                } else {
                    oclMain.ClearBuffers(id);
                }

                oclMain.AddInputBuffer(id, nodeTypes);
                oclMain.AddInputBuffer(id, pfDataVect);
                oclMain.AddInputBuffer(id, elsys.GetRealAdmittanceMatrix());
                oclMain.AddInputBuffer(id, elsys.GetImagAdmittanceMatrix());

                oclMain.AddOutputBuffer(id, (int) std::pow(elsys.NodesRowSize(), 2));

                oclMain.SetKernelParameters(id);


                oclMain.SetRange(id,
                                 cl::NullRange,
                                 cl::NDRange{(size_t) elsys.NodesRowSize(), (size_t) elsys.NodesRowSize()},
                                 cl::NullRange);
            }
        }

        std::vector<double> singleVecM((size_t) std::pow(elsys.NodesRowSize(), 2), 0);
        oclMain.Run(4, singleVecM);

        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] == 2) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    if (elsys.GetNodesCol(1)[j] != 0) {
                        int idx{i * elsys.NodesRowSize() + j};
                        temp.push_back(singleVecM[idx]);
                    }
                }
                M.emplace_back(temp);
            }
        }
/*
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
*/
        /** L **/
        std::vector<std::vector<double> > L;

        for (auto &i: kernelNames) {
            if (i == "pf_newton_raphson_L") {
                id = 5;

                if (!oclMain.KernelExists(id)) {
                    oclMain.MakeKernel(i, id);
                } else {
                    oclMain.ClearBuffers(id);
                }

                oclMain.AddInputBuffer(id, nodeTypes);
                oclMain.AddInputBuffer(id, pfDataVect);
                oclMain.AddInputBuffer(id, elsys.GetRealAdmittanceMatrix());
                oclMain.AddInputBuffer(id, elsys.GetImagAdmittanceMatrix());

                oclMain.AddOutputBuffer(id, (int) std::pow(elsys.NodesRowSize(), 2));

                oclMain.SetKernelParameters(id);


                oclMain.SetRange(id,
                                 cl::NullRange,
                                 cl::NDRange{(size_t) elsys.NodesRowSize(), (size_t) elsys.NodesRowSize()},
                                 cl::NullRange);
            }
        }

        std::vector<double> singleVecL((size_t) std::pow(elsys.NodesRowSize(), 2), 0);
        oclMain.Run(5, singleVecL);

        for (int i = 0; i < elsys.NodesRowSize(); i++) {
            if (elsys.GetNodesCol(1)[i] == 2) {
                std::vector<double> temp;
                for (int j = 0; j < elsys.NodesRowSize(); j++) {
                    if (elsys.GetNodesCol(1)[j] == 2) {
                        int idx{i * elsys.NodesRowSize() + j};
                        temp.push_back(singleVecL[idx]);
                    }
                }
                L.emplace_back(temp);
            }
        }
/*
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
*/
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

        //inverse operation;
        std::vector<double> vecJAC;
        std::vector<double> singleVecJAC;

        for (int i = 0; i < JAC.size(); i++) {
            std::vector<double> col;
            for (int j = 0; j < JAC.size(); j++) {
                col.push_back(JAC[j][i]);
            }
            for (const auto &k: col) {
                vecJAC.push_back(k);
            }
        }
        auto row = JAC.size();
        auto col = JAC[0].size();
        {
            /** need to find a way to get rid of matlab; my own inverse function is broken! -->later**/
            mxArray *mxJAC = mxCreateDoubleMatrix(row, col, mxREAL);
            std::copy(vecJAC.begin(), vecJAC.end(), mxGetPr(mxJAC));

            mxArray *mxInvJAC;

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
        }

        for (auto &i : kernelNames) {
            if (i == "pf_newton_raphson_error_compute") {
                id = 9;
                if (!oclMain.KernelExists(id)) {
                    oclMain.MakeKernel(i, id);
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

            double qcons = elsys.GetNodesRowCol(i,6);
            double qdeb = powerFlowData.reactivePower[i]+qcons;
            double qmin = elsys.GetNodesRowCol(i, 8)-qcons;
            double qmax = elsys.GetNodesRowCol(i, 9)-qcons;
            double qimp = elsys.GetNodesRowCol(i,11);

            if (elsys.GetNodesCol(1)[i] == 2) {
                if(nominalGeneratorType[i]==2) {
                    double U = powerFlowData.voltage[i];
                    powerFlowData.voltage[i] += calculatedErrors[idx_plus] * U;
                    idx_plus++;
                }
                if(nominalGeneratorType[i]==1){
                    if(qimp == qmin){
                        if(powerFlowData.voltage[i]<elsys.GetNodesRowCol(i,7)){
                            currentIterGeneratorType[i]=1;
                            powerFlowData.voltage[i]=elsys.GetNodesRowCol(i,7);
                        }
                    }
                    if(qimp == qmax){
                        if(powerFlowData.voltage[i]>elsys.GetNodesRowCol(i,7)){
                            currentIterGeneratorType[i]=1;
                            powerFlowData.voltage[i]=elsys.GetNodesRowCol(i,7);
                        }
                    }
                }
                relativeUnitsSystem[1*elsys.NodesRowSize()+i] = currentIterGeneratorType[i];
            } else {
                powerFlowData.voltage[i] += 0.0;
            }

        }
        elsys.SetNodesData(relativeUnitsSystem,elsys.NodesRowSize(),elsys.NodesColSize());

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

    info = "DEBUGG END  MAIN FUNCTION!";
    LOG.Write(info);

    return 0;
}