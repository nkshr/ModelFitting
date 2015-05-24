#include <opencv2/opencv.hpp>

using namespace cv;

class GNewton
{
public:
	Mat param;

	GNewton(Mat dst, Mat src, Mat init_param);
	void step();
private:
	Mat function();
	Mat res_mat, dst, src, jacobian;	//residual matrix

	void calc_jaco();
	void rotate(Mat &dst, double angle);
};