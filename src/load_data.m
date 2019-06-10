%{
%
%   Writen by Petru Cojea on 22.04.2019
%
%}


fileName = 'date_TEST2_FIN2.xlsx';

fileSheet = 2;
fileXlRange = 'A1:J13';
networkData = NetworkData(fileName,fileSheet,fileXlRange);

fileSheet = 3;
fileXlRange = 'B3:J17';
lineData = NetworkLineData(fileName,fileSheet,fileXlRange);