%  Example script to demonstrate how to implement a quadrature waveform
%  generator for the robotic sting-ray senior design project.

clear, clc


%  --define the frequencies

frange = linspace(0.1,3,100);       % desired frequency range (Hz)

fs = 50;        % sample rate (Hz)
ts = 1/fs;      % sample period (sec) only used for plotting


%  --curve fit the "constant" values in the generator

alpha = cos(2*pi*frange/fs);
beta = sin(2*pi*frange/fs);

PA = polyfit(frange,alpha,4);
PB = polyfit(frange,beta,4);

alpha_fit = polyval(PA,frange);
beta_fit = polyval(PB,frange);

figure
plot(frange,alpha_fit,'g')
hold on
plot(frange,alpha,'r.','markersize',10)
plot(frange,beta_fit,'b')
plot(frange,beta,'c.','markersize',10)
grid on
xlabel('Desired Frequency (Hz)')
ylabel('Waveform Generator Coefficients')
legend('Fitted \alpha','Actual \alpha','Fitted \beta','Actual \beta', ...
    'location','west')
%%

%  --configure for waveform generation

fd = 1.1;     % desired frequency (Hz)

a = PA(5) + fd*(PA(4) + fd*(PA(3) + fd*(PA(2) + fd*PA(1))));
b = PB(5) + fd*(PB(4) + fd*(PB(3) + fd*(PB(2) + fd*PB(1))));


N = 100;        % number of time steps to run the discrete simulation

Ck_old = 1;       % initial condition on cosine
Sk_old = 0;       % initial condition on sine

C = zeros(1,N);
S = zeros(1,N);


%  --generate a complex profile

testcase = 3;

switch testcase
    case 1  % randomize
        NW = 6;      % number of independent waveforms (motor commands)
        M = (pi/8)*rand(1,NW);      % magnitudes (radians)
        phi = pi*randn(1,NW);       % phase angles
        
    case 2  % fixed values
        NW = 4;
        M = (pi/10)*[1 1 1 1];
        phi = (pi/4)*[-1,1,-1,1];
        
    case 3  % fixed values
        NW = 8;
        M = (pi/8)*[1 1 1 1 1 1 1 1];
        phi = (pi/10)*[0,1,2,3,4,5,6,7];
        
    case 4  % fixed values
        NW = 8;
        M = (pi/8)*[1 1 1 1 1 1 1 1];
        phi = (pi/3)*[0,1,2,3,4,5,6,7];
end

for i = 1:NW,
    A(i) = M(i) * cos(phi(i));  % in-phase magnitude
    B(i) = M(i) * sin(phi(i));  % quadrature magnitude
end

x = zeros(NW,N);        % storage for the waveforms


%  --run the discrete-time simulation

for k = 1:N,
    
    %  --save the data to an array for plotting later
    
    C(k) = Ck_old;
    S(k) = Sk_old;
    
    
    %  --compute the next waveform sample
    
    Ck_new = a * Ck_old - b * Sk_old;
    Sk_new = b * Ck_old + a * Sk_old;
    
    Ck_old = Ck_new;
    Sk_old = Sk_new;
    
    
    %  --compute the complex waveforms
    
    for i = 1:NW,
        theta(i,k) = A(i) * Ck_new + B(i) * Sk_new;     % radians
    end
    
end


%  --plot the time responses

t = ts * [0:N-1];

figure
subplot(2,1,1)
plot(t,C,'r.-',t,S,'g.-')
grid on
xlabel('Time (sec)')
ylabel('Quadrature Waveforms')
subplot(2,1,2)
plot(t,theta)
grid on
xlabel('Time (sec)')
ylabel('Individual Waveforms (radians)')


%  --3D plot of the first data point in the simulation

L = NW*ones(1,NW);       % length of each beam

X = [0:NW-1];           % longitudinal positions of each beam

Y = zeros(1,NW);        % y-coordinate of central axis of rotation

figure
plot3(X,0*X,0*X,'color',0.5*[1 1 1],'linewidth',10)     % central axis of rotation
c = jet(NW);
hold on
for i = 1:NW,
    h(i) = plot3(X(i)*[1 1],[0,L(i)*cos(theta(i,1))],[0,L(i)*sin(theta(i,1))]);
    set(h(i),'linewidth',10,'color',c(i,:))
end
xlabel('x')
ylabel('y')
zlabel('z')
view([78,28])
view([154,16])
axis equal
xlim([min(X),max(X)])
ylim(max(L)*[0,1])
zlim(max(L)*[-1,1]/3)


%%  --animate the remaining time response

for k = 1:N,
    for i = 1:NW,
        y = L(i)*cos(theta(i,k));
        z = L(i)*sin(theta(i,k));
        set(h(i),'ydata',[0,y],'zdata',[0,z])
    end
    drawnow
end

        
    



        




    
    