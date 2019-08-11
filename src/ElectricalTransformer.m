
%presupunere transformatoare nr x2, se va lua size de la (nr total /m)+1
% ex: pentru datele de la transf 3 si 4; m = 2

classdef ElectricalTransformer
    properties
             from;
             to;
             voltage_from;
             voltage_to;
             apparentPower;
             deltaP0;
             current0;
             deltaPsc;
             voltageSC;
             nrOfTrafo;
             plot;
             deltaUp;
    end
    methods
        %
        function obj = Procesare(obj)
            
            obj.rezistentaLong = obj.apparentPower;
            for i = 1:size(obj.rezistentaLong)
                obj.rezistentaLong(i) = obj.pierderiScurtcircuit(i)*(obj.tensiuneNominalaF(i).^2)/1000/obj.apparentPower(i).^2;
            end
            
            obj.admitantaLong = obj.apparentPower;
            for i = 1:size(obj.admitantaLong)
                obj.admitantaLong(i) = obj.tensiuneScurtcircuit(i)/100*(obj.tensiuneNominalaF(i).^2/obj.apparentPower(i));
            end
            
            obj.reactantaLong = obj.apparentPower;
            for i = 1:size(obj.reactantaLong)
                obj.reactantaLong(i) = sqrt(obj.admitantaLong(i).^2-obj.rezistentaLong(i).^2);
            end
            
            obj.impedantaLongComplex = obj.apparentPower;
            for i = 1:size(obj.impedantaLongComplex)
                obj.impedantaLongComplex(i) = obj.rezistentaLong(i) + 1j*obj.reactantaLong(i);
            end
            
            
            
            
            obj.conductantaTrvr = obj.apparentPower;
            for i = 1:size(obj.conductantaTrvr)
                obj.conductantaTrvr(i) = obj.pierderiGol(i)/1000/obj.tensiuneNominalaF(i).^2;
            end
            
            obj.admitantaTrvr = obj.apparentPower;
            for i = 1:size(obj.admitantaTrvr)
                obj.admitantaTrvr(i) = obj.curentGol(i)/100*(obj.apparentPower(i)/obj.tensiuneNominalaF(i).^2) ;
            end
            
            obj.susceptantaTrvr = obj.apparentPower;
            for i = 1:size(obj.susceptantaTrvr)
                obj.susceptantaTrvr(i) = sqrt(obj.admitantaTrvr(i).^2 - obj.conductantaTrvr(i).^2) ;
            end
            
            obj.admitantaTrvrComplex = obj.apparentPower;
            for i = 1:size(obj.admitantaTrvrComplex)
                obj.admitantaTrvrComplex(i) = obj.conductantaTrvr(i) - 1j*obj.susceptantaTrvr(i);
            end
            
        end
        %}
    end
end





%%end Tabel 3 Parametrii Transformatoarelor