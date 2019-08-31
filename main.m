%{
%
%   Writen by Petru Cojea on 22.04.2019
%
%}

addpath('input','src');

%

CASE_TEST2_FIN2

[a_TEST2,b_TEST2,c_TEST2] = RunPF_OCL(0,[],networkData,lineData,trafoData);

%}

%{

CASE_IEEE_14

[a_14,b_14,c_14] = RunPF_OCL(0,[],networkData,lineData,trafoData);


%}


%{

CASE_IEEE_30

[a_30,b_30,c_30] = RunPF_OCL(0,[],networkData,lineData,trafoData);

%}

%{

CASE_IEEE_57

[a_57,b_57,c_57] = RunPF_OCL(0,[],networkData,lineData,trafoData);

%}

%{

CASE_IEEE_118

[a_118,b_118,c_118] = RunPF_OCL(0,[],networkData,lineData,trafoData);

%}


%{
[~,f] = inmem( '-completenames' );
result = strfind( f, ['mex_Newton_Raphson_OpenCL' '.' mexext] );
result = f(cellfun( @isempty, result, 'UniformOutput', true ) == 0);
clear( result{:} )
%}


