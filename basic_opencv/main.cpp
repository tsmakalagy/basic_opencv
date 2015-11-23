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
std::vector<Point> getJunctions(Mat *im);

int main(int argc, char* argv[])
{
	Mat image = Mat::zeros( 300, 300, CV_8U );
	Mat image2 = Mat::zeros( 300, 300, CV_8UC3 );
	if(argc > 1) {
		cv::Mat imageMat = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
		if (imageMat.empty())
		{
				std::cerr << "ERROR: Could not read image " << argv[1] << std::endl;
				return 1;
		}

		//Grayscale matrix
		cv::Mat grayscaleMat (imageMat.size(), CV_8U);

		//Convert BGR to Gray
		cv::cvtColor( imageMat, grayscaleMat, CV_BGR2GRAY );

		//Binary image
		cv::Mat binaryMat(grayscaleMat.size(), grayscaleMat.type());

		//Apply thresholding
		cv::threshold(grayscaleMat, image, 100, 255, cv::THRESH_BINARY);
	} else {
		line( image, Point( 15, 20 ), Point( 70, 20), Scalar( 110, 220, 0 ),  1, 8 );
	}
   
	// Draw a line 
	//line( image, Point( 15, 20 ), /*Point( 20, 80)*/ Point(20, 20), Scalar( 255, 255, 255 ),  1, 4 );
	
    //std::vector<int> coords;		
	//cout << "M = "<< endl << " "  << image << endl << endl;
	//Mat skeleton_coordinates;
	//cv::findNonZero(image, skeleton_coordinates);	
	//cout << "M = "<< endl << " "  << test << endl << endl;
	std::vector<Point> t = getEndPointsMat(&image);
	for (std::vector<Point>::size_type i = 0; i != t.size(); i++)
    {
		//circle(image, t[i],2,Scalar( 255, 0, 255 ), 1, 8);
		//cout << "(" << t[i].x << "," << t[i].y << ")" << endl;
	}
	std::vector<Point> junc = getJunctions(&image);
	for (std::vector<Point>::size_type i = 0; i != junc.size(); i++)
    {
		circle(image, junc[i],2,Scalar( 255, 0, 255 ), 1, 8);
		//cout << "(" << t[i].x << "," << t[i].y << ")" << endl;
	}
	imshow("Image", image);
	waitKey( 0 );
}

std::vector<Point> getEndPointsMat(Mat *im)
{
	vector<Point> endpoints;
	vector<Point> skeleton_coordinates;
	Mat m_coordinates;
	cv::findNonZero(*im, m_coordinates);
	
	m_coordinates.copyTo(skeleton_coordinates);
	for (std::vector<Point>::iterator it = skeleton_coordinates.begin(); it != skeleton_coordinates.end(); ++it)
    {
		Point current_coord = Point(it->x, it->y);
		vector<Point> neighbors = getNeighborsCoordinatesPoint(current_coord);
		vector<int> pos = compare2vectorsOfPoint(neighbors, skeleton_coordinates);
		if (pos.size() == 1)
		{
			endpoints.push_back(current_coord);
		}
		else {
			/*bool present = false;
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
				}
				
				vector<int> mb = compare2vectorsOfInt(continuity, pos);
				if (mb.size() > 0) {
					present = true;
					break;
				}
			}
			if (!present) {
				endpoints.push_back(current_coord);
			}*/
			//if (pos.size() > 1)
				//endpoints.push_back(current_coord);
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

std::vector<Point> getJunctions(Mat *im)
{
	std::vector<Point> junctions;
	int p1;


	// For each pixel in our image...
	for (int i = 1; i < im->rows-1; i++) {
		for (int j = 1; j < im->cols-1; j++) {
			

			// See what the pixel is at this location
			p1 = im->at<uchar>(i,j);

			// If not a skeleton point, skip
			if (p1 == 0) {				
				continue;
			}
			
			uchar p2 = im->at<uchar>(i-1, j);
			uchar p3 = im->at<uchar>(i-1, j+1);
			uchar p4 = im->at<uchar>(i, j+1);
			uchar p5 = im->at<uchar>(i+1, j+1);
			uchar p6 = im->at<uchar>(i+1, j);
			uchar p7 = im->at<uchar>(i+1, j-1);
			uchar p8 = im->at<uchar>(i, j-1);
			uchar p9 = im->at<uchar>(i-1, j-1);

			int same = (p9==p1);
                same+=(p8==p1);
                same+=(p7==p1);
                same+=(p6==p1);
                same+=(p5==p1);
                same+=(p4==p1);
                same+=(p3==p1);
                same+=(p2==p1);
			//cout << "Same:" << same << endl;
             if (same==2)
             {
				 junctions.push_back(Point(i,j));
			 }
			
		}
	}
	
	return junctions;
}
