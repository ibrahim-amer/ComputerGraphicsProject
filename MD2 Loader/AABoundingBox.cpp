#include "AABoundingBox.h"
#include <gl/glut.h>


void AABoundingBox::Render() const {
	glPushMatrix();
		glTranslatef(Center.x, Center.y, Center.z);
		glScalef(Dims.x, Dims.y, Dims.z);
		glColor3f(0.2f, 1, 0);
		glutWireCube(1);
	glPopMatrix();	
}

bool AABoundingBox::IntersectsWith(const AABoundingBox &otherBox) const  {
	Vector3 diff = Center - otherBox.Center;
	return abs(diff.x) < 0.5 * (Width() + otherBox.Width()) &&
		abs(diff.y) < 0.5 * (Height() + otherBox.Height()) &&
		abs(diff.z) < 0.5 * (Depth() + otherBox.Depth());
}
