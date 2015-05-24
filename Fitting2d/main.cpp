#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include "fitting2d.h"

using namespace std;
using namespace cv;

Mat temp, img;
Point m_pt(550, 450);	//point of mouse
double scale = 1.0;
vector<Point2d> t_pts, f_pts, init_pts;	//target points, fitted points

void onMouse(int event, int x, int y, int, void*);
void rotate(double angle);

int main(int argc, char ** argv)
{
	img = Mat(480, 640, CV_8UC3);
	img = Scalar(255, 255, 255);
	
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			t_pts.push_back(Point2d(i * 50 + 10, j * 50 + 10));
			f_pts.push_back(Point2d(i * 10, j * 10));
			init_pts.push_back(Point2d(i * 10, j * 10));
		}
	}

	line(img, t_pts[0], t_pts[1], Scalar(255, 0, 0), 1, CV_AA);
	line(img, t_pts[2], t_pts[3], Scalar(255, 0, 0), 1, CV_AA);
	line(img, t_pts[4], t_pts[5], Scalar(255, 0, 0), 1, CV_AA);
	line(img, t_pts[6], t_pts[7], Scalar(255, 0, 0), 1, CV_AA);

	line(img, t_pts[0], t_pts[6], Scalar(255, 0, 0), 1, CV_AA);
	line(img, t_pts[1], t_pts[7], Scalar(255, 0, 0), 1, CV_AA);

	namedWindow("disp");
	setMouseCallback("disp", onMouse, 0);
	temp = img.clone();
	
	int key;
	bool run = true;
	double ang = 0;
	
	while(run)
	{
		rotate(ang);
		for (int i = 0; i < t_pts.size(); ++i)
		{
			circle(temp, t_pts[i], 2, Scalar(255, 0, 0), 2, CV_AA);
			circle(temp, Point2d(scale * f_pts[i].x + m_pt.x,
				scale * f_pts[i].y + m_pt.y), 2, Scalar(0, 255, 0), 2, CV_AA);
		}

		imshow("disp", temp);
		key = waitKey(30);
		temp = img.clone();
		
		switch (key)
		{
		case ' ':
			run = false;
			break;
		case 'w':
			scale += 0.1;
			break;
		case 's':
			scale -= 0.1;
			break;
		case 'W':
			scale += 1.0;
			break;
		case 'S':
			scale -= 1.0;
			break;
		case 'a':
			ang -= 1.0;
			break;
		case 'd':
			ang += 1.0;
			break;
		default:
			break;
		}
	}

	Mat dst(3, 8, CV_64F), src(3, 8, CV_64F), param(4, 1, CV_64F);
	for (int i = 0; i < 8; ++i)
	{
		dst.at<double>(0, i) = t_pts[i].x;
		dst.at<double>(1, i) = t_pts[i].y;
		dst.at<double>(2, i) = 1.0;
		src.at<double>(0, i) = init_pts[i].x;
		src.at<double>(1, i) = init_pts[i].y;
		src.at<double>(2, i) = 1.0;
	}
	
	param.at<double>(0, 0) = (CV_PI / 180.0) * ang;
	param.at<double>(1, 0) = m_pt.x;
	param.at<double>(2, 0) = m_pt.y;
	param.at<double>(3, 0) = scale;

	cout << dst << endl;
	cout << src << endl;
	cout << param << endl;

	GNewton gn(dst, src, param);
	for (int i = 0; i < 10; ++i)
	{
		gn.step();
	}
	cout << gn.param << endl;
	temp = img.clone();
	rotate(param.at<double>(0, 0) * (180 / CV_PI));
	for (int i = 0; i < t_pts.size(); ++i)
	{
		circle(temp, t_pts[i], 2, Scalar(255, 0, 0), 2, CV_AA);
		circle(temp, Point2d(param.at<double>(3, 0) * f_pts[i].x + param.at<double>(1, 0),
			param.at<double>(3, 0) * f_pts[i].y + param.at<double>(2, 0)), 2, Scalar(0, 255, 0), 2, CV_AA);
	}
	imshow("disp", temp);
	waitKey(0);
	return 0;
}

void rotate(double angle)
{
	angle = (CV_PI / 180) * angle;
	for (int i = 0; i < 8; ++i)
	{
		f_pts[i].x = cos(angle) * (init_pts[i].x - 15) - sin(angle) * (init_pts[i].y - 5) + 15;
		f_pts[i].y = sin(angle) * (init_pts[i].x - 15) + cos(angle) * (init_pts[i].y - 5) + 5;
	}
}

void onMouse(int event, int x, int y, int, void*)
{
	temp = img.clone();
	line(temp, Point(0, y), Point(temp.cols + 1, y), Scalar(0, 200, 0));
	line(temp, Point(x, 0), Point(x, temp.rows + 1), Scalar(0, 200, 0));

	static bool move = false;
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		move = true;
		break;
	case EVENT_LBUTTONUP:
		move = false;
		break;
	case EVENT_MOUSEMOVE:
		if (move)
		{
			m_pt.x = x;
			m_pt.y = y;
		}
		break;
	}
}