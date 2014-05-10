xStart=zeros(3,1);
fun=@dplanFun;
x=fsolve(fun, xStart);
save data.out xStart -ascii;
quit;