#include <opencv2/opencv.hpp>

using namespace cv;


struct vec3
{
	double x, y, z;
	vec3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};


Mat generate_rmat(vec3 axis, double angle);
void apply_rotate(vector<Point3d> src, vector<Point3d> dst, Mat rmat);
void draw_cube(vector<Point2d> img_pts, Mat frame);
void rotate_axis(Mat rmat, vec3 &dst, vec3 src);