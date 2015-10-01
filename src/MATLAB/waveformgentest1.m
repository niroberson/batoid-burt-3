clear
clc

dt = 10; %ms
fs = 1/dt;
maxit = 1000;

Pa = [0.000000641, 0.000000052,-0.001974030, 0.000000082, 0.999999986];
Pb = [0.000000101,-0.000041791, 0.000000840, 0.062831256, 0.000000098];

f = 0.5;

alphar = Pa(5)+f*(Pa(4)+f*(Pa(3)+f*(Pa(2)+f*Pa(1))));
betar  = Pb(5)+f*(Pb(4)+f*(Pb(3)+f*(Pb(2)+f*Pa(1))));

Ck = 1; Co = 1;
Sk = 0; So = 0;

for i = 1:maxit
    Co = Ck;
    So = Sk;
    Ck = alphar*Co - betar*So;
    Sk = betar*Co + alphar*So;
    
    posC(i) = Ck;
    posS(i) = Sk;
end

t = (1:maxit)*dt/1000;

plot(t,posC,t,posS,'r')