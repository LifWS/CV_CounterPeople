#include "TrackingObject.h"

TrackingObject::TrackingObject(Rect register_object, int objId) {
	srand(time(0));
	life = 10;
	this->isCounted = false;
	this->objId = objId;
	object_path.push_back(register_object);
	int b = rand() % 256;
	int g = rand() % 256;
	int r = rand() % 256;
	color = Scalar(b, g, r);
}

Rect TrackingObject::getCurrentPosition() {
	return object_path[object_path.size() - 1];
}

Rect TrackingObject::getPastPosition() {
	return object_path[object_path.size() - 2];
}

void TrackingObject::updateObject(Rect new_object_position) {
	if (life < 10) life++;
	object_path.push_back(new_object_position);
}

void TrackingObject::removeLife() {
	life--;
}