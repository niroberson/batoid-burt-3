function create_fin_gif(f, A, xd, xc, dt, Nz, filename)
% Create initial plot
figure(1), hold on
h1 = plot(xd, A(xd).*f(xd, 0));
h2 = plot(xc, A(xc).*f(xc, 0));
xlim([xd(1) xd(end)])
ylim([-max(A(xd)) max(A(xd))])
xlabel('x [in]')
ylabel('y [degrees]')

% Plot amplitude envelopes
plot(xd, A(xd), 'r--')
plot(xd, -A(xd), 'r--')

% Create movie structures
init_getframe = struct('cdata',[],'colormap',[]);
frames = repmat(init_getframe, Nz, 1 );
frames(1) = getframe(figure(1));
im = frame2im(frames(1));
[imind,cm] = rgb2ind(im,256);
imwrite(imind,cm,filename,'gif', 'Loopcount',inf);

% Create movie
idx = 1;
for t = dt:dt:Nz
    set(h1,'YData', A(xd).*f(xd, t));
    set(h2,'YData', A(xc).*f(xc, t));
    drawnow
    frames(idx) = getframe(figure(1));
    im = frame2im(frames(idx));
    [imind,cm] = rgb2ind(im,256);
    imwrite(imind,cm,filename,'gif','WriteMode','append','delaytime',dt);
    pause(dt)
    idx = idx + 1;
end

% % Play movie
% fps = 1/dt;
% movie(frames, 1, fps)