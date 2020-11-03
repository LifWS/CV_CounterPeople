#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Tracker.h"
#include "TrackingObjectSet.h"
#include <vector>

using namespace cv;
using namespace std;

Mat src, src_gray, dst, blurr, dst1, dst2, dst3,acc, background, motion;
int KERNEL_SIZE = 13;
float alpha = 0.4;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
Mat morpho_kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
//Mat morpho_kernel1 = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
Tracker tracker = Tracker();
int total = 0;


/// Function header
void thresh_callback(int, void*);

/**
* @function main
*/
int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	VideoCapture cap("video.avi");

	
	while (cap.isOpened()) {
		
		cap.read(src);
		cap.read(src_gray);
		acc = Mat::zeros(src.size(), CV_32FC3);

		blur(src, src, Size(3, 3));
		absdiff(src, src_gray, dst1);//ภาพ1ลบภาพก่อนหน้า
		cvtColor(dst1, dst2, COLOR_BGR2GRAY);//ทำให้ภาพเป็นสีเทา
		blur(dst2, dst3, Size(3, 3));

		absdiff(src, src_gray, dst);//ภาพ1ลบภาพก่อนหน้า
		cvtColor(dst, dst, COLOR_BGR2GRAY);//ทำให้ภาพเป็นสีเทา
		blur(dst, dst, Size(3, 3));//ทำให้ภาพสมูสจากที่เห็นพิเซลเยอะๆ
		threshold(dst, dst, 20, 255, THRESH_BINARY);//กำหนดค่ากลางเทสโฮล
		
		morphologyEx(dst, dst, MORPH_OPEN, morpho_kernel);
		
		dilate(dst, dst, morpho_kernel, Point(4, 4), 1);
		//dilate(dst, dst, morpho_kernel, Point(1, 1), 1);
		
		

		thresh_callback(0, 0);
		if (waitKey(10) >= 0) break;
	}
	/// Convert image to gray and blur it

	/// Create Window
	return(0);
}
void thresh_callback(int, void*) {
	
	findContours(dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	vector<Rect> classifiedRect;
	// Mat drawing = Mat::zeros(dst.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		if ((boundRect[i].height > 75 && boundRect[i].width > 85) && (boundRect[i].height < 200 && boundRect[i].width < 200))
			classifiedRect.push_back(boundRect[i]);
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
		//minEnclosingCircle(contours_poly[i], center[i], radius[i]);//วงกลม
	}

	// Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
	
	Mat drawing = src;
	line(drawing, Point(0, drawing.rows / 2), Point(src.cols, src.rows / 2), Scalar(0, 0, 0), 2);

	for (int i = 0; i < classifiedRect.size(); i++)
	{
		
		/*if (contourArea(contours[i]) < 2000) {//กรองขนาดกรอบที่มีขนาดเล็กหรือสิ่งที่เราไม่ต้องการออก

			continue;
		}*/
		Scalar color = tracker.Track(classifiedRect[i], src.rows);
		//drawContours(src, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		//rectangle(src, boundRect[i].tl(), boundRect[i].br(), color, 0, 255, 0);
		rectangle(src, classifiedRect[i].tl(), classifiedRect[i].br(), color, 2, 8, 0);
		//circle(src, center[i], (int)radius[i], color, 2, 8, 0);//วงกลม
		int begin = classifiedRect[i].y;
		int end = begin + classifiedRect[i].height;
		if ((begin < (drawing.rows / 2)) && (end > (drawing.rows / 2))) {
			int up_or_down = tracker.rectCounter(classifiedRect[i]);
			if (up_or_down == 1) {
				total++;
			}
			else if (up_or_down == -1) {
				total++;
			}
		}
	}
	cout << tracker.objsSet->objs.size() << endl;
	putText(drawing, "Total: ", Point(20, 425), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(10, 255, 255), 2, 2);
	putText(drawing, to_string(total), Point(150, 425), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(10, 255, 255), 2, 2);
	// drawContours(src, dst, -1, (0,255,0), 2);
	// thresh_callback(0, 0);
	imshow("src", src);
	imshow("src_gray", src_gray);
	imshow("dst", dst);
	//imshow("src", src);
	//imshow("dstDialation", dst1);
	//imshow("dst2", dst2);
	//imshow("dst3", dst3);
}