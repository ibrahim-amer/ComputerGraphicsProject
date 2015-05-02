
#include"Vector3.h"
#include"AABoundingBox.h"
#include"GameManager.h"
#ifndef REDCONE
#define REDCONE
class RedCone
{
public:
	Vector3 p, v;
	Vector3 pinit, vinit;
	RedCone()
	{
		p = Vector3();
	}
	RedCone(Vector3 p, Vector3 v)
	{
		this->p = p;
		this->v = v;
		this->pinit = p;
		this->vinit = v;
	}
	void Render()
	{
		glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(p.x, p.y, p.z);
			glutWireCone(5, 20, 10, 10);
		glPopMatrix();
	}
	void Reset()
	{
		p =pinit ; // VECTOR3( fact ,pinit.y , pinit.z  );
		v = vinit;
	}
	void Update(float dtSec) {
		//TimeSinceLastFire += dtSec;
		p.z = p.z + (2*dtSec*v.z);
		//p.y = p.y - (10*dtSec*v.y);
	}

	AABoundingBox GetAABB() const {
		Vector3 dims(EnemySize, EnemySize, EnemySize);
		return AABoundingBox(p, 2*dims);
	}
};
#endif