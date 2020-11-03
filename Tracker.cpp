#include "Tracker.h"
Scalar Tracker::Track(Rect new_rect, int rows) {
	int objidx = whichObjectBelong(new_rect, rows);
	Scalar color;
	if (objidx == -1) // not exist
	{
		color = AddNewObject(new_rect);
	}
	else // exist
	{
		color = AddExistingObject(objidx, new_rect);
	}
	UpdateObject();
	return color;
}

int Tracker::rectCounter(Rect passedRect) {
	int objIdx = whichObjectBelong(passedRect);
	TrackingObject* temp = &objsSet->objs[objIdx];
	if (!temp->isCounted) {
		auto current = temp->getCurrentPosition();
		auto past = temp->getPastPosition();
		int direction = current.y - past.y;
		temp->isCounted = true;
		if (direction > 0) {
			return -1;
		}
		else {
			return 1;
		}

	}
	return 0;
}

Scalar Tracker::AddNewObject(Rect new_obj_position) {
	TrackingObject temp = TrackingObject(new_obj_position, registered_id);
	registered_id++;
	if (registered_id > 20000) registered_id = 0;
	objsSet->objs.push_back(temp);
	objsSet->objs[objsSet->objs.size() - 1].updateObject(new_obj_position);
	return objsSet->objs[objsSet->objs.size() - 1].color;
}

Scalar Tracker::AddExistingObject(int objIdx, Rect new_obj_position) {
	objsSet->objs[objIdx].updateObject(new_obj_position);
	return objsSet->objs[objsSet->objs.size() - 1].color;
}

int Tracker::whichObjectBelong(Rect new_object_position, int rows) {
	// check minimum distance
	int objIdx = -1;
	double minDist = 10000, dist;
	//if ((new_object_position.y < objsSet->BORDER_RADIUS) || (rows -  (new_object_position.y + new_object_position.height) < objsSet->BORDER_RADIUS)) {
	//	return -1;
	//}
	for (size_t i = 0; i < objsSet->objs.size(); i++) {
		dist = CalcDistance(new_object_position, objsSet->objs[i].getCurrentPosition());
		if (dist < minDist) {
			minDist = dist;
			objIdx = i;
		}
	}
	if (minDist > objsSet->TRACKER_RADIUS)
		objIdx = -1;
	return objIdx;
}

int Tracker::whichObjectBelong(Rect new_object_position) {
	// check minimum distance
	int objIdx = -1;
	double minDist = 10000, dist;
	for (size_t i = 0; i < objsSet->objs.size(); i++) {
		dist = CalcDistance(new_object_position, objsSet->objs[i].getCurrentPosition());
		if (dist < minDist) {
			minDist = dist;
			objIdx = i;
		}
	}
	if (minDist > objsSet->TRACKER_RADIUS)
		objIdx = -1;
	return objIdx;
}

bool isZeroLife(const TrackingObject &obj) {
	if (obj.life <= 0)
		return true;
	return false;
}

void Tracker::UpdateObject() {
	for (size_t i = 0; i < objsSet->objs.size(); i++)
		objsSet->objs[i].removeLife();
	objsSet->objs.erase(remove_if(objsSet->objs.begin(), objsSet->objs.end(), isZeroLife), objsSet->objs.end());
}

double Tracker::CalcDistance(Rect newBlob, Rect Obj) {
	return sqrt(pow(newBlob.x - Obj.x, 2) + pow(newBlob.y - Obj.y, 2));
}


