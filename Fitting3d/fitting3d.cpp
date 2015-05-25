#include "fitting3d.h"

using namespace cv;

void draw_cube(vector<Point2d> img_pts, Mat frame)
{
	for (unsigned int i = 0; i < img_pts.size() - 4; ++i)
		circle(frame, img_pts[i], 2, Scalar(100, 100, 0));

	line(frame, img_pts[0], img_pts[1], Scalar(100, 100, 0));
	line(frame, img_pts[1], img_pts[2], Scalar(100, 100, 0));
	line(frame, img_pts[2], img_pts[3], Scalar(100, 100, 0));
	line(frame, img_pts[3], img_pts[0], Scalar(100, 100, 0));

	line(frame, img_pts[4], img_pts[5], Scalar(100, 100, 0));
	line(frame, img_pts[5], img_pts[6], Scalar(100, 100, 0));
	line(frame, img_pts[6], img_pts[7], Scalar(100, 100, 0));
	line(frame, img_pts[7], img_pts[4], Scalar(100, 100, 0));

	line(frame, img_pts[0], img_pts[4], Scalar(100, 100, 0));
	line(frame, img_pts[1], img_pts[5], Scalar(100, 100, 0));
	line(frame, img_pts[2], img_pts[6], Scalar(100, 100, 0));
	line(frame, img_pts[3], img_pts[7], Scalar(100, 100, 0));
}

Mat generate_rmat(vec3 axis, double angle)
{
	Mat cmat = (Mat_<double>(3, 3) <<
		0, -axis.z, axis.y,
		axis.z,  0, -axis.x,
		-axis.y, axis.x, 0);
	
	angle = (CV_PI / 180) * angle;
	Mat temp = Mat::eye(3, 3, CV_64F) + sin(angle) * cmat + (1 - cos(angle)) * cmat * cmat;
	return temp;
}

void apply_rotate(vector<Point3d> src, vector<Point3d> dst, Mat rmat)
{
	for (unsigned int i = 0; i < src.size(); ++i)
	{
		dst[i].x = rmat.at<double>(0, 0) * src[i].x
			+ rmat.at<double>(0, 1) * src[i].y
			+ rmat.at<double>(0, 2) * src[i].z;
		dst[i].y = rmat.at<double>(1, 0) * src[i].x
			+ rmat.at<double>(1, 1) * src[i].y
			+ rmat.at<double>(1, 2) * src[i].z;
		dst[i].z = rmat.at<double>(2, 0) * src[i].x
			+ rmat.at<double>(2, 1) * src[i].y
			+ rmat.at<double>(2, 2) * src[i].z;
	}
}

void rotate_axis(Mat rmat, vec3 &dst, vec3 src)
{
	vec3 buf(0, 0, 0);
	dst.x = rmat.at<double>(0, 0) * src.x 
		+ rmat.at<double>(0, 1) * src.y 
		+ rmat.at<double>(0, 2) * src.z;
	dst.y = rmat.at<double>(1, 0) * src.x
		+ rmat.at<double>(1, 1) * src.y
		+ rmat.at<double>(1, 2) * src.z;
	dst.z = rmat.at<double>(2, 0) * src.x
		+ rmat.at<double>(2, 1) * src.y
		+ rmat.at<double>(2, 2) * src.z;
}

//void Cube::rotate_x(GLfloat angle)
//{
//	Mat cmat = (Mat_<float>(3, 3) <<
//		0, -xVec.at<float>(2, 0), xVec.at<float>(1, 0),
//		xVec.at<float>(2, 0), 0, -xVec.at<float>(0, 0),
//		-xVec.at<float>(1, 0), xVec.at<float>(0, 0), 0);
//	angle = (CV_PI / 180) * angle;
//	Mat rollingMat = Mat::eye(3, 3, CV_32F) + sin(angle) * cmat + (1 - cos(angle)) * cmat * cmat;
//	rmat = rollingMat * rmat;
//}