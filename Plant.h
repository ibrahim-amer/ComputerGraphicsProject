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
 
 
class Plant {
public:
    Vector3 P , V;
    CMD2Model plant;
    float animTimeSec;
   float angle;
   int scl ;
        Plant(const Vector3 &pos, int size , Vector3 V) {
                P = pos;
                animTimeSec = 0;
				scl=3;
                CTextureManager::GetInstance()->Initialize();
                plant.LoadModel("modelpack9/plant2/plant2.md2");
                plant.SetAnim(0);
        }
 
        
 
        void Render()  {
                
                glPushMatrix();
                glColor3f(0, 1, 0);
                Vector3 saveP = P ; 
        
                glScalef(1, 0.7, 1);
                glTranslatef(6*saveP.x, 4*saveP.y, 6*saveP.z);
                glRotated ( angle , 0 , 1 , 0  );
	            glTranslatef (0,0,0);
		        glScalef(scl, scl, scl);
                glEnable(GL_TEXTURE_2D);
                plant.DrawModel(animTimeSec);
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();
        }
 
};