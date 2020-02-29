//
// Created by Petru on 11.10.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_DATATYPES_H
#define NEWTON_RAPHSON_MATLAB_MEX_DATATYPES_H


enum class DataTypes : int {
    VECTOR_1D = 0,
    VECTOR_2D=1,
    MATLAB_DATA_MATRIX,
    OPENCL_BUFFER_1D,
    OPENCL_BUFFER_2D
};

enum class BUS : int {
    NUMBER = 0,
    TYPE = 1,
    NOMINAL_VOLTAGE,
    GEN_ACTIVE_POW,
    GEN_REACTIVE_POW,
    CONS_ACTIVE_POW,
    CONS_REACTIVE_POW,
    IMPOSED_VOLTAGE,
    MIN_REACTIVE_POW,
    MAX_REACTIVE_POW,
    IMPOSED_ACTIVE_POW,
    IMPOSED_REACTIVE_POW
};

enum class BRANCH : int {
    FROM = 0,
    TO = 1,
    NOMINAL_VOLTAGE,
    LINE_lENGTH,
    RESISTANCE_0,
    REACTANCE_0,
    CONDUCTANCE_0,
    SUSCEPTANCE_0,
    NR_OF_CONDUCTORS
};

enum class TRANSFORMER : int {
    FROM = 0,
    TO = 1,
    RESISTANCE_0,
    REACTANCE_0,
    CONDUCTANCE_0,
    SUSCEPTANCE_0,
    TRAFO_REPORT,
    NR_OF_TRAFO
};

enum class GENERATOR : int {
    //nothing for now
};

#endif //NEWTON_RAPHSON_MATLAB_MEX_DATATYPES_H
