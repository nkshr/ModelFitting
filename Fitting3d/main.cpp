#include <iostream>
#include <opencv2/opencv_lib.hpp>
#include <opencv2/opencv.hpp>
#include "fitting3d.h"

using namespace std;
using namespace cv;


#define WIDTH 640
#define HEIGHT 480

Mat img(HEIGHT, WIDTH, CV_64FC3), frame;
Mat m_tvec = (Mat_<double>(3, 1) << 0, 0, -50);	//model transfer vector
vector<Point2d> dst_img_pts;
Mat m_rmat = Mat::eye(3, 3, CV_64F), m_rvec;
vector<Point2d> temp_img_pts;
Mat cameraMatrix = (Mat_<double>(3, 3) << 850, 0, 320, 0, 850, 240, 0, 0, 1);
Mat distCoeffs, rvec, tvec, rmat = Mat::eye(3, 3, CV_64F);
vector<Point3d> m_obj_pts, temp_obj_pts;

void onMouse(int event, int x, int y, int, void*);

int main(int argc, char ** argv)
{
	double length = 10;
	m_obj_pts.push_back(Point3d(length, length, length));
	m_obj_pts.push_back(Point3d(-length, length, length));
	m_obj_pts.push_back(Point3d(-length, -length, length));
	m_obj_pts.push_back(Point3d(length, -length, length));

	m_obj_pts.push_back(Point3d(length, length, -length));
	m_obj_pts.push_back(Point3d(-length, length, -length));
	m_obj_pts.push_back(Point3d(-length, -length, -length));
	m_obj_pts.push_back(Point3d(length, -length, -length));

	tvec = (Mat_<double>(3, 1) << 0, 0, -100);
	Rodrigues(rmat, rvec);
	projectPoints(m_obj_pts, rvec, tvec, cameraMatrix, distCoeffs, dst_img_pts);

	img = Scalar(33, 33, 33);
	namedWindow("display", WINDOW_NORMAL);

	draw_cube(dst_img_pts, img);
	frame = img.clone();
	//setMouseCallback("display", onMouse, 0);
	int key;
	bool disp = true;
	vec3 waxis_x(1, 0, 0), waxis_y(0, 1, 0), waxis_z(0, 0, 1);
	vec3 axis_x(1, 0, 0), axis_y(0, 1, 0), axis_z(0, 0, 1);
	vector<Point3d> axis_pts3d;
	vector<Point2d> axis_pts2d;
	
	m_obj_pts.push_back(Point3d(waxis_x.x * 20, waxis_x.y * 20, waxis_x.z * 20));
	m_obj_pts.push_back(Point3d(waxis_y.x * 20, waxis_y.y * 20, waxis_y.z * 20));
	m_obj_pts.push_back(Point3d(waxis_z.x * 20, waxis_z.y * 20, waxis_z.z * 20));
	m_obj_pts.push_back(Point3d(0, 0, 0));

	while (disp)
	{
		frame = img.clone();
		Rodrigues(m_rmat, m_rvec);
		
		projectPoints(m_obj_pts, m_rvec, m_tvec, cameraMatrix, distCoeffs, temp_img_pts);
		draw_cube(temp_img_pts, frame);
	
		line(frame, temp_img_pts[11], temp_img_pts[8], Scalar(100, 0, 0));
		line(frame, temp_img_pts[11], temp_img_pts[9], Scalar(0, 100, 0));
		line(frame, temp_img_pts[11], temp_img_pts[10], Scalar(0, 0, 100));

		imshow("display", frame);
		key = waitKey(30);
		switch (key)
		{
		case 27:
			disp = false;
			break;
		case '[':
			m_tvec.at<double>(2, 0) += 1;
			break;
		case ']':
			m_tvec.at<double>(2, 0) -= 1;
			break;
		case 'a':
			m_tvec.at<double>(0, 0) += 1;
			break;
		case 'd':
			m_tvec.at<double>(0, 0) -= 1;
			break;
		case 'w':
			m_tvec.at<double>(1, 0) += 1;
			break;
		case 's':
			m_tvec.at<double>(1, 0) -= 1;
			break;

		case'l':
			m_rmat = (generate_rmat(axis_y, 1) * m_rmat);
			break;
		case'j':
			m_rmat = (generate_rmat(axis_y, -1) * m_rmat);
			break;
		case 'L':
			m_rmat = (generate_rmat(axis_z, 1) * m_rmat);
			break;
		case 'J':
			m_rmat = generate_rmat(axis_z, -1) * m_rmat;
			break;
		case 'i':
			m_rmat = generate_rmat(axis_x, 1) * m_rmat;
			break;
		case 'k':
			m_rmat = generate_rmat(axis_x, -1) * m_rmat;
			break;
		default:
			break;
		}
		rotate_axis(m_rmat, axis_x, waxis_x);
		rotate_axis(m_rmat, axis_y, waxis_y);
		rotate_axis(m_rmat, axis_z, waxis_z);
	}
	return 0;
}

void onMouse(int event, int x, int y, int, void*)
{
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
			m_tvec.at<double>(0, 0) = x;
			m_tvec.at<double>(1, 0) = y;
		}
		break;
	}
}