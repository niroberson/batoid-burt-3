clear
clc

dt = 10; %ms
fs = 1/dt;
maxit = 1000;

Pa = [0.000000641, 0.000000052,-0.001974030, 0.000000082, 0.999999986];
Pb = [0.000000101,-0.000041791, 0.000000840, 0.062831256, 0.000000098];

Pcf = [60.709408575, -182.128225725, 166.173260485, -28.799478096, -15.731910650, -0.223054590, 1.002616693]; %FOR NORM'D X/LAM
Psf = [-54.848410591, 137.121026478, -98.700216010, 10.929297536, 5.472509859, 0.012896364];  

N = 6;
f = 0.5;
x = (1:1:N)*3;
L = x(end);
lam = 2/3*L;


alphar = Pa(5)+f*(Pa(4)+f*(Pa(3)+f*(Pa(2)+f*Pa(1))));   %cos coef for frequency
betar  = Pb(5)+f*(Pb(4)+f*(Pb(3)+f*(Pb(2)+f*Pa(1))));   %sin coef for frequency

xl = x/lam;
for i = 1:length(xl), xl(i) = mod(xl(i),1); end
alphaf = Pcf(7)+xl.*(Pcf(6)+xl.*(Pcf(5)+xl.*(Pcf(4)+xl.*(Pcf(3)+xl.*(Pcf(2)+xl.*(Pcf(1))))))); %cos coef for wavelength
betaf  = Psf(6)+xl.*(Psf(5)+xl.*(Psf(4)+xl.*(Psf(3)+xl.*(Psf(2)+xl.*(Psf(1))))));             %sin coef for wavelength

Ck = 1; Co = 1;
Sk = 0; So = 0;

for i = 1:maxit
    Co = Ck;
    So = Sk;
    Ck = alphar*Co - betar*So;
    Sk = betar*Co + alphar*So;
    
    yi = betaf*Ck - alphaf*Sk;
    plot(x,yi)
    ylim([-2 2])
    pause(0.01)
end