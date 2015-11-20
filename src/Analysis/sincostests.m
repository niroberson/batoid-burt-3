clear
clc

xlam = 0:0.01:1;

cf = cos(2*pi*xlam);
sf = sin(2*pi*xlam);

Pcf = polyfit(xlam,cf,6);   %Need a 6th order poly for decent approx of full wave
Psf = polyfit(xlam,sf,5);   %Need a 5th order poly for decent approx of full wave

alf =@(xl) Pcf(7)+xl.*(Pcf(6)+xl.*(Pcf(5)+xl.*(Pcf(4)+xl.*(Pcf(3)+xl.*(Pcf(2)+xl.*Pcf(1))))));
bef =@(xl) Psf(6)+xl.*(Psf(5)+xl.*(Psf(4)+xl.*(Psf(3)+xl.*(Psf(2)+xl.*Psf(1)))));

plot(xlam,cf,'.')
hold on
plot(xlam,sf,'r.')
fplot(alf,[0 1],'g')
fplot(bef,[0 1],'k')
hold off
xlabel('x / \lambda')
ylim([-1.2 1.2])

fr = 0:0.05:5;
dt = 10 /1000;
fs = 1/dt;

ar = cos(2*pi*fr/fs);
br = sin(2*pi*fr/fs);

Pa = polyfit(fr,ar,4);
Pb = polyfit(fr,br,4);

alr =@(f) Pa(5)+f*(Pa(4)+f*(Pa(3)+f*(Pa(2)+f*Pa(1))));
ber =@(f) Pb(5)+f*(Pb(4)+f*(Pb(3)+f*(Pb(2)+f*Pb(1))));

figure(2)
plot(fr,ar,'.')
hold on
plot(fr,br,'r.')
fplot(alr,[0 5],'g')
fplot(ber,[0 5],'k')
hold off
xlabel('Frequency f (Hz)')
ylim([-0.1 1.1])

%%
L = 1;
N = 20;
x = (1:1:N)'*L/N;

f = 1;
A = 9;
lam = 3/4*L;

Ai = A*x;
alphaf = alf(mod(x/lam,1));
betaf = bef(mod(x/lam,1));
alphar = alr(f);
betar = ber(f);

for i = 1:length(Pa)
    fprintf('%15.9f  ',Pa(i))
    fprintf('%15.9f\n',Pb(i))
end
fprintf('\n')

for i = 1:length(Pcf)
    fprintf('%15.9f  ',Pcf(i))
    
    if i<=length(Psf), fprintf('%15.9f\n',Psf(i)); end
end
fprintf('\n')

% tmax = 10;
% maxit = tmax*fs;
% Ck = 1;
% Sk = 0;
% tcur = now*24*3600; %convert days (now) to seconds
% for k = 1:maxit
%     to = tcur;
%     Co = Ck;
%     So = Sk;
%     Ck = alphar*Co - betar*So;
%     Sk = betar*Co + alphar*So;
%     
%     yi = Ai.*(betaf*Ck - alphaf*Sk);
%     plot(x,yi)
%     xlim([0 L])
%     ylim([-A*L A*L])
%     
%     tcur = now*24*3600; %convert days (now) to seconds
%     delt = tcur - to;
%     pause(dt-delt);
% end