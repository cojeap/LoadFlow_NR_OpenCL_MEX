classdef NetworkLineData
    properties
        fromNode
        toNode
        nominalvoltage
        length
        resistance0
        reactance0
        conductance0
        susceptance0
        nrOfConductors        
    end
    methods
        function obj = NetworkLineData(fileName,sheet,xlRange)
            if isempty(fileName)
                msgID = 'NetworkLineData:FileErr';
                msgText = 'File name is invalid!';
                MExcept = MException(msgID,msgText);
                throw(MExcept);
            end
            if exist(fileName,'file')==0
                msgID = 'NetworkLineData:FileErr';
                msgText = 'File does not exist or it has a wrong name!';
                MExcept = MException(msgID,msgText);
                throw(MExcept);
            end
            if isnan(sheet) || sheet == 0
                msgID = 'NetworkLineData:Sheet';
                msgText = 'Incorrect sheet number';
                MExcept = MException(msgID,msgText);
                throw(MExcept);
            end
            
            temp = xlsread(fileName,sheet,xlRange);
            [row,~] = size(temp);
            for i=1:row
                obj.fromNode(i) = temp(i,1);
                obj.toNode(i) = temp(i,2);
                obj.nominalvoltage(i) = temp(i,3);
                obj.length(i) = temp(i,8);
                obj.resistance0(i) = temp(i,4);
                obj.reactance0(i) = temp(i,5);
                obj.conductance0(i) = temp(i,6);
                obj.susceptance0(i) = temp(i,7);
                obj.nrOfConductors(i) = temp(i,9);
            end
            obj = obj.TransposeData();
        end
    end
    methods (Hidden = true)
        function obj = TransposeData(obj)
            obj.fromNode = obj.fromNode';
            obj.toNode = obj.toNode';
            obj.nominalvoltage = obj.nominalvoltage';
            obj.length = obj.length';
            obj.resistance0 = obj.resistance0';
            obj.reactance0 = obj.reactance0';
            obj.conductance0 = obj.conductance0';
            obj.susceptance0 = obj.susceptance0';
            obj.nrOfConductors = obj.nrOfConductors';
        end
    end
end