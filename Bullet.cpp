/**
@file Bullet.cpp

Implementation of the Bullet class.

Created on: March 2010
*/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "Bullet.h"

const float Bullet::DefaultLifeTime = 3;

/**
	Moves the bullet and updates its lifetime.
	*/
void Bullet::Update(float dt) {
	//Has the bullet died?
	//if(LifeTime < dtSec) {
	//	LifeTime = 0;
	//	//don't update
	//	return; 
	//}
	P.x += dt * V.x;
	P.z += dt * V.z;
	P.y =-50;
	LifeTime -= dt;
}

/**
Renders the bullet as a sphere whose color changes according to lifetime
(r: 1 -> 0,
 g: 0, 
 b: 0 -> 1).
*/
void Bullet::Render() const {
	glPushMatrix();
		glTranslatef(P.x, P.y, P.z);
		glColor3f(0,0,0);
		glScaled(3,3,3 );
		//glColor3f(0.3843, 0.3843, 0.3843);
		glutWireSphere(1, 20, 20);
	glPopMatrix();
}

void Bullet::UpdateAdvancedBullet(float dt)
{
	P.x += V.x * dt;
	P.y += V.y * dt;
	P.z += V.z * dt;
}



