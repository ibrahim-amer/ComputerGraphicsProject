#pragma once

#include "Vector3.h"
#include "AABoundingBox.h"
#include "Bullet.h"
#include "gl/glut.h"
#include "Classes/Headers/texture.h"
#include "Classes/Headers/md2.h"
#include "Classes/Headers/pcx.h"
class HeroTeapot {
public:
	Vector3 P;
	int teapotSize;
	CMD2Model plane;
	CMD2Model wings;
	float animTimeSec;
	float angle ;  // LEL CAM 
	float angleX ;
	float angleZ ;
    HeroTeapot(const Vector3 &pos, int size) {
		P = pos;
		teapotSize = size;
		animTimeSec = 0;
		CTextureManager::GetInstance()->Initialize();
		plane.LoadModel("models/terran/terran.md2");
		plane.LoadSkin("models/terran/terran.pcx");
		plane.SetAnim(0);

/*
		wings.LoadModel("models/airplane/wing.md2");
		wings.LoadSkin("models/airplane/air3_b.pcx");
		wings.SetAnim(0)*/;
		
	}

	AABoundingBox GetAABB() const {
		Vector3 dims(.15, .15, .15);
		return AABoundingBox(P, 2*dims);
	}
	void Render()  {
		
		glPushMatrix();
			//glColor3f(0, 0, 1);
		Vector3 saveP = P ; 
	
		glScalef(0.5, 0.5, 0.5);
		glRotatef(-180 , 0 , 1 , 0 );
		// glTranslatef(-angleX, 0, -angleZ);
	    glRotatef(-angle , 0, 1, 0);
		glScalef(0.5,0.5,0.5);
		//glColor3f(0, 0, 1);
		

			//glutSolidTeapot(teapotSize);
			glEnable(GL_TEXTURE_2D);
			plane.DrawModel(animTimeSec/100);
			//wings.DrawModel(animTimeSec);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	Bullet ShootAt(const Vector3 &target) {
		Vector3 bulDir = (target - P);
		bulDir.Normalize();
		Bullet bul(P, 40 * bulDir);
		return bul;
	}
};