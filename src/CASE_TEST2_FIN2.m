% Newton_Raphson_OpenCL_MEX. Loads data from xls files to be passed to the mex function. 
% 
% Writen by Petru Cojea on 22.04.2019
%

folder = 'input';
fileName = 'date_TEST2_FIN2.xlsx';

inputFile = [folder , '\' , fileName];

fileSheet = 2;
fileXlRange = 'A1:J13';
networkData = NetworkData(inputFile,fileSheet,fileXlRange);

fileSheet = 3;
fileXlRange = 'B3:J17';
lineData = NetworkLineData(inputFile,fileSheet,fileXlRange);

%trafoData = ElectricalTransformer();
trafoData=[];