/*@file AABoundingBox.h
The declaration of Axis-Aligned Bounding Box class that is used for simple collision detection.
*/
#pragma once

#include "Vector3.h"

class AABoundingBox {
public: 
	Vector3 Center;
	Vector3 Dims;

	AABoundingBox() {
	}

	AABoundingBox(const Vector3& center, const Vector3 &dims) 
			: Center(center), Dims(dims)
	{}

	float& Width() { return Dims.x; }
	float& Height() { return Dims.y; }
	float& Depth() { return Dims.z; }
	
	float Width() const { return Dims.x; }
	float Height() const { return Dims.y; }
	float Depth() const { return Dims.z; }

	/**
	Checks if the 2 bounding boxes (the current and otherBox) intersect.
	*/
	bool IntersectsWith(const AABoundingBox &otherBox) const;

	void Render() const;
};
