#include <windows.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/glaux.h>
#include <ctime>
#include <cstdio>
#include "Classes/Headers/FirstPersonCamera.h"
#include <vector>
#include"Bullet.h"
#include"RedCone.h"   // lama bashelhaa betedee errors !! 
#include <sstream>
#include "Classes/Headers/texture.h"
#include "Classes/Headers/md2.h"
#include<iostream>
#include <stdio.h>
#include <irrKlang.h>
#if defined(WIN32)
#include <conio.h>
#else
#include "../common/conio.h"
#endif
using namespace std;
using namespace irrklang;
 
 
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "irrKlang.lib")

/****************************************** Global Variables *********************************************/ 
 
int largc;
char** largs;
int mode = 0 ;
UINT prevFrameTime = 0;
float spotAngle = 0;
float dtSec = 0;
int Camera_Mode1 = 1 ;   //Sniper Mode.
float Angle=0;
float UpdateX=0;
float UpdateZ=0;
float speed=10;
float SniperH=0;
int CamPos=60;          
int done1 , done2 , done3 , done4 , done5,done6 , done7 , done8 , done9 , done10 , done11 , done12  , done13 , done14 , done15 , done16 ;
vector<Bullet> bullets;
float PI=0.017453292519943295769236907684883;
FirstPersonCamera camera;
float beastAngle = 0;
float animTimeSec = 0;
//GLuint texture;
GameManager game;
int y=0;
bool SniperMode = false;
 


/***************************** Methods ********************************/ 

void InitGraphics(int argc, char *argv[]);
void InitEnemies();
void RenderEnemiesCones();
void CheckCollisions();
void SetLights();
void DrawCube();
void DrawSquare();
void Draw3DSolarSystem();
float fRotAroundY;
void DrawSquare(int x,int y);
void RenderPlants();
void resetEnemiesAngle ();
void enemiesScale ();
void OnDisplay();
void Update();
GLuint LoadTexture( const char * filename, int width, int height);
GLuint LoadTextureRAW( const char * filename, int wrap );
unsigned int LoadTexture(LPCWSTR fileName) ; 
GLuint texture1 [6];
GLuint texture;


//------------------------------------------CONTROLS----------------------------------------------//

void SetTransformations() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(50, 800.0/800, 0.25, 1000);
 
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); //CT <- I
        camera.Tellgl( Camera_Mode1 , game.hero.P );
        
}
void OnKeyboard(unsigned char c, int x, int y) {

        if(c == 27)      //esc    <orignal>   // not Sniper Mode.
        {
                Camera_Mode1=1 ; 
        }
        else if (c == 13 )  // enter    < inside  //  Sniper Mode.
        {
                Camera_Mode1=0 ; 
                SniperMode = true;
        }
 
        switch(toupper(c))      
        {
                case ' ':  // SHOOT 
                        {
							PlaySoundA("media/explosion.wav",NULL,SND_ASYNC);	
							                       // here i'am taking the CAM direction to modify the Bullet velocty
                        FirstPersonCamera Tmp;   
                        if ( Camera_Mode1){     
                        Tmp.Reset(UpdateX,5 ,UpdateZ,
                                UpdateX+100*sin(Angle*PI), 5, UpdateZ-100*cos(Angle*PI),
                                0,1,0);
                        }
                        else {                  // for the Sniper Mode.  i will give it the hero position 
                                Tmp.Reset(game.hero.P.x,5 ,game.hero.P.z,
                                UpdateX+100*sin(Angle*PI), 5, UpdateZ-100*cos(Angle*PI),
                                0,1,0);
                        }
                        Vector3 temp = 200 * Tmp.GetLookDirection();     // MSH lee 200 
                        Bullet BB (Tmp.P,Vector3(temp.x , -40  , temp.z));
                        game.heroBullets.push_back(BB);
 
                        }
                break; 
                
                
                //to pause game
        case 'S':
                int ret = MessageBox(NULL, "Game Paused\nTo continue press Yes to exit game press No", "Pause", MB_YESNO);
                if (ret == IDNO)
                        exit(0);
                break;
        
        }
 
} 
void OnSpecial(int key,int x,int y)
{
        float Tz=UpdateZ,Tx=UpdateX,Px,Py,Pz;
        switch(key)
        {
                case GLUT_KEY_LEFT:
                        Angle-=3.6;
                        break;
                case GLUT_KEY_RIGHT:
                        Angle+=3.6;
                        break;
                case GLUT_KEY_UP:
					if (!Camera_Mode1)
						y++;
                        //Tz-=speed*cos(Angle*PI);
                        //Tx+=speed*sin(Angle*PI);
						
                        break;
                case GLUT_KEY_DOWN:
					if (!Camera_Mode1)
						y--;
                        //Tz+=speed*cos(Angle*PI);
                        //Tx-=speed*sin(Angle*PI);
                        break;                          
			                           
        };
        if(Tx<1490 && Tx>-1490)
                UpdateX=Tx;
 
        if(Tz<1490 && Tz>-1490)
                UpdateZ=Tz;
        Px=UpdateX-CamPos*sin(Angle*PI);
        Pz=UpdateZ+CamPos*cos(Angle*PI);
        Py=15;
        camera.Reset(Px,Py+10,Pz,(UpdateX+(1500/Py)*sin(Angle*PI)),0,(UpdateZ-(1500/Py)*cos(Angle*PI)),
                0,1,0);

		game.hero.angle= Angle ;
 
        
}
float dt ; 
void InitGraphics(int argc, char *argv[]) {     
 
        InitEnemies();
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        glutInitWindowPosition(100, 100); //pass (-1, -1) for Window-Manager defaults
        glutInitWindowSize(800, 800);
        glutCreateWindow("OpenGL Lab");
        glutKeyboardFunc(OnKeyboard);
        glutSpecialFunc(OnSpecial);
        glutDisplayFunc(OnDisplay);
 
        glEnable(GL_DEPTH_TEST);
        texture = LoadTextureRAW( "texture.raw", TRUE );  // for the texture
         int x =2;
        texture = LoadTextureRAW( "texture.raw", TRUE );
        texture1[0] = LoadTexture( "skybox/back.bmp", 256*x , 256*x ); //load the texture
        texture1[1] = LoadTexture( "skybox/front.bmp", 256*x, 256 *x); //load the texture
        //texture1[1] = LoadTexture( "skybox/front.bmp", 256*x, 256*x ); //load the texture
        texture1[2] = LoadTexture( "skybox/left.bmp", 256*x, 256 *x); //load the texture
        texture1[3] = LoadTexture( "skybox/right.bmp", 256*x, 256 *x); //load the texture
        texture1[4] = LoadTexture( "skybox/down.bmp", 256*x, 256*x ); //load the texture
        texture1[5] = LoadTexture( "skybox/up.bmp", 256*x, 256*x ); //load the texture
        
        //Reset the hero position, hero size, number of enemies, minimum position for enemies and maximum position for enemies
        game.Reset(Vector3(0,-50,-50) , 10, 0, Vector3(-50, 0, -30), Vector3(50, 0, -150), Angle );
        glutMainLoop();
}
void OnDisplay() {
        
        glClearColor(0.94901, 0.9215, 0.81568, 1);		
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        SetTransformations();
        //drawing code geos here 
 		
		Draw3DSolarSystem();  // DRAW THE SKY BOX 
         
		RenderEnemiesCones();  // draw the enemies 
        game.Render(); // draw the bullets  
		RenderPlants();  // DRAW THE PLANTS 


		if(!Camera_Mode1){
		glPushMatrix();
	{ 

		glLoadIdentity();
		glColor3f(0,0,0);
		glTranslatef(0,  (60-0.5*CamPos)/60+y  , -10);
		glScaled(0.1,0.1,0.1);
		glutSolidTorus(5,20 , 20, 20) ;  // da 8OOO9 :D
		glLoadIdentity();
		
	}
	glPopMatrix();
	glPushMatrix();
	{
		glLoadIdentity();
		glColor3f(1,1,1);
		glTranslatef(0,(60-0.5*CamPos)/60,-10);
		glBegin(GL_LINES);
			glVertex3f(-0.5f, 0, 0);
			glVertex3f(0.5f, 0, 0);
			glVertex3f(0, 0.5f, 0);
			glVertex3f(0, -0.5f, 0);
		glEnd();
	}
	glPopMatrix();
		}
        glFlush();
        glutSwapBuffers();
        Update();
        glutPostRedisplay();
}
int main(int argc, char* argv[]) {
	ISoundEngine* engine = createIrrKlangDevice();

 if (!engine)
 {
  printf("Could not startup engine\n");
  return 0; // error starting up the engine
 }//C:\Users\HP\Desktop\Final modifikation -SKG_COPY - 11 PM - 23-5\media
     engine->play2D("media/getout.ogg", true);
     largs = argv;
        largc = argc;
        int ret;
        ret = MessageBox(NULL, "Hello!!!\n Are you Ready to start the game :D ??", "Air Fight", MB_YESNO);
        if (ret == IDNO)
                return 0;
        timeBeginPeriod(1);
        InitGraphics(argc, argv);
        timeEndPeriod(1);
        return 0;
}
 
//-------------------------------------THE SCENE---'SKY BOX'--------------------------------------//


GLuint LoadTexture( const char * filename, int width, int height) {
    GLuint texture;
    unsigned char * data;
    FILE* file;
 
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    fclose( file );
 
    glGenTextures( 1, &texture ); 
    glBindTexture( GL_TEXTURE_2D, texture ); 
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
 
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
 
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
 
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );    
    free(data);
    return texture; 
}  
GLuint LoadTextureRAW( const char * filename, int wrap )
{
  GLuint texture;
  int width, height;
  BYTE * data;
  FILE * file;
 
  // open texture data
  file = fopen( filename, "rb" );
  if ( file == NULL ) return 0;
 
  // allocate buffer
  width = 256;
  height = 256;
  data = (BYTE*)malloc( width * height * 3 );
 
  // read texture data
  fread( data, width * height * 3, 1, file );
  fclose( file );
 
  // allocate a texture name
  glGenTextures( 1, &texture );
 
  // select our current texture
  glBindTexture( GL_TEXTURE_2D, texture );
  
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,
    height, GL_RGB, GL_UNSIGNED_BYTE, data );
 
  // free buffer
  free( data );
 
  return texture;
 
}
unsigned int LoadTexture(LPCWSTR fileName) { 
        unsigned int texId;
        AUX_RGBImageRec *pBitmap = NULL;
 
        pBitmap = auxDIBImageLoadW(fileName);
        
        if (pBitmap == NULL)
        {
                // No image found, or bad format.
                throw exception("Cannot load the specified image");
        }
        //get an id
        glGenTextures(1, &texId);
         //select/allocate&select the texture object with id
   glBindTexture(GL_TEXTURE_2D, texId);
        //load the bitmap into video memory
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
                //MinFIlter <-- LInear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);           // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);           // Linear Filtering
 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       // Clamping Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);       // Clamping Parameters
 
        // C++ does not manage the memory. You have to manage it yourself.
        if (pBitmap)                                                                            // If we loaded the bitmap
        {
                if (pBitmap->data)                                                              // If there is texture data
                        free(pBitmap->data);                                            // Free the texture data, we don't need it anymore
                free(pBitmap);                                                                  // Free the bitmap structure
        }
        
        return texId;
}
void DrawCube()
{
	//front face  z=0.5;
	glPushMatrix();
	
	glTranslatef(0,0,0.5f);
	glRotatef(-180,1,0,0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D,texture1[0]);	
	DrawSquare(1,1);
	glPopMatrix();


	//back face
	glBindTexture(GL_TEXTURE_2D,texture1[0]);
	glPushMatrix();
	glTranslatef(0,0,-0.5f);
	glRotatef(180,0,1,0);
	DrawSquare(1,1);
	glPopMatrix();

	//right face: x=+0.5
	glBindTexture(GL_TEXTURE_2D,texture1[0]);
	glPushMatrix();
	glTranslatef(0.5f,0,0);
	glRotatef(90,0,1,0);
	DrawSquare(1,1);
	glPopMatrix();


	//left face: X=-0.5
	glPushMatrix();
	glTranslatef(-0.5f,0,0);
	glRotatef(90,0,1,0);
	DrawSquare(1,1);
	glPopMatrix();

	//up face y=-0,5
	glBindTexture(GL_TEXTURE_2D,texture1[0]);
	glPushMatrix();
	glTranslatef(0,-0.5f,0);
	glRotatef(90,1,0,0);
	DrawSquare(1,1);
	glPopMatrix();
	

}
void DrawSquare(int x,int y) {
	//draw a simple quad
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);

	glTexCoord2f(0,0);
		glVertex3f(-0.5f, -0.5f, 0);

		
glTexCoord2f(x,0);
	
		glVertex3f(0.5f, -0.5f, 0);

		
glTexCoord2f(x,y);
	
		glVertex3f(0.5f, 0.5f, 0);

		
glTexCoord2f(0,y);
	
		glVertex3f(-0.5f, 0.5f, 0);
	glEnd();
}
void DrawSquare(){
	// draw a simple quad
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
		glVertex3f(-0.5f,-0.5f,0);
		glTexCoord2f(0,1);
		glVertex3f(0.5f,-0.5f,0);
		glTexCoord2f(1,0);
		glVertex3f(0.5f,0.5f,0);
		glTexCoord2f(1,1);
		glVertex3f(-0,0.5f,0);

		glEnd();
}
void Draw3DSolarSystem(){
	glPushMatrix();
	
	glEnable(GL_TEXTURE);
		glTranslatef(0,-5,0);
		glScalef(10000,1000,10000);
		glTranslatef(0,0.6,0);
		 DrawCube();
	glPopMatrix();
}


//-------------------------------------THE GAME CONTROL ------------------------------------------//


int getLevel(int mode)
{
        //level 1
        if (mode==0 )
                return 1;
        //level 2
        if (mode== 1)
                return 2;
    
}
void Update()
{
        UINT currTime = timeGetTime();
        //the first update?
        if(prevFrameTime == 0) {
                prevFrameTime = currTime;
                return;
        }
        
 
        dt = (currTime - prevFrameTime)/1000.0;
        // helic
        game.h.Update();
        game.h.animTimeSec += 2000 * dt;
 
        game.h2.Update();
        game.h2.animTimeSec += 2000 * dt;
 
        game.h3.Update();
        game.h3.animTimeSec += 2000 * dt;
 
        //game.bb.Update();
        game.bb.animTimeSec += 2000 * dt;
        
        game.buggy1.Update4();
        game.buggy2.Update3();
        game.buggy1.animTimeSec += 2000 * dt;
        game.buggy2.animTimeSec += 2000 * dt;

		game.p1.animTimeSec += 2000 * dt;
		game.p2.animTimeSec += 2000 * dt;
		game.p3.animTimeSec += 2000 * dt;
		game.p4.animTimeSec += 2000 * dt;

		// to make the plants rotate around y 
		game.p1.angle+=0.03;
		game.p2.angle+=0.03;
		game.p3.angle+=0.03;
		game.p4.angle+=0.03;
 
 
        if (mode == 1){   // For the moving Enemies " ONLY Second level " 
                if (!done1)
                        game.Enemy.Update2();
                if (!done2)
                        game.Enemy2.Update();
                if (!done3)
                        game.Enemy3.Update4();
                if (!done4)
                        game.Enemy4.Update();
                if (!done5)
                        game.Enemy5.Update3();
                if (!done6)
                        game.Enemy6.Update3();
                if (!done7)
                        game.Enemy7.Update2();
                if (!done8)
                        game.Enemy8.Update();
                if (!done9)
                        game.Enemy9.Update4();
                if (!done10)
                        game.Enemy10.Update4();
                if (!done11)
                        game.Enemy11.Update4();
                if (!done12)
                        game.Enemy12.Update2();
                if (!done13)
                        game.Enemy13.Update();
                if (!done14)
                        game.Enemy14.Update();
                if (!done15)
                        game.Enemy15.Update3();
                if (!done16)
                        game.Enemy16.Update2();
 
 
        }
 
 
 
        //Write all update code here.   
        prevFrameTime = currTime;
        CheckCollisions();
        game.HandleCollisions();
        game.Update(dt);
 
        spotAngle += 2;
        game.hero.animTimeSec += 120 * dt;
        
        //game.Enemy.animTimeSec += 120 * dt;
 
        //Display health, score, fps
        char titleStr[512];
        sprintf(titleStr, "Health: %d, Score: %d, Level : %d, fps: %f", game.heroHealth, game.heroScore, getLevel(mode), 1/dt);
        glutSetWindowTitle(titleStr);
}
void InitEnemies(){
 
        game.h.P=Vector3(-20 , 20 , 70);  // the heli 
        game.h2.P=Vector3(50 , 20 , -80);  // the heli 
        game.h3.P=Vector3(-50 , 20 , -80);  // the heli 

        game.bb.P=Vector3(70 , -50 , 60);
 
        game.buggy1.P=Vector3(50 , -50 , 100);
        game.buggy2.P=Vector3(-100 , -50 , -30);
 
        if (mode == 0 ){

        game.Enemy.P=Vector3(-110,-50,140);		
        game.Enemy2.P=Vector3(-150,-50,60);		
        game.Enemy3.P=Vector3(170,-50,60);
        game.Enemy4.P=Vector3(-110,-50,-150);		
        game.Enemy5.P=Vector3(200,-50,140);		  
        game.Enemy6.P=Vector3(-230,-50,169);		
        game.Enemy7.P=Vector3(-250,-50, 140);		
        game.Enemy8.P=Vector3(-200,-50,-130);		
        game.Enemy9.P=Vector3(100,-50,-150);		
        game.Enemy10.P=Vector3(130,-50,-100);
        game.Enemy11.P=Vector3(150,-50,-94);
        game.Enemy12.P=Vector3(-250,-50,-180);		 
        game.Enemy13.P=Vector3(-20,-50,-110);
        game.Enemy14.P=Vector3(220,-50,-177);		
        game.Enemy15.P=Vector3(110,-50,80);
        game.Enemy16.P=Vector3(-40,-50,140);

        enemiesScale();
        }
                if (mode == 1 ){
        game.h.P=Vector3(-50 , 20 , 80);  // the heli 
        game.h2.P=Vector3(50 , 20 , -80);  // the heli 
        game.h3.P=Vector3(-50 , 20 , -80);  // the heli  
		resetEnemiesAngle();
        
        }
        
}
void enemiesScale ()
{
	game.Enemy.scl=2.5; 
	game.Enemy2.scl=2.9;
	game.Enemy4.scl=2.5; 
	game.Enemy5.scl=3;
	game.Enemy6.scl=3;   // to far then we have to scale it 
	game.Enemy7.scl=3;   // to far then we have to scale it 
	game.Enemy8.scl=3.6;
	game.Enemy9.scl=2.6;
	game.Enemy10.scl=3.7;
	game.Enemy11.scl=2.9;
	game.Enemy12.scl=3.5;
	game.Enemy14.scl=4;
}
void resetEnemiesAngle ()
{  

	if ( done1)
	    game.Enemy.angle+=-90;
	if ( done2)
        game.Enemy2.angle+=-90;
	if ( done3)
        game.Enemy3.angle+=-90;
	if ( done4)
        game.Enemy4.angle+=-90;
	if ( done5)
        game.Enemy5.angle+=-90;
	if ( done6)
        game.Enemy6.angle+=-90;
	if ( done7)
        game.Enemy7.angle+=-90;
	if ( done8)
        game.Enemy8.angle+=-90;
	if ( done9)
        game.Enemy9.angle+=-90;
	if ( done10)
        game.Enemy10.angle+=-90;
	if ( done11)
        game.Enemy11.angle+=-90;
	if ( done12)
        game.Enemy12.angle+=-90;
	if ( done13)
        game.Enemy13.angle+=-90;
	if ( done14)
        game.Enemy14.angle+=-90;
	if ( done15)
        game.Enemy15.angle+=-90;
	if ( done16)
        game.Enemy16.angle+=-90;
}
void RenderEnemiesCones()
{

        game.Enemy.Render();
        game.Enemy2.Render();
        game.Enemy3.Render();
        game.Enemy4.Render();
        game.Enemy5.Render();
        game.Enemy6.Render();
        game.Enemy7.Render();
        game.Enemy8.Render();
        game.Enemy9.Render();
        game.Enemy10.Render();
        game.Enemy11.Render();
        game.Enemy12.Render();
        game.Enemy13.Render();
        game.Enemy14.Render();
        game.Enemy15.Render();
        game.Enemy16.Render();
        

		game.h.Render();
        game.h2.Render();
        game.h3.Render();
 
        game.bb.Render();
 
        game.buggy1.Render();
        game.buggy2.Render();
 
 
 
 
 
}
void InitPlants()
{
	game.p1.P = Vector3(-100, -50, -120);
	game.p2.P = Vector3(70, -50, 10);
	game.p3.P = Vector3(40, -50, -20);
	game.p4.P = Vector3(80, -50, 45);
}
void RenderPlants()
{
	game.p1.Render();
	game.p2.Render();
	game.p3.Render();
	game.p4.Render();
}
void CheckCollisions()
{
        
        AABoundingBox enemBox;
        size_t nBulls ;
        enemBox = game.Enemy.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                //              game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy.angle=90;
                                game.heroScore += 100;   // yet3'ayar 
                                done1=1;
                                break;
								game.heroBullets[bullInd].GetAABB().Render();
								
                        }
                }
                
 
                enemBox = game.Enemy2.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                        //      game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy2.angle=90;
                                game.heroScore += 100;
                                done2=1;
                                break;
                        }
                }
                                 enemBox = game.Enemy3.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy3.angle=90;
                                game.heroScore += 100;
                                done3=1;
                                break;
                        }
                }
                                 enemBox = game.Enemy4.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy4.angle=90;
                                game.heroScore += 100;
                                done4=1;
                                break;
                        }
                }
                                 enemBox = game.Enemy5.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy5.angle=90;
                                game.heroScore += 100;
                                done5=1;
                                break;
                        }
                }
                                 enemBox = game.Enemy6.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy6.angle=90;
                                done6=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
                 enemBox = game.Enemy7.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                        //      game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy7.angle=90;
                                done7=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
                 enemBox = game.Enemy8.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy8.angle=90;
                                done8=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
 
         enemBox = game.Enemy9.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy9.angle=90;
                                done9=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
 
                 enemBox = game.Enemy10.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy10.angle=90;
                                done10=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
 
 
                 enemBox = game.Enemy11.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy11.angle=90;
                                done11=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
                 enemBox = game.Enemy12.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                                //game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy12.angle=90;
                                done12=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
 
                 enemBox = game.Enemy13.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                        //      game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy13.angle=90;
                                done13=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
 
                 enemBox = game.Enemy14.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                        //      game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy14.angle=90;
                                done14=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
 
                 enemBox = game.Enemy15.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                        //      game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy15.angle=90;
                                done15=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
 
                 enemBox = game.Enemy16.GetAABB();
                 nBulls = game.heroBullets.size();
                for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
                        if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
                //              game.heroBullets.erase(game.heroBullets.begin() + bullInd);
                                game.Enemy16.angle=90;
                                done16=1;
                                game.heroScore += 100;
                                break;
                        }
                }
 
                if (!done1)                     
                  game.Enemy.animTimeSec += 120 * dt;
                if (!done2)                     
                  game.Enemy2.animTimeSec += 120 * dt;
                if (!done3)                     
                  game.Enemy3.animTimeSec += 120 * dt;
                if (!done4)                     
                  game.Enemy4.animTimeSec += 120 * dt;
                if (!done5)                     
                  game.Enemy5.animTimeSec += 120 * dt;
                if (!done6)                     
                  game.Enemy6.animTimeSec += 120 * dt;
 
                if (!done7)                     
                  game.Enemy7.animTimeSec += 120 * dt;
                if (!done8)                     
                  game.Enemy8.animTimeSec += 120 * dt;
                if (!done9)                     
                  game.Enemy9.animTimeSec += 120 * dt;
                if (!done10)                    
                  game.Enemy10.animTimeSec += 120 * dt;
                if (!done11)                    
                  game.Enemy11.animTimeSec += 120 * dt;
                if (!done12)                    
                  game.Enemy12.animTimeSec += 120 * dt;
                if (!done13)                    
                  game.Enemy13.animTimeSec += 120 * dt;
                if (!done14)                    
                  game.Enemy14.animTimeSec += 120 * dt;
                if (!done15)                    
                  game.Enemy15.animTimeSec += 120 * dt;
                if (!done16)                    
                  game.Enemy16.animTimeSec += 120 * dt;
 
 
                  int CurrentState = done1 + done2 + done3 + done4 + done5 + done6 +done7 + done8 + done9 + done10 + done11 + done12 + done13 + done14 + done15 + done16; 
                  if (CurrentState ==10 & mode== 0 ){
                        mode++;
						InitEnemies();
						game.Enemy.setAni(1);
						game.Enemy2.setAni(1);
						game.Enemy3.setAni(1);
						game.Enemy4.setAni(1);
						game.Enemy5.setAni(1);
						game.Enemy6.setAni(1);


                        done1 = done2 = done3 = done4 = done5 = done6=done7 = done8 = done9 = done10 = done11 = done12 = done13 = done14 = done15 = done16=0; 
                       // InitEnemies();
                }
 
                
}