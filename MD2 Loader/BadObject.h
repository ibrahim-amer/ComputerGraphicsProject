#include"Vector3.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include"AABoundingBox.h"
#ifndef BADOBJECT
#define BADOBJECT
class BadObject
{
public:
	float size;
	Vector3 P;
	BadObject()
	{
		//P = Vector3(0, 0, 0); 
		//size = 1;
	}

	BadObject(Vector3 P)
	{
		this->P = P;
		size = 1;
	}

	void Render()
	{
		glPushMatrix();
		{

			glTranslatef(P.x, P.y, P.z);
			glColor3f(1, 0, 0);
			glutSolidTorus(size / 2, size * 2, 20, 20) ;
		}
		glPopMatrix();
	}

	AABoundingBox GetAABB() const {
		Vector3 dims(size, size, size);
		return AABoundingBox(P, 2*dims);
	}
};
#endif