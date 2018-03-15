#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

double square_length = 40;   //mm
Size pattern_size(6, 4);
double apertureWidth = 4.8;
double apertureHeight = 3.6;
double fovx, fovy, focalLength;
Point2d principalPoint;
double aspectRatio;

int main()
{
	Mat image, image_gray;
	VideoCapture input("output%03d.jpg");
	//VideoCapture input(0);


	namedWindow("display", WINDOW_NORMAL);
	resizeWindow("display", 1000, 800);
	
	vector<Point2f> corners_image;
	vector<vector<Point2f>> points_image;


	int count = 0;
	while(1)
	{
		if (!input.read(image))
			break;
		
		input >> image;

		cvtColor(image, image_gray, CV_BGR2GRAY);
		
		bool found = findChessboardCorners(image_gray, pattern_size, corners_image);
		if (found)
		{
			drawChessboardCorners(image, pattern_size, corners_image, found);
			points_image.push_back(corners_image);
		}
		
		imshow("display", image);
		waitKey(300);
			
	}

	vector<Point3f> corners_world;

	for(int i=0;i<pattern_size.height; i++)
		for (int j=0; j<pattern_size.width; j++)
		{
			corners_world.push_back( Point3f(j*square_length, i*square_length, 0) );
		}
	
	vector<vector<Point3f>>points_world(points_image.size(), corners_world);
	Mat K, D;
	vector<Mat>R, T;

	calibrateCamera(points_world, points_image, image_gray.size(), K, D, R, T);
	//cout << "Reprojection error : " << rms << endl;
	
	cout << endl << "Intrinsic : " << endl << K << endl << endl;



	calibrationMatrixValues(K, image_gray.size(), apertureWidth, apertureHeight, fovx, fovy, focalLength, principalPoint, aspectRatio);

	double mx = image_gray.size().width / apertureWidth;
	double my = image_gray.size().height / apertureHeight;

	cout << "focalLength X: " << K.at<double>(0, 0) / mx << endl;
	cout << "focalLength Y: " << K.at<double>(1, 1) / my << endl;
	cout << "principalPoint : " << principalPoint << endl;
	cout << "aspectRatio : " << aspectRatio << endl;

	




	waitKey();

}

