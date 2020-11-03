#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace cv;

class TrackingObject
{
private:
	int objId;
	vector<Rect> object_path; //store collection of person position

public:

	int life;
	Scalar color;
	TrackingObject(Rect regiter_object, int objId);
	Rect getCurrentPosition();
	void removeLife();
	void updateObject(Rect new_object_position);
	bool isCounted;
	Rect getPastPosition();

};

