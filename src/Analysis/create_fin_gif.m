function create_fin_gif(f, A, x, Nz, fps, filename)
% Create initial plot
figure, hold on
h = plot(x, A.*f(0));
% plot(x, f(x), 'r--')
% plot(x, f(x), 'r--')
xlim([x(1) x(end)])
ylim([-max(A) max(A)])
xlabel('x')
ylabel('y')

% Create movie structures
init_getframe = struct('cdata',[],'colormap',[]);
frames = repmat(init_getframe, Nz, 1 );
frames(1) = getframe;
im = frame2im(frames(1));
[imind,cm] = rgb2ind(im,256);
imwrite(imind,cm,filename,'gif', 'Loopcount',inf);

% Create movie
for t = 1:1:Nz
    set(h,'YData', A.*f(t));
    drawnow
    frames(t) = getframe;
    im = frame2im(frames(t));
    [imind,cm] = rgb2ind(im,256);
    imwrite(imind,cm,filename,'gif','WriteMode','append','delaytime',1/fps);
    pause(1/1000)
end

% Play movie
movie(frames, 10, fps)