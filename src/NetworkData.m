%%load power network data from an xls file,in an object

classdef NetworkData <handle
    properties
        nodeNr
        nodeType
        nominalVoltage
        activePower
        reactivePower
        consumedActivePower
        consumedReactivePower
        imposedVoltage
        minimumReactivePower
        maximumReactivePower
    end
    methods
        function obj = NetworkData(fileName,fsheet,xlRange)
            %{
            if isempty(fileName)
                msgID = 'NetworkData:FileErr';
                msgText = 'File name is invalid!';
                MExcept = MException(msgID,msgText);
                throw(MExcept);
            end
            if exist(fileName,'file')==0
                msgID = 'NetworkData:FileErr';
                msgText = 'File does not exist or it has a wrong name!';
                MExcept = MException(msgID,msgText);
                throw(MExcept);
            end
            if isnan(sheet) || sheet == 0
                msgID = 'NetworkData:Sheet';
                msgText = 'Incorrect sheet number';
                MExcept = MException(msgID,msgText);
                throw(MExcept);
            end
            %}
            temp = xlsread(fileName,fsheet,xlRange,'basic');
            [row,~] = size(temp);
            obj.nodeType = [];
            for i = 1:row
                if(temp(i,2)==2)%1
                    obj.nodeType = [obj.nodeType;2] ;
                elseif(temp(i,2)==1)%2
                    obj.nodeType = [obj.nodeType;1] ;
                elseif(temp(i,2)==0)%3
                    obj.nodeType = [obj.nodeType;0] ;
                end
            end
            for i = 1:row
                obj.nodeNr(i) = temp(i,1);
                obj.nominalVoltage(i) = temp(i,3) ;
                obj.activePower(i) = temp(i,4) ;
                obj.reactivePower(i) = temp(i,5) ;
                obj.consumedActivePower(i) = temp(i,6) ;
                obj.consumedReactivePower(i) = temp(i,7) ;
                obj.imposedVoltage(i) = temp(i,8) ;
                obj.minimumReactivePower(i) = temp(i,9) ;
                obj.maximumReactivePower(i) = temp(i,10) ;
            end
            
            obj = TransposeData(obj);
        end
    end
    methods (Hidden = true)
        function obj = TransposeData(obj) % due to xlread -> change from [~,col] to [row,~]
            obj.nodeNr = obj.nodeNr';
            obj.nominalVoltage = obj.nominalVoltage';
            obj.activePower = obj.activePower';
            obj.reactivePower = obj.reactivePower';
            obj.consumedActivePower = obj.consumedActivePower';
            obj.consumedReactivePower = obj.consumedReactivePower';
            obj.imposedVoltage = obj.imposedVoltage';
            obj.minimumReactivePower = obj.minimumReactivePower';
            obj.maximumReactivePower = obj.maximumReactivePower';
        end
    end
end


%%end load data function