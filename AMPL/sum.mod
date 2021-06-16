set I;
param Value{I} integer;
param TargetSum integer;
var Take{I} binary integer;

maximize TotalValue: sum{i in I} Take[i] * Value[i];
subject to Limit: sum{i in I} Take[i] * Value[i] <= TargetSum;
