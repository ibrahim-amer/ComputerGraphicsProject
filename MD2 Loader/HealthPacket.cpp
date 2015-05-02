#pragma once

#include "HealthPacket.h"


#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "Bullet.h"

const float HealthPacket::DefaultLifeTime = 3;

/**
	Moves the bullet and updates its lifetime.
	*/
void HealthPacket::Update(float dtSec) {
	
	P.z += dtSec * V.z;
	LifeTime -= dtSec;
}


void HealthPacket::Render() const {
	glPushMatrix();
		glTranslatef(P.x, P.y, P.z);
		glColor3f(0.8196, 0.537, 0.8784);
		glutWireSphere(1, 20, 20);
	glPopMatrix();

}

void HealthPacket::UpdateAdvancedBullet(float dt)
{
	P.x += V.x * dt;
	P.y += V.y * dt;
	P.z += V.z * dt;
}



