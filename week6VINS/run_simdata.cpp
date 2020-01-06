#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <iomanip>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>
#include "System.h"

using namespace std;
using namespace cv;
using namespace Eigen;

const int nDelayTimes = 2;
string sData_path = "~/vio/vio_data_simulation/bin/";
string sConfig_path = "../config/";

std::shared_ptr<System> pSystem;

void PubImuData()
{
	string sImu_data_file = sData_path + "imu_pose_noise.txt";
	cout << "1 PubImuData start sImu_data_filea: " << sImu_data_file << endl;
	ifstream fsImu;
	fsImu.open(sImu_data_file.c_str());
	if (!fsImu.is_open()){
		cerr << "Failed to open imu file! " << sImu_data_file << endl;
		return;
	}

	std::string sImu_line;
	double dStampNSec = 0.0;
	Vector3d vAcc;
	Vector3d vGyr;
	Eigen::Quaterniond Qwb;
	Vector3d position;
	
	while (std::getline(fsImu, sImu_line) && !sImu_line.empty()) // read imu data
	{
		std::istringstream ssImuData(sImu_line);
		ssImuData >> dStampNSec >>Qwb.w()>>Qwb.x()>>Qwb.y()>>Qwb.z()>>position.x()>>position.y()>>position.z() >>vGyr.x() >> vGyr.y() >> vGyr.z() >> vAcc.x() >> vAcc.y() >> vAcc.z();
		// cout << "Imu t: " << fixed << dStampNSec << " gyr: " << vGyr.transpose() << " acc: " << vAcc.transpose() << endl;
		pSystem->PubImuData(dStampNSec, vGyr, vAcc);
		usleep(5000 * nDelayTimes);
	}
	fsImu.close();
}

void PubImageData()
{
	string sImage_file = sData_path + "cam_pose_tum.txt";
	cout << "1 PubImageData start sImage_file: " << sImage_file << endl;
	ifstream fsImage;
	fsImage.open(sImage_file.c_str());
	if (!fsImage.is_open()){
		cerr << "Failed to open image file! " << sImage_file << endl;
		return;
	}

	std::string sImage_line;
	double dStampNSec;
	int n = 0;
	// cv::namedWindow("SOURCE IMAGE", CV_WINDOW_AUTOSIZE);
	while (std::getline(fsImage, sImage_line) && !sImage_line.empty())
	{
		vector<cv::Point2f> feacturePoint;
		std::istringstream ssImageData(sImage_line);
		ssImageData >> dStampNSec ;
		// cout << "Image t : " << fixed << dStampNSec <<endl;

		std::stringstream filename1;
        filename1<<"keyframe/all_points_"<<n<<".txt";
		n = n+1;
		string featurePath = sData_path + filename1.str();
		// cout<<"featurePath: "<<featurePath<<endl;

		ifstream fsFeature;
		fsFeature.open(featurePath.c_str());
		if (!fsFeature.is_open()){
			cerr << "Failed to open image file! " << featurePath << endl;
			return;
		}
		std::string sFeature_line;
		while(std::getline(fsFeature,sFeature_line) && !sFeature_line.empty()){
			cv::Point2f featureTemp;
			Vector4d temp;
			std::istringstream ssFeatureData(sFeature_line);
			double u = 0.0;
			double v = 0.0;
			ssFeatureData>>temp.x()>>temp.y()>>temp.z()>>temp.w()>>u>>v;
			featureTemp.x = u;featureTemp.y = v;
			feacturePoint.push_back(featureTemp);
		}
		// cout<<"feacturePoint.size(): "<<feacturePoint.size()<<endl;
		pSystem->PubImageData(dStampNSec, feacturePoint);
		usleep(50000*nDelayTimes);
	}
	fsImage.close();
}

int main(int argc, char **argv)
{
	/* if(argc != 3){
		cerr << "./run_euroc PATH_TO_FOLDER/MH-05/mav0 PATH_TO_CONFIG/config \n" 
			<< "For example: ./run_euroc /home/stevencui/dataset/EuRoC/MH-05/mav0/ ../config/"<< endl;
		return -1;
	}
	sData_path = argv[1];
	sConfig_path = argv[2]; */

	pSystem.reset(new System(sConfig_path));

	std::thread thd_BackEnd(&System::ProcessBackEnd, pSystem);
	std::thread thd_PubImuData(PubImuData);
	std::thread thd_PubImageData(PubImageData);
	std::thread thd_Draw(&System::Draw, pSystem);
	
	thd_PubImuData.join();
	thd_PubImageData.join();
	thd_BackEnd.join();
	thd_Draw.join();

	cout << "main end... see you ..." << endl;
	system("pause");
	return 0;
}