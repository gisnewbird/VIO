clear 
close all

dt = dlmread('/home/dukeguo/imu_utils/src/imu_utils/data/data_imutest_gyr_t.txt');
data_x = dlmread('/home/dukeguo/imu_utils/src/imu_utils/data/data_imutest_gyr_x.txt'); 
data_y= dlmread('/home/dukeguo/imu_utils/src/imu_utils/data/data_imutest_gyr_y.txt'); 
data_z = dlmread('/home/dukeguo/imu_utils/src/imu_utils/data/data_imutest_gyr_z.txt'); 
data_draw=[data_x data_y data_z] ;

data_sim_x= dlmread('/home/dukeguo/imu_utils/src/imu_utils/data/data_imutest_sim_gyr_x.txt'); 
data_sim_y= dlmread('/home/dukeguo/imu_utils/src/imu_utils/data/data_imutest_sim_gyr_y.txt'); 
data_sim_z= dlmread('/home/dukeguo/imu_utils/src/imu_utils/data/data_imutest_sim_gyr_z.txt'); 
data_sim_draw=[data_sim_x data_sim_y data_sim_z] ;


figure
loglog(dt, data_draw , 'o');
% loglog(dt, data_sim_draw , '-');
xlabel('time:sec');                
ylabel('Sigma:deg/h');             
% legend('x','y','z');      
grid on;                           
hold on;                           
loglog(dt, data_sim_draw , '-');
