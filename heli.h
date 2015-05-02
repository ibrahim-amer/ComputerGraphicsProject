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
 
 
class heli {
public:
    Vector3 P , V;
        int teapotSize;
        CMD2Model heliBody;
        float animTimeSec;
        float angle ;
        Vector3 Velo;
        heli(const Vector3 &pos, int size , Vector3 V) {
                Velo=V;
                P = pos;
                teapotSize = size;
                animTimeSec = 0;
                CTextureManager::GetInstance()->Initialize();
                heliBody.LoadModel("models/heli/helicopter.md2");
                heliBody.LoadSkin("models/heli/helicopter1.bmp");
                //plane.LoadSkin("C:\\Users\\Public\\Pictures\\Sample Pictures\\Penguins.bmp");
                heliBody.SetAnim(0);
 
/*
                wings.LoadModel("models/airplane/wing.md2");
                wings.LoadSkin("models/airplane/air3_b.pcx");
                wings.SetAnim(0)*/;
                
        }
 
        AABoundingBox GetAABB() const {
                Vector3 dims(3, 3, 3);        // :@ msh radta tozbot 
                return AABoundingBox(P, 2*dims);
        }
         
        void Render()  {
                
                glPushMatrix();
                glColor3f(0, 1, 0);
                Vector3 saveP = P ; 
        
                glScalef(1, 0.7, 1);
                glTranslatef(6*saveP.x, 4*saveP.y, 6*saveP.z);
        glRotated ( angle , 0 , 0 , 1  );
            glTranslatef (0,0,0);
                //              glScalef(.15, .15, .15);
                  //  glTranslatef(6*P.x, 4*P.y, 6*P.z);
                
 
                        
                        //glutSolidTeapot(teapotSize);
                        glEnable(GL_TEXTURE_2D);
                        heliBody.DrawModel(animTimeSec);
                        //wings.DrawModel(animTimeSec);
                        glDisable(GL_TEXTURE_2D);
                glPopMatrix();
        }
 
 
         // These updates methods Only for the second level , Enemies will move in spacefic direction , 
         // Until a spacefic time , and then convert the direction 
                void Update() {
                                P.z = P.z-0.01;
                            P.x = P.x-0.01;
                }
 
 
                
 
};