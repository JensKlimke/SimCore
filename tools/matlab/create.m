%clear all;clc;fname='../../log/veh_driver_velocity.json';create;
val = jsondecode(fileread(fname));

time = [val.time];
data = [val.data];

d_vDes = nan(size(time));
d_aux  = nan(size(time));

fn = fields(data);
for i = 1:numel(fn)
    disp(fn{i})
    evalin('base', ['d_', fn{i}, '=[data.(fn{i})];']);
end




hFig = figure(1);
clf;


a0 = subplot(5,2, [1,3,5,7,9]);
hold on;
grid on;
axis equal;
plot(d_x, d_y);

title('trajectory');
xlabel('x [m]');
ylabel('y [m]');


a1 = subplot(5,2,2);
hold on;
grid on;

plot(time, d_v);
plot(time, d_vDes, 'k-', 'LineWidth', 2);

title('velocity');
xlabel('t [s]');
ylabel('v [m/s]');
legend({'v', 'v_{Des}'});



a2 = subplot(5,2,4);
hold on;
grid on;

plot(time, d_a);
plot(time, d_aDes, 'k-', 'LineWidth', 2);
% plot(time, d_aDes - 0.01, 'k:', 'LineWidth', 0.5);
% plot(time, d_aDes + 0.01, 'k:', 'LineWidth', 0.5);

title('acceleration');
xlabel('t [s]');
ylabel('a [m/s^2]');
legend({'a', 'a_{Des}'});




a3 = subplot(5,2,6);
hold on;
grid on;

plot(time, d_dPsi);
plot(time, d_dPsiDes, 'k-', 'LineWidth', 2);
% plot(time, d_dPsi - 0.01, 'k:', 'LineWidth', 0.5);
% plot(time, d_dPsi + 0.01, 'k:', 'LineWidth', 0.5);

title('yaw rate');
xlabel('t [s]');
ylabel('\psi [1/s]');
h1 = legend({'\psi', '\psi_{Des}'});


a4 = subplot(5,2,8);
hold on;
grid on;

plot(time, d_pedal);
plot(time, d_steer);

title('primary controls');
xlabel('t [s]');
ylabel('i [-]');
legend({'pedal', 'streering'});


a5 = subplot(5,2,10);
hold on;
grid on;

plot(time, d_aux);

title('primary controls');
xlabel('t [s]');
ylabel('aux [-]');

linkaxes([a1, a2, a3, a4, a5], 'x');


ButtonH = uicontrol('Style', 'pushbutton', 'String', 'Reload');
ButtonH.Callback = @(btn,event) evalin('base', ['clear all;clc;fname=''', fname, ''';create']);


