#include "fitting2d.h"
#include <math.h>
#include <iostream>

using namespace std;

GNewton::GNewton(Mat dst, Mat src, Mat init_param)
{
	res_mat.create(3, 8, CV_64F);
	dst.create(3, 8, CV_64F);
	src.create(3, 8, CV_64F);
	param.create(4, 0, CV_64F);
	jacobian.create(16, 4, CV_64F);
	this->dst = dst;
	this->src = src;
	param = init_param;

}

void GNewton::step()
{
	Mat delta;
	double residual = 0;

	res_mat = dst - function();
	for (int i = 0; i < res_mat.cols; ++i)
	{
		residual += pow(res_mat.at<double>(0, i), 2.0);
		residual += pow(res_mat.at<double>(1, i), 2.0);
	}
	cout << residual << endl;
	calc_jaco();
	Mat temp = jacobian.t() * jacobian;
	Mat temp2(16, 1, CV_64F);
	for (int i = 0; i < 8; ++i)
	{
		temp2.at<double>(i*2, 0) = res_mat.at<double>(0, i);
		temp2.at<double>(i*2+1, 0) = res_mat.at<double>(1, i);

	}
	
	param = param - temp.inv() * jacobian.t() * temp2;
}

Mat GNewton::function()
{
	Mat result(3, 8, CV_64F);
	Mat temp(3, 8, CV_64F);
	temp = src.clone();
	rotate(temp, param.at<double>(0, 0));
	for (int i = 0; i < 8; ++i)
	{
		result.at<double>(0, i)= param.at<double>(3, 0) * temp.at<double>(0, i) + param.at<double>(1, 0);
		result.at<double>(1, i) = param.at<double>(3, 0) * temp.at<double>(1, i) + param.at<double>(2, 0);
		result.at<double>(2, i) = 1.0;
	}
	cout << result << endl;
	return result;
}

void GNewton::rotate(Mat &dst, double angle)
{
	for (int i = 0; i < 8; ++i)
	{
		dst.at<double>(0, i) = cos(angle) * (src.at<double>(0, i) - 15) - sin(angle) * (src.at<double>(1, i) - 5) + 15;
		dst.at<double>(1, i) = sin(angle) * (src.at<double>(0, i) - 15) + cos(angle) * (src.at<double>(1, i) - 5) + 5;
	}
}

void GNewton::calc_jaco()
{
	for (int i = 0; i < 8; ++i)
	{
		jacobian.at<double>(i*2, 0) =
			param.at<double>(3, 0) * sin(param.at<double>(0, 0)) * src.at<double>(0, i)
			+ param.at<double>(3, 0) * cos(param.at<double>(0, 0)) * src.at<double>(1, i);
		jacobian.at<double>(i*2, 1) = -1.0;
		jacobian.at<double>(i*2, 2) = 0.0;
		jacobian.at<double>(i * 2, 3) = -cos(param.at<double>(0, 0)) * src.at<double>(0, i)
			+ sin(param.at<double>(0, 0)) * src.at<double>(1, i);

		jacobian.at<double>(i*2 + 1, 0) =
			-param.at<double>(3, 0) * cos(param.at<double>(0, 0)) * src.at<double>(0, i)
			+ param.at<double>(3, 0) * sin(param.at<double>(0, 0)) * src.at<double>(1, i);
		jacobian.at<double>(i*2 + 1, 1) = 0.0;
		jacobian.at<double>(i*2 + 1, 2) = -1.0;
		jacobian.at<double>(i*2 + 1, 3) = -sin(param.at<double>(0, 0)) * src.at<double>(0, i)
			- cos(param.at<double>(0, 0)) * src.at<double>(1, i);
	}
} 