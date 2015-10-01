clear; clc %10/2/14

%Fluid Properties (water)
dens = 997; %kg/m3

%Stingray Movement Parameters
w = 28/6*0.0254; %width of section controlled by servo
om = 15; %rad/s - frequency of fin waves
L = 8.5*0.0254; %m - length of fin section from body
Am = L*.15; %m - max amplitude at fin tip (vertical)

thm = atan(Am/L); %rad - max fin angle from horizontal

%Pressure Relation
syms x
v = x*om*thm;       %Velocity of points along the fin
P = 1/2*dens*v^2;   %Pressure resulting from flapping fin

%Determining necessary torque/length
fpl = int(P,x,0,x);
tpl = int(fpl,x,0,L);
tor = tpl*w;

fprintf('Length: %f m\n',L)
fprintf('Width:  %f m\n',w)
fprintf('Amp of Flap: %f m\n',Am)
fprintf('Freq of Flap: %f rad/s\n',om)
fprintf('\n')
fprintf('Torque/Width:    %f N-m/m\n',double(tpl))
fprintf('Torque Required: %f N-m\n',double(tor))
fprintf('\n')

%Toss on a safety factor and convert to specs units
n = 2;
tormet = double(tor)*100/9.8;   %kg-cm (not even a torque; specs are dumb)
torimp = double(tor)*3.59694310194/0.0254; %oz-in
fprintf('Min Torque Specifications: Safety factor of %.2f\n',n)
fprintf('\t%f kg-cm\n',tormet)
fprintf('\t%f oz-in\n\n',torimp)

%Look at speed specs
winv = 1/(om*180/pi);   %s/deg
wspec = 60*winv;        %s/60deg
fprintf('Speed Specifications:\n')
fprintf('\t%f s/60deg\n',wspec)

%Check out stall torque - max moment of inertia/mass of a rod   11/16
alpha = thm*om^2;    %rad/s^2
J = double(tor)/alpha;      %kg-m^2
Lrod = 8*0.0254;    %m
mrod = 12*J/Lrod^2  %kg