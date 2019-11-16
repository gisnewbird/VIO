%% Initalization
close all
clear all

% Read in our toolboxes
addpath('/home/dukeguo/imu_utils/src/kalibr_allan/matlab/functions')
addpath('functions/allan_v3')

% Our bag information
%titlestr = 'XSENS MTi-G-710';
%mat_path = '../data/bags/results_20170908T182715.mat';

%titlestr = 'Tango Yellowstone #1';
%mat_path = '../data/bags/results_20171031T115123.mat';

titlestr = 'ADIS16448 VI-Sensor';
mat_path = '/home/dukeguo/imu_utils/src/kalibr_allan/data/results_20191025T141518.mat';

% Load the mat file (should load "data_imu" matrix)
fprintf('=> opening the mat file.\n')
load(mat_path);


%% Get the calculated sigmas

fprintf('=> plotting accelerometer.\n')
[fh1,sigma_a,sigma_ba] = gen_chart(results_ax.tau1,results_ax.sig2,...
                                    results_ay.sig2,results_az.sig2,...
                                    titlestr,'acceleration','m/s^2',...
                                    'm/s^2sqrt(Hz)','m/s^3sqrt(Hz)');


fprintf('=> plotting gyroscope.\n')
[fh2,sigma_g,sigma_ga] = gen_chart(results_wx.tau1,results_wx.sig2,...
                                    results_wy.sig2,results_wz.sig2,...
                                    titlestr,'gyroscope','rad/s',...
                                    'rad/s^1sqrt(Hz)','rad/s^2sqrt(Hz)');

                                

%% Print out for easy copying
fprintf('=> final results\n');
% Accelerometer加速计
fprintf('accelerometer_noise_density = %.8f\n',sigma_a); //加速计bias
fprintf('accelerometer_random_walk   = %.8f\n',sigma_ba);//加速计bias随机游走
% Gryoscope陀螺仪
fprintf('gyroscope_noise_density     = %.8f\n',sigma_g); //陀螺仪bias
fprintf('gyroscope_random_walk       = %.8f\n',sigma_ga);//陀螺仪bias随机游走



%% Save to file
[pathstr, name, ext] = fileparts(mat_path);
print(fh1,'-dpng','-r500',[pathstr,'/',name,'_accel.png'])
print(fh2,'-dpng','-r500',[pathstr,'/',name,'_gyro.png'])





