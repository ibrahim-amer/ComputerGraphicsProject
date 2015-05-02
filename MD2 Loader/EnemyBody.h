#pragma once

#include "Vector3.h"
#include "AABoundingBox.h"
#include "Bullet.h"
#include "gl/glut.h"

const float EnemySize = 10;

class EnemyBody {
public:
	Vector3 P;
	float TimeSinceLastFire;
	float InterBulletTime;
	

	EnemyBody(const Vector3 &pos) {
		P = pos;
		TimeSinceLastFire = 0;
		InterBulletTime = 4;
	}

	void Update(float dtSec) {
		TimeSinceLastFire += dtSec;
		//P = P + dtSec;
	}

	AABoundingBox GetAABB() const {
		Vector3 dims(EnemySize, EnemySize, EnemySize);
		return AABoundingBox(P, 2*dims);
	}

	void Render() const {
		glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(P.x, P.y, P.z);
			glutSolidCone(5, 20, 10, 10);
		glPopMatrix();
	}

	bool FireReady() const { return TimeSinceLastFire >= InterBulletTime; }

	Bullet ShootAt(const Vector3 &target) {
		Vector3 bulDir = (target - P);
		bulDir.Normalize();
		Bullet bul(P, 40 * bulDir);
		TimeSinceLastFire = 0;
		return bul;
	}
};