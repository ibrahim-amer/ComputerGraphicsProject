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
	float angle ;
    HeroTeapot(const Vector3 &pos, int size) {
		P = pos;
		teapotSize = size;
		animTimeSec = 0;
		CTextureManager::GetInstance()->Initialize();
		plane.LoadModel("models/airplane/body.md2");
		plane.LoadSkin("models/airplane/air3_b.pcx");
		plane.SetAnim(0);

		wings.LoadModel("models/airplane/wing.md2");
		wings.LoadSkin("models/airplane/air3_b.pcx");
		wings.SetAnim(0);
		
	}

	AABoundingBox GetAABB() const {
		Vector3 dims(.15, .15, .15);
		return AABoundingBox(P, 2*dims);
	}


	/*void Rotat(float angleDegrees)
	{
		Vector3 jDir(0, 0, 1);


		P.RotateAround2(angleDegrees, jDir);
		

	}*/

	void Render()  {
		
		glPushMatrix();
			//glColor3f(0, 0, 1);
		Vector3 saveP = P ; 
	
		glScalef(.15, .15, .15);
		    glTranslatef(6*saveP.x, 6*saveP.y, 6*saveP.z);
        	glRotated ( angle , 0 , 0 , 1  );
	        glTranslatef (0,0,0);
			
			
			//glutSolidTeapot(teapotSize);
			glEnable(GL_TEXTURE_2D);
			plane.DrawModel(animTimeSec);
			wings.DrawModel(animTimeSec);
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