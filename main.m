%{
%
%   Writen by Petru Cojea on 22.04.2019
%
%}

addpath('input','src');


load_data;

starttimeMEX=cputime;

[out1_sysrelunits, out2_P, out3_Q, out4_real,out5_imag,out6_JAC,out7_invJAC,out8_errorPQ,out9_pfResults] = mex_Newton_Raphson_OpenCL(...
    networkData.nodeNr,...
    networkData.nodeType,...
    networkData.nominalVoltage,...
    networkData.activePower,...
    networkData.reactivePower,...
    networkData.consumedActivePower,...
    networkData.consumedReactivePower,...
    networkData.imposedVoltage,...
    networkData.minimumReactivePower,...
    networkData.maximumReactivePower,...
    lineData.fromNode,...
    lineData.toNode,...
    lineData.nominalvoltage,...
    lineData.length,...
    lineData.resistance0,...
    lineData.reactance0,...
    lineData.conductance0,...
    lineData.susceptance0,...
    lineData.nrOfConductors...
    );
stoptimeMEX=cputime;

durataMEX = stoptimeMEX-starttimeMEX;


