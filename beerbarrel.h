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
 
 
class beerbarrel {
public:
    Vector3 P , V;
        int teapotSize;
        CMD2Model barrel;
        float animTimeSec;
        float angle ;
        Vector3 Velo;
        int MovesCounter  ; 
        int yes ; 
        beerbarrel(const Vector3 &pos, int size , Vector3 V) {
                yes=1 ; 
                MovesCounter=0 ;
                Velo=V;
                P = pos;
                teapotSize = size;
                animTimeSec = 0;
                CTextureManager::GetInstance()->Initialize();
                barrel.LoadModel("models/beerbarrel/bar1.md2");
        barrel.LoadSkin("models/beerbarrel/bar1.bmp");
        //      plane.LoadSkin("C:\\Users\\Public\\Pictures\\Sample Pictures\\Penguins.bmp");
                barrel.SetAnim(0);
 
/*
                wings.LoadModel("models/airplane/wing.md2");
                wings.LoadSkin("models/airplane/air3_b.pcx");
                wings.SetAnim(0)*/;
                
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
                //              glScalef(.15, .15, .15);
                  //  glTranslatef(6*P.x, 4*P.y, 6*P.z);
                
 
                        
                        //glutSolidTeapot(teapotSize);
                        glEnable(GL_TEXTURE_2D);
                        barrel.DrawModel(animTimeSec/70);
                        //wings.DrawModel(animTimeSec);
                        glDisable(GL_TEXTURE_2D);
                glPopMatrix();
        }
 
         // These updates methods Only for the second level , Enemies will move in spacefic direction , 
         // Until a spacefic time , and then convert the direction 
 
};