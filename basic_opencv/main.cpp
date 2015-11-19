//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video/background_segm.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

//C++
#include <iostream>
#include <sstream>
#include <array>
#include <cstdint>
using namespace cv;
using namespace std;

Mat getNeighborsCoordinates(int xx, int yy);
std::vector<Point> getNeighborsCoordinatesPoint(Point current_coord);
std::vector<Point> getEndPointsMat(Mat *im);
std::vector<int> compare2vectorsOfPoint(vector<Point> v1, vector<Point> v2);
std::vector<int> compare2vectorsOfInt(vector<int> v1, vector<int> v2);

int main(int argc, char* argv[])
{
	Mat image = Mat::zeros( 300, 300, CV_8U );
	Mat image2 = Mat::zeros( 300, 300, CV_8UC3 );
   
	// Draw a line 
	//line( image, Point( 15, 20 ), /*Point( 20, 80)*/ Point(20, 20), Scalar( 255, 255, 255 ),  1, 4 );
	line( image, Point( 15, 20 ), Point( 70, 20), Scalar( 110, 220, 0 ),  1, 8 );
    std::vector<int> coords;		
	//cout << "M = "<< endl << " "  << image << endl << endl;
	Mat skeleton_coordinates;
	cv::findNonZero(image, skeleton_coordinates);	
	Mat test = getNeighborsCoordinates(15, 20);
	//cout << "M = "<< endl << " "  << test << endl << endl;
	std::vector<Point> t = getEndPointsMat(&image);
	for (std::vector<Point>::size_type i = 0; i != t.size(); i++)
    {
		circle(image2, t[i],1.0,Scalar( 255, 0, 255 ), 1, 8);
		//cout << "(" << t[i].x << "," << t[i].y << ")" << endl;
	}
	imshow("Image", image2);
	waitKey( 0 );
}

std::vector<Point> getEndPointsMat(Mat *im)
{
	vector<Point> endpoints;
	vector<Point> skeleton_coordinates;
	Mat m_coordinates;
	cv::findNonZero(*im, m_coordinates);
	
	//cout << "Pos = "<< endl << " "  << m_coordinates << endl << endl;
	/*for (int i = 1; i < m_coordinates.rows; i++)
	{
		int current_x = m_coordinates.at<uchar>(i, 1); // cols
		int current_y = m_coordinates.at<uchar>(i, 2); // rows
		Mat neighbor = getNeighborsCoordinates(current_x, current_y);
		Mat pos= m_coordinates == neighbor;
		cout << "Pos = "<< endl << " "  << pos << endl << endl;
		bool eq = cv::countNonZero(m_coordinates != neighbor) == 0;
		break;
	}*/
	m_coordinates.copyTo(skeleton_coordinates);
	for (std::vector<Point>::iterator it = skeleton_coordinates.begin(); it != skeleton_coordinates.end(); ++it)
    {
		Point current_coord = Point(it->x, it->y);
		vector<Point> neighbors = getNeighborsCoordinatesPoint(current_coord);
		cout << "(" << current_coord.x << "," << current_coord.y << ")  [";
		for (std::vector<int>::size_type k = 0; k != neighbors.size(); k++)
		{
			cout << neighbors[k].x << "," << neighbors[k].y << " ";
		}
		cout << "] ";
		vector<int> pos = compare2vectorsOfPoint(neighbors, skeleton_coordinates);
		for (std::vector<int>::size_type l = 0; l != pos.size(); l++)
		{
			cout << pos[l] << ",";
		}
		cout << endl;
		//cout << "Pos = " << pos[1] << endl;
		
		if (pos.size() == 0)
		{
			cout << "zero" << endl;
			endpoints.push_back(current_coord);
		}
		else {
			bool present = false;
			array<int, 3> v_init = {4,5,6};
			// Test all the pixels around this current pixel
			for (std::vector<int>::size_type i = 0; i != pos.size(); i++)
			{				
				vector<int> continuity(v_init.begin(), v_init.end());
				int current_pos = pos[i];
				for (unsigned int p=0; p < v_init.size(); ++p) {
					continuity[p] += current_pos;
					if (continuity[p] > 8)
						continuity[p] = (continuity[p] % 9) + 1;
					//cout << "Vals = " << continuity[p] << endl;
					vector<int> mb = compare2vectorsOfInt(continuity, pos);
					if (mb.size() > 0) {
						present = true;
						break;
					}
				}
				

			}
			if (present) {
				endpoints.push_back(current_coord);
			}
		}
		//break;
	}

	return endpoints;
}

Mat getNeighborsCoordinates(int xx, int yy)
{
	Mat m = (Mat_<int>(8, 2) << 
		xx + 1, yy, 
		xx - 1, yy, 
		xx, yy + 1,
		xx, yy - 1,
		xx + 1, yy + 1,
		xx + 1, yy - 1,
		xx - 1, yy + 1, 
		xx - 1, yy - 1);
	
	return m;
}

std::vector<Point> getNeighborsCoordinatesPoint(Point current_coord)
{
	int xx = current_coord.x;
	int yy = current_coord.y;
	vector<Point> neighbor;
	neighbor.push_back(Point(xx + 1, yy));
	neighbor.push_back(Point(xx - 1, yy));
	neighbor.push_back(Point(xx, yy + 1));
	neighbor.push_back(Point(xx, yy - 1));
	neighbor.push_back(Point(xx + 1, yy + 1));
	neighbor.push_back(Point(xx + 1, yy - 1));
	neighbor.push_back(Point(xx - 1, yy + 1));
	neighbor.push_back(Point(xx - 1, yy - 1));

	return neighbor;
}

std::vector<int> compare2vectorsOfPoint(vector<Point> v1, vector<Point> v2)
{
	std::vector<int> position;
	for (std::vector<Point>::size_type i = 0; i != v1.size(); i++)
    {
		for (std::vector<Point>::size_type j = 0; j != v2.size(); j++)
		{
			if (v1[i].x == v2[j].x && v1[i].y == v2[j].y)
			{
				position.push_back(i);
				break;
			}
		}
	}
	return position;
}

std::vector<int> compare2vectorsOfInt(vector<int> v1, vector<int> v2)
{
	std::vector<int> position;
	for (std::vector<int>::size_type i = 0; i != v1.size(); i++)
    {
		for (std::vector<int>::size_type j = 0; j != v2.size(); j++)
		{
			if (v1[i] == v2[j])
			{
				position.push_back(i);
				break;
			}
		}
	}
	return position;
}


