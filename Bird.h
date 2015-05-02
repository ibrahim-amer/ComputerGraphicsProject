#pragma once
#include"Vector3.h"
#include"AABoundingBox.h"
#include"GameManager.h"
#include "Vector3.h"
#include "AABoundingBox.h"
#include "Bullet.h"
#include "gl/glut.h"
#include "Classes/Headers/texture.h"
#include "Classes/Headers/md2.h"
#include "Classes/Headers/pcx.h"


class Bird {
public:
	Vector3 P , V;
	int teapotSize;
	CMD2Model plane;
	CMD2Model wings;
	float animTimeSec;
	float angle ;
	Vector3 Velo;
	int MovesCounter  ; 
	int yes ; 
	Bird(const Vector3 &pos, int size , Vector3 V) {
		yes=1 ; 
		MovesCounter=0 ;
		Velo=V;
		P = pos;
		teapotSize = size;
		animTimeSec = 0;
		CTextureManager::GetInstance()->Initialize();
		plane.LoadModel("models/heli/helicopter.md2");
		plane.LoadSkin("models/heli/helicopter1.bmp");
	//	plane.LoadSkin("C:\\Users\\Public\\Pictures\\Sample Pictures\\Penguins.bmp");
		plane.SetAnim(0);

/*
		wings.LoadModel("models/airplane/wing.md2");
		wings.LoadSkin("models/airplane/air3_b.pcx");
		wings.SetAnim(0)*/;
		
	}

	AABoundingBox GetAABB() const {
		Vector3 dims(3, 3, 3);        // :@ msh radta tozbot 
		return AABoundingBox(P, 2*dims);
	}


	/*void Rotat(float angleDegrees)
	{
		Vector3 jDir(0, 0, 1);


		P.RotateAround2(angleDegrees, jDir);
		

	}*/

	void Render()  {
		
		glPushMatrix();
		glColor3f(0, 1, 0);
		Vector3 saveP = P ; 
	
		//glScalef(0.0001, 0.0001, 0.0001);
		//glTranslatef (40,100,20);
		glTranslatef(6*saveP.x, 4*saveP.y, 6*saveP.z);
        glRotated ( angle , 0 , 0 , 1  );
	    glTranslatef (40,50,20);
		//		glScalef(.15, .15, .15);
		  //  glTranslatef(6*P.x, 4*P.y, 6*P.z);
        	

			
			//glutSolidTeapot(teapotSize);
			glEnable(GL_TEXTURE_2D);
			plane.DrawModel(animTimeSec/70);
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

	 // These updates methods Only for the second level , Enemies will move in spacefic direction , 
	 // Until a spacefic time , and then convert the direction 
		void Update() {
			if ( MovesCounter == 0 ) 
				yes= 1 ;
			else if (MovesCounter == 100 ) 
				yes= 0 ; 


			if (yes ) 
			{
				P.z = P.z+0.05;
			    P.x = P.x+0.05;
				MovesCounter++;

			}
			else 
			{
				P.z = P.z-0.05;
			    P.x = P.x-0.05;
				MovesCounter--;
				
			}

		}


		void Update2() {
			
			if ( MovesCounter == 0 ) 
				yes= 1 ;
			else if (MovesCounter == 100 ) 
				yes= 0 ; 

			if ( yes) 
			{
				P.z = P.z-2;
			    P.x = P.x+2;
				MovesCounter++;

			}
			else 
			{
				P.z = P.z+0.05;
			    P.x = P.x-0.05;
				MovesCounter--;
			}

		}

		void Update3() {

			if ( MovesCounter == 0 ) 
				yes= 1 ;
			else if (MovesCounter == 100 ) 
				yes= 0 ; 

			if ( yes ) 
			{
				P.z = P.z-0.05;
			    P.x = P.x-0.05;
				MovesCounter++;
			}
			else 
			{
				P.z = P.z+0.05;
			    P.x = P.x+0.05;
				MovesCounter--;
			}
		}


		void Update4() {

			if ( MovesCounter == 0 ) 
				yes= 1 ;
			else if (MovesCounter == 100 ) 
				yes= 0 ; 


			//if ( yes ) 
			//{
				P.z = P.z+0.1;
			    P.x = P.x-0.1;
				MovesCounter++;

			//}
			//else 
			//{
			//	P.z = P.z-0.05;
			//    P.x = P.x+0.05;
			//	MovesCounter--;
			////}
			
		}

};