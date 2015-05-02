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
 
 
class buggy {
public:
    Vector3 P , V;//Pposition of the buggy .. V is the velocity of the buggy
        int teapotSize;
        CMD2Model buggyBody; // MD2 object to load the buggy model 
        float animTimeSec;
        float angle ;//current Angle of the model (used in rotation)
        Vector3 Velo;
        int MovesCounter  ; //counter to keep track of buggy's moves to control it's moves
        int yes ; //flag (takes values 0 or 1) if MovesCounter = 0 -> yes = 1 else yes = 0 (used in update function)
        buggy(const Vector3 pos, int size , Vector3 V) {
                yes=1 ; 
                MovesCounter=0 ;
                Velo=V;
                P = pos;
                teapotSize = size;
                animTimeSec = 0;
                CTextureManager::GetInstance()->Initialize();
                buggyBody.LoadModel("models/buggy/BUGGY.md2");
                buggyBody.LoadSkin("models/buggy/buggy.bmp");
                //plane.LoadSkin("C:\\Users\\Public\\Pictures\\Sample Pictures\\Penguins.bmp");
                buggyBody.SetAnim(0);
 
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
        
                glScalef(1, 0.7, 1);
                glTranslatef(6*saveP.x, 4*saveP.y, 6*saveP.z);
        glRotated ( angle , 0 , 0 , 1  );
            glTranslatef (0,0,0);
                                glScalef(3, 3, 3);
                  //  glTranslatef(6*P.x, 4*P.y, 6*P.z);
                
 
                        
                        //glutSolidTeapot(teapotSize);
                        glEnable(GL_TEXTURE_2D);
                        buggyBody.DrawModel(animTimeSec);
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
 
         
                void Update() {
                        if ( MovesCounter == 0 ) 
                                yes= 1 ;
                        else if (MovesCounter == 400 ) 
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
                        else if (MovesCounter == 400 ) 
                                yes= 0 ; 
 
                        if ( yes) 
                        {
                                P.z = P.z-0.05;
                            P.x = P.x+0.05;
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
                        else if (MovesCounter == 400 ) 
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
                        else if (MovesCounter == 400 ) 
                                yes= 0 ; 
 
 
                        if ( yes ) 
                        {
                                P.z = P.z+0.05;
                            P.x = P.x-0.05;
                                MovesCounter++;
 
                        }
                        else 
                        {
                                P.z = P.z-0.05;
                            P.x = P.x+0.05;
                                MovesCounter--;
                        }
                        
                }
 
 
                
 
};