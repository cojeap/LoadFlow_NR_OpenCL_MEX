%{
%
%   Writen by Petru Cojea on 22.04.2019
%
%}

addpath('input','src');

CASE_IEEE_30

%CASE_TEST2_FIN2

[a,b,c] = RunPF_OCL(0,[],networkData,lineData,trafoData);


%{
[~,f] = inmem( '-completenames' );
result = strfind( f, ['mex_Newton_Raphson_OpenCL' '.' mexext] );
result = f(cellfun( @isempty, result, 'UniformOutput', true ) == 0);
clear( result{:} )
%}


