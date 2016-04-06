%Error analysis for the bar

%2/28/16
clear
clc
close all

load('allpoints.mat')
%Set up an object to save to movie:
VidObj = VideoWriter('animation');
VidObj.FrameRate=5;
open(VidObj);

%setting up the bar %
XL1mean=[mean(XL1(1,:));mean(XL1(2,:));mean(XL1(3,:))];
XL2mean=[mean(XL2(1,:));mean(XL2(2,:));mean(XL2(3,:))];

barpt8=XL2mean;
barpt1=XL1mean;
incr=(barpt8-barpt1)/7
barpt2=barpt1+incr*1;
barpt3=barpt1+incr*2;
barpt4=barpt1+incr*3;
barpt5=barpt1+incr*4;
barpt6=barpt1+incr*5;
barpt7=barpt1+incr*6;
barpt=[barpt1,barpt2,barpt3,barpt4,barpt5,barpt6,barpt7,barpt8];


%all points data 
MegaXL=[XL3;XL4;XL5;XL6;XL7;XL8;XL9;XL10];

%8 different bars


%angle & elevation
az1 = 5;
el1 = 5;
az2 = 10;
el2 = -55;
az=[az1,az1,az1,az1,az1,az1,az1,az1,az1,az1,az1,az1,6,7,8,9,10,10,10,az2,az2,az2,az2,az2,az2,az2,az2,az2,az2,az2,az2];
el=[el1,el1,el1,el1,el1,el1,el1,el1,el1,el1,el1,el1,5,0,-5,-10,-15,-30,-45,el2,el2,el2,el2,el2,el2,el2,el2,el2,el2,el2,el2];


% az=[az1:(az2-az1)/31:az2];
% el=[el1:(el2-el1)/31:el2];
for t=1:31
    hold on

    for i=1:8
        view(az(t), el(t));
        scatter3(MegaXL(3*(i-1)+1,t),MegaXL(3*(i-1)+2,t),MegaXL(3*(i-1)+3,t))
        plot3([barpt1(1,1),XL3(1,t)],[barpt1(2,1),XL3(2,t)],[barpt1(3,1),XL3(3,t)])
        plot3([barpt2(1,1),XL4(1,t)],[barpt2(2,1),XL4(2,t)],[barpt2(3,1),XL4(3,t)])
        plot3([barpt3(1,1),XL5(1,t)],[barpt3(2,1),XL5(2,t)],[barpt3(3,1),XL5(3,t)])
        plot3([barpt4(1,1),XL6(1,t)],[barpt4(2,1),XL6(2,t)],[barpt4(3,1),XL6(3,t)])
        plot3([barpt5(1,1),XL7(1,t)],[barpt5(2,1),XL7(2,t)],[barpt5(3,1),XL7(3,t)])
        plot3([barpt6(1,1),XL8(1,t)],[barpt6(2,1),XL8(2,t)],[barpt6(3,1),XL8(3,t)])
        plot3([barpt7(1,1),XL9(1,t)],[barpt7(2,1),XL9(2,t)],[barpt7(3,1),XL9(3,t)])
        plot3([barpt8(1,1),XL10(1,t)],[barpt8(2,1),XL10(2,t)],[barpt8(3,1),XL10(3,t)])
    end
    xlim([-300 500])
    ylim([-200 500])
    zlim([400 800])
    axis 
    plot3(barpt(1,:),barpt(2,:),barpt(3,:),'LineWidth',5)
    pause(0.2)
%     drawnow
   
    
    %Save this frame into the defined video object:
    ax = gca;
    ax.Units = 'pixels';
    pos = ax.Position;
    marg = 31;
    rect = [-marg, -marg, pos(3)+2*marg, pos(4)+2*marg];
    F = getframe(gca,rect);
    ax.Units = 'normalized';

    writeVideo(VidObj, F);
    clf
    
end
    

%close the file (to save it)

close(VidObj);