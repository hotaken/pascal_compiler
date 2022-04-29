program dead;

type
  yaUstal = integer;

var 
  r6:yaUstal;
  r5:string;
  r1,r2: integer; { values on cubes }
  r1:real;
  r12:int;
begin
  r5:=r6;
  r1 := 2+1;
  r2 := 3+1;
  r1:= r2+r1*(r1+r2/(r1+r5));
  r1:= r1+r2+r2+r1+1234;
  while r1<33 do
		if r1<12 then r1:=r1+1 else r1:=r1+2;
  r12:= 12+(r1*3/(2+1))
end.