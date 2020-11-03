#pragma once
#include "TrackingObjectSet.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
class Tracker
{
private:
	int registered_id;
public:
	TrackingObjectSet *objsSet;

	Tracker() {
		this->objsSet = new TrackingObjectSet();
		registered_id = 0;
	}
	virtual Scalar Track(Rect new_rect, int rows);
	Scalar AddNewObject(Rect new_obj_position);
	Scalar AddExistingObject(int objIdx, Rect new_obj_position);
	int whichObjectBelong(Rect new_object_position, int rows);
	int whichObjectBelong(Rect new_object_position);
	void UpdateObject();
	double CalcDistance(Rect newBlob, Rect Obj);
	int rectCounter(Rect passedRect);
};

