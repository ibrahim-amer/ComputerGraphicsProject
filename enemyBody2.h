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
 
 
class enemyBody2 {
public:
    int scl  ;
        Vector3 P , V;
        int teapotSize;
        CMD2Model enemyBody;
        CMD2Model wings;
        float animTimeSec;
        float angle ;
        Vector3 Velo;
        int MovesCounter  ; 
        int yes ; 
		int Ani ;//= 0 ; 
        enemyBody2(const Vector3 &pos, int size , Vector3 V) {
                yes=1 ; 
                MovesCounter=0 ;
                Velo=V;
                P = pos;
                scl=2;
                teapotSize = size;
                animTimeSec = 0;
                CTextureManager::GetInstance()->Initialize();
                enemyBody.LoadModel("models/terran/terran.md2");
              enemyBody.LoadSkin("models/terran/terran.pcx");
     //           enemyBody.LoadSkin("C:/Users\\Public\\Pictures\\Sample Pictures\\Penguins.bmp");

                enemyBody.SetAnim(Ani);

 
/*
                wings.LoadModel("models/airplane/wing.md2");
                wings.LoadSkin("models/airplane/air3_b.pcx");
                wings.SetAnim(0)*/;
                
        }
		void setAni(int a )
		{
			 enemyBody.SetAnim(a); 

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
        
                glScalef(0.5, 0.5, 0.5);
                glTranslatef(6*saveP.x, 4*saveP.y, 6*saveP.z);
        glRotated ( angle , 0 , 0 , 1  );
            glTranslatef (0,0,0);
                glScalef(scl, scl, scl);
                //              glScalef(.15, .15, .15);
                  //  glTranslatef(6*P.x, 4*P.y, 6*P.z);
                
 
                        
                        //glutSolidTeapot(teapotSize);
                        glEnable(GL_TEXTURE_2D);
                        enemyBody.DrawModel(animTimeSec/70);
                        //wings.DrawModel(animTimeSec);
                        glDisable(GL_TEXTURE_2D);
                glPopMatrix();
        }
 
 
         // These updates methods Only for the second level , Enemies will move in spacefic direction , 
         // Until a spacefic time , and then convert the direction 
                void Update() {
                    if ( MovesCounter == 0 ) 
                            yes= 1 ;
                    else if (MovesCounter == 400 ) 
                            yes= 0 ; 
 
 
                    if (yes ) 
                    {
                            P.z = P.z+0.05;
                        P.x = P.x+0.08;
                            MovesCounter++;
 
                    }
                    else 
                    {
                            P.z = P.z-0.05;
                        P.x = P.x-0.08;
                            MovesCounter--;
                                
                    }
 
            }
 
 
            void Update2() 
                {
                        
                                if ( MovesCounter == 0 ) 
                                                yes= 1 ;
                                else if (MovesCounter == 400 ) 
                                                yes= 0 ; 
 
                                if ( yes) 
                                {
                                                P.z = P.z-0.08;
                                        P.x = P.x+0.05;
                                                MovesCounter++;
 
                                }
                                else 
                                {
                                                P.z = P.z+0.08;
                                        P.x = P.x-0.05;
                                                MovesCounter--;
                                }
 
            }
 
                void Update3() {
 
                        if ( MovesCounter == 0 ) 
                                yes= 1 ;
                        else if (MovesCounter == 400 ) 
                                yes= 0 ; 
 
                        if ( yes ) 
                        {
                                P.z = P.z-0.08;
                            P.x = P.x-0.05;
                                MovesCounter++;
                        }
                        else 
                        {
                                P.z = P.z+0.08;
                            P.x = P.x+0.05;
                                MovesCounter--;
                        }
                }
 
 
                void Update4() {
 
                        if ( MovesCounter == 0 ) 
                                yes= 1 ;
                        else if (MovesCounter == 400 ) 
                                yes= 0 ; 
 
 
                        if ( yes ) 
                        {
                                P.z = P.z+0.08;
                            P.x = P.x-0.08;
                                MovesCounter++;
 
                        }
                        else 
                        {
                                P.z = P.z-0.08;
                            P.x = P.x+0.08;
                                MovesCounter--;
                        }
                        
                }
 
};