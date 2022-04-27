var 
  r1,r2: integer; // values on cubes
begin
  r1 := Random(6)+1;
  r2 := Random(6)+1;
  writeln('Values: ',r1,' ',r2);
  writeln('Summ, ',r1+r2);
end.