#include <windows.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/glaux.h>
#include <ctime>
#include <cstdio>
#include "Classes/Headers/FirstPersonCamera.h"
#include <vector>
#include"Bullet.h"
#include"RedCone.h"
#include <sstream>
#include "Classes/Headers/texture.h"
#include "Classes/Headers/md2.h"
#include<iostream>
using namespace std;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "glut32.lib")
//#pragma comment(lib, "SDL.lib")
//#pragma comment(lib, "SDLmain.lib")
/*** Global Variables ***/
int largc;
char** largs;
int mode = 0 ;
UINT prevFrameTime = 0;
float spotAngle = 0;
float dtSec = 0;
int Camera_Mode1 = 0 ; 
int Camera_Mode2 = 0 ; 
int Camera_Mode3 = 0 ; 

FirstPersonCamera camera;

float beastAngle = 0;
float animTimeSec = 0;
int op=0;
int OOOP = 0 ; 

//FirstPersonCamera camera;
GameManager game;
vector<HealthPacket> greenBullets;
//ISoundEngine* engine;
string toString(int n)
{
	string str;
	stringstream ss;
	ss << n;
	ss >> str;
	return str;
}

void AdvancedBulletsWithRedCones();
vector<RedCone> redCones;


int getLevel(int mode)
{
	//level 1
	if (mode >= 0 && mode <= 7)
		return 1;
	//level 2
	if (mode >=8 && mode <= 10)
		return 2;
	//level 3
	if (mode >= 11 && mode <= 12)
		return 3;
	if (mode >= 13 )
		return 4;
}
void InitGraphics(int argc, char *argv[]);
void InitGreenBullets();
void RenderGreenBullets();
void UpdateGreenBullets(float);
void InitRedCones();
void RenderRedCones();
void UpdateRedCones(float);
void greenBulletsCheck();
void RedConesCheck();
void CheckCollisions();
void SetLights();
void RenderScene();
float Currentangle ;
void DrawCube();
void DrawSquare();
float fRotAroundY;
GLuint texture;



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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		// Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// Linear Filtering

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       // Clamping Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);       // Clamping Parameters

	// C++ does not manage the memory. You have to manage it yourself.
	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		free(pBitmap);									// Free the bitmap structure
	}
	
	return texId;
}


void SetLights()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

static GLfloat SpecularLight[] = {1.0, 1.0, 1.0};
static GLfloat DiffuseLight[] = {1.0, 1.0, 1.0}; 
static GLfloat AmbientLight[] = {0.2, 0.2, 0.2};
static GLfloat LightPosition[] = {0.0, 50.0, 0.0, 0.0}; 
glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);
}

void SetMaterial()
{
	float mat0_diffuse[4] =  { 1, 1, 1, 1 };

	float mat0_ambient[4] =  { 1, 1, 1, 1 };

	float mat0_specular[4] =  { 0.1, 0.1, .1, .1 };

	float mat0_shininess = 10;
	float mat0_emission[4] =  { 0, 0, 0, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0_ambient);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat0_shininess);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat0_emission);
}

void SetTransformations() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 800.0/800, 0.25, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //CT <- I
	camera.Tellgl(game.hero.P , Camera_Mode1 , Camera_Mode2 , Camera_Mode3 );
	
}

float Yawangle = 0, Rollangle = 0, Pitchangle = 0;

void OnKeyboard(unsigned char c, int x, int y) {
	float step = 2;
	if(c == 27)      //esc    <orignal>
	{
		Camera_Mode1=0 ; 
		Camera_Mode2=0 ; 
		Camera_Mode3=1 ; 
	}
	else if (c == 13 )  // enter    < inside the plane >
	{
		Camera_Mode1=1 ; 
		Camera_Mode2=0 ; 
		Camera_Mode3=0 ; 
	}
	else if (c==9 )  // tab             <outside the plan > 
	{
		Camera_Mode1=0 ; 
		Camera_Mode2=1 ;
		Camera_Mode3=0 ; 
	}

	switch(toupper(c))	
	{
		case ' ':
		
			game.HeroShoot();
			game.ShootAdvancedBullets();
		break;
	
		case 'Y':
			Yawangle += 0.1f; 
			camera.Yaw(Yawangle);
			
			break;
		
		case 'P': 
			Pitchangle += 0.1f; 
			camera.Pitch(Pitchangle);
			break;
	    case 'R': 
		   Rollangle += 0.1f; 
		   camera.Roll(Rollangle); 
		   break;
	    case 'F': 
		  camera.Walk(1); 
		  break;

        case 'V': 
		  camera.Walk(-1);
		  break;

	    case 'B': 
		 camera.Strafe(1); 
		 break;

       case 'C': 
		
		camera.Strafe(-1); 
		break;
    case 'U': 
		camera.Fly(1); 
		break;
	case 'D': 
		camera.Fly(-1); 
		break;
	case 'O': 
		OOOP=1;
		break;
	//to pause game
	case 'S':
		int ret = MessageBox(NULL, "Game Paused\nTo continue press Yes to exit game press No", "Pause", MB_YESNO);
		if (ret == IDNO)
			exit(0);
		break;
	}

}

void OnSpecial(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_UP: //move forward
		if (game.hero.P.z <= -200)
			break;
			game.hero.P.z -= 2;
		break;
		case GLUT_KEY_DOWN: //move backward
			if (game.hero.P.z >= 55)
				break;
			game.hero.P.z += 2;
		break;
		case GLUT_KEY_LEFT: //move left
			if (game.hero.P.x <= -200)
				break;
			game.hero.P.x -= 2;
		break;
		case GLUT_KEY_RIGHT: //move right
			if (game.hero.P.x >= 200)
				break;
			game.hero.P.x += 2;
		break;
	}
}

void Update()
{
	UINT currTime = timeGetTime();
	//the first update?
	if(prevFrameTime == 0) {
		prevFrameTime = currTime;
		return;
	}
	//interframe time (in seconds)
	float dt = (currTime - prevFrameTime)/1000.0;
	if (OOOP)
		op=0;
	else if (Currentangle > 8 )// || currAngle < -10  ) 
		{
			//currAngle=-currAngle ;
			op = 0 ; 
	}
	else if (Currentangle < -8  ) 
		{
			//currAngle=-currAngle ;
			op = 1 ; 
	}




	if (op==0)
		Currentangle -= 45 * dt;
	else 
		Currentangle += 45 * dt;

	//Write all update code here.	
	prevFrameTime = currTime;
	greenBulletsCheck();
	RedConesCheck();
	CheckCollisions();
	game.HandleCollisions();
	game.Update(dt);
	UpdateGreenBullets(2*dt);
	UpdateRedCones(2*dt);
	spotAngle += 2;
	game.hero.animTimeSec += 120 * dt;

	//Display health, score, fps
	char titleStr[512];
	sprintf(titleStr, "Health: %d, Score: %d, Level : %d, fps: %f", game.heroHealth, game.heroScore, getLevel(mode), 1/dt);
	glutSetWindowTitle(titleStr);
}

void OnDisplay() {
	//set the background color to white
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	SetTransformations();

	    glPushMatrix();
	{
        glScalef(10, 10, 10);
        // Inside OnPaint
		glRotatef(fRotAroundY, 0, 1, 0);

		// setup texture mapping
		glEnable( GL_TEXTURE_2D );
		//glBindTexture( GL_TEXTURE_2D, texture );

        DrawCube();        
    }
    glPopMatrix();

	//drawing code geos here
	RenderGreenBullets();
	//SetLights();
	//SetMaterial();
	RenderRedCones();
	game.Render();


	glFlush();
	glutSwapBuffers();
	Update();
	glutPostRedisplay();
}


int main(int argc, char* argv[]) {
//	engine = createIrrKlangDevice();
	//engine->play2D("C:/Users/user/Documents/Visual Studio 2012/Projects/HelloWorld-Sounds/media/MF-W-90.XM", true);
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

  // select modulate to mix texture with color for shading
//  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );


  // when texture area is small, bilinear filter the closest MIP map
 // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
   //                GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first MIP map
 // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
    //               wrap ? GL_REPEAT : GL_CLAMP );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
    //               wrap ? GL_REPEAT : GL_CLAMP );

  // build our texture MIP maps
  
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,
    height, GL_RGB, GL_UNSIGNED_BYTE, data );

  // free buffer
  free( data );

  return texture;

}

void InitGraphics(int argc, char *argv[]) {
	
	InitGreenBullets();
	InitRedCones();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100); //pass (-1, -1) for Window-Manager defaults
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL Lab");
	glutKeyboardFunc(OnKeyboard);
	glutSpecialFunc(OnSpecial);
	glutDisplayFunc(OnDisplay);

	glEnable(GL_DEPTH_TEST);
	texture = LoadTextureRAW( "texture.raw", TRUE );

	
	//Reset the hero position, hero size, number of enemies, minimum position for enemies and maximum position for enemies
	game.Reset(Vector3(0,-50,-50) , 10, 0, Vector3(-50, 0, -30), Vector3(50, 0, -150), Currentangle);
	glutMainLoop();
}




void DrawCube() // for the texture
{

    ////+z square
    //glPushMatrix();
    //{
    //    //glColor3f(1, 0, 0);
    //    glTranslatef(0, 0, .5f);
    //    DrawSquare();
    //}
    //glPopMatrix();

    ////-z square
    //glPushMatrix();
    //{
    //    //glColor3f(0, 1, 0);
    //    glTranslatef(0, 0, -.5f);
    //    DrawSquare();
    //}
    //glPopMatrix();

    //+x square
    //glPushMatrix();
    //{
    //    //glColor3f(1, .5f, 0);
    //    glTranslatef(.5f, 0, 0);
    //    glRotatef(90, 0, 1, 0);
    //    DrawSquare();
    //}
    //glPopMatrix();

    //-x square
    //glPushMatrix();
    //{
    //    //glColor3f(0, 1, .5f);
    //    glTranslatef(-.5f, 0, 0);
    //    glRotatef(90, 0, 1, 0);
    //    DrawSquare();
    //}
    //glPopMatrix();

    //+y square
    //glPushMatrix();
    //{
    //    //glColor3f(.5f, 0, 1);
    //    glTranslatef(0, .5f, 0);
    //    glRotatef(90, 1, 0, 0);
    //    DrawSquare();
    //}
    //glPopMatrix();

    //-y square
    glPushMatrix();
    {
        //glColor3f(1, 0, .5f);
		glScalef(60,40,70);
        glTranslatef(-.1f, -.5f, -.1f);
	   glRotatef(90, 1, 0, 0);
        DrawSquare();
    }
    glPopMatrix();
}

/**
Draws a square.
*/
void DrawSquare()  // for the texture
{
    glBegin(GL_QUADS);
	glTexCoord2d(0.0,0.0);
	glVertex2f(-.5f, -.5f);

	glTexCoord2d(1.0,0.0);
    glVertex2f(.5f, -.5f);

	glTexCoord2d(1.0,1.0);
    glVertex2f(.5f, .5f);

	glTexCoord2d(0.0,1.0);
    glVertex2f(-.5f, .5f);
    glEnd();
}


void RenderGreenBullets()
{
	for (int i = 0; i < greenBullets.size(); i++)
			greenBullets[i].Render();
}

void UpdateGreenBullets(float dt)
{
	for (int i = 0; i < greenBullets.size(); i++)
		greenBullets[i].Update(dt);
	game.hero.angle=Currentangle;
}
void InitRedCones(){
//{
	Vector3 p(0, -50, -500);
	Vector3 v(0, 0, 20);
//	
//
	if (mode == 0  ||mode ==1 ){

	redCones.push_back(RedCone(Vector3 (40, -50, -500), Vector3(0, 0, 0)));
	redCones.push_back(RedCone(Vector3 (55, -50, -500), Vector3(0, 0, 0)));
	redCones.push_back(RedCone(Vector3 (70, -50, -500), Vector3(0, 0, 0)));
	redCones.push_back(RedCone(Vector3 (20, -50, -500), Vector3(0, 0, 0))); 
	
	}
//
//	redCones.push_back(RedCone(Vector3 (-40, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-55, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-70, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	}
//
//	if (mode == 2 || mode == 3) {
//	redCones.push_back(RedCone(Vector3 (40, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (55, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (70, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (20, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (7, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//
//	redCones.push_back(RedCone(Vector3 (-40, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-55, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-70, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-7, -50, -500), Vector3(0, 0, game.RandFloat(30, 100 ))));
//	}
//	if (mode == 4 ) {
//	redCones.push_back(RedCone(Vector3 (100, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (85, -50, -600), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (40, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (55, -50, -600), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (70, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (25, -50, -600), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (10, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -600), Vector3(0, 0, game.RandFloat(30, 100 ))));
//
//	redCones.push_back(RedCone(Vector3 (-20, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -600), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -600), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -500), Vector3(0, 0, game.RandFloat(30, 100 ))));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -600), Vector3(0, 0, game.RandFloat(30, 100 ))));
//	}
//
//	if (mode == 5 ) {
//	redCones.push_back(RedCone(Vector3 (100, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (85, -50, -700), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (40, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (55, -50, -700), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (70, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (25, -50, -700), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (10, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -700), Vector3(0, 0, game.RandFloat(30, 100 ))));
//
//	redCones.push_back(RedCone(Vector3 (-20, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -700), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -500), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -700), Vector3(0, 0, game.RandFloat(30, 100))));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -500), Vector3(0, 0, game.RandFloat(30, 100 ))));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -700), Vector3(0, 0, game.RandFloat(30, 100 ))));
//	}
//	
//	if (mode == 7 || mode == 6 ) {
//
//	
//	redCones.push_back(RedCone(Vector3 (145, -50,  -700), Vector3(0, 0,80)));
//	redCones.push_back(RedCone(Vector3 (130, -50,  -700), Vector3(0, 0,80)));
//	redCones.push_back(RedCone(Vector3 (115, -50,  -700), Vector3(0, 0,80)));
//
//	redCones.push_back(RedCone(Vector3 (100, -50, -600), Vector3(0, 0,70)));
//	redCones.push_back(RedCone(Vector3 (85, -50,  -600), Vector3(0, 0,70)));
//	redCones.push_back(RedCone(Vector3 (70, -50,  -600), Vector3(0, 0,70)));
//	redCones.push_back(RedCone(Vector3 (55, -50,  -600), Vector3(0, 0,70)));
//	redCones.push_back(RedCone(Vector3 (40, -50,  -600), Vector3(0, 0,70)));
//
//
//	redCones.push_back(RedCone(Vector3 (25, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -700),Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -700),Vector3(0, 0, 80)));
//	
//
//	redCones.push_back(RedCone(Vector3 (-50, -50, -600), Vector3(0, 0, 70)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -600), Vector3(0, 0, 70)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -600), Vector3(0, 0, 70)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -600), Vector3(0, 0, 70)));
//	redCones.push_back(RedCone(Vector3 (-110, -50, -600),Vector3(0, 0, 70)));
//
//	redCones.push_back(RedCone(Vector3 (-125, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-140, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-155, -50, -700), Vector3(0, 0, 80)));
//	}
//
//	//--------------------------------------------------------------LEVEL 2 --------------------------------------------------------------------
//	if (mode == 8  ) {
//
//	redCones.push_back(RedCone(Vector3 (0, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (5, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-10, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (5, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (0, -50, -500), Vector3(0, 0, 50)));
//
//
//	redCones.push_back(RedCone(Vector3 (40, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (35, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (45, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (30, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (50, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (45, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (35, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (40, -50, -500), Vector3(0, 0, 50)));
//
//	redCones.push_back(RedCone(Vector3 (-40, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-45, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-30, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-45, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-40, -50, -500), Vector3(0, 0, 50)));
//	
//	}
//
//	if (mode == 10  ||  mode == 9 ) {
//
//	redCones.push_back(RedCone(Vector3 (0, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (5, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-10, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (5, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (0, -50, -500), Vector3(0, 0, 50)));
//	
//
//	redCones.push_back(RedCone(Vector3 (40, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (35, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (45, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (30, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (50, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (45, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (35, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (40, -50, -500), Vector3(0, 0, 50)));
//
//	redCones.push_back(RedCone(Vector3 (-40, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-45, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-30, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-45, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-40, -50, -500), Vector3(0, 0, 50)));
//
//
//	redCones.push_back(RedCone(Vector3 (80, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (75, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (85, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (70, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (90, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (75, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (85, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (80, -50, -500), Vector3(0, 0, 50)));
//
//
//	redCones.push_back(RedCone(Vector3 (-80, -50, -700), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-75, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-85, -50, -650), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-70, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-90, -50, -600),Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-75, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-85, -50, -550), Vector3(0, 0, 50)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -500), Vector3(0, 0, 50)));
//	
//	}
//
//	// -----------------------------------------------------LEVEL 3 --------------------------------------------------------------------------
//
//
//	if ( mode == 11 || mode == 12 ) {
//		game.weaponType = 3;
//	redCones.push_back(RedCone(Vector3 (100, -50, -500), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (85, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (40, -50, -500), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (55, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (70, -50, -500), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -500), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -700), Vector3(0, 0, 80)));
//
//	redCones.push_back(RedCone(Vector3 (-20, -50, -500), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -500), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -700), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -500), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -700), Vector3(0, 0, 80)));
//	
//
//
//    redCones.push_back(RedCone(Vector3 (100, -50, -800), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (85, -50, -1000), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (40, -50, -800), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (55, -50, -1000), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (70, -50, -800), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -1000), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -800), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -1000), Vector3(0, 0, 80)));
//
//	redCones.push_back(RedCone(Vector3 (-20, -50, -800), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -1000), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -800), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -1000), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -800), Vector3(0, 0, 80)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -1000), Vector3(0, 0, 80)));
//	}
//
//
//	if (mode == 13 || mode == 14 || mode ==15 || mode == 16 ) {
//		
//	redCones.push_back(RedCone(Vector3 (145, -50,  -700), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (130, -50,  -700), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (115, -50,  -700), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (100, -50, -600), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (85, -50,  -600), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (70, -50,  -600), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (55, -50,  -600), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (40, -50,  -600), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -700), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -700), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -700), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -700),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -700),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -600), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -600), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -600), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -600), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-110, -50, -600),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-125, -50, -700), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-140, -50, -700), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-155, -50, -700), Vector3(0, 0, 100)));
//
//
//
//	redCones.push_back(RedCone(Vector3 (145, -50,  -900), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (130, -50,  -900), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (115, -50,  -900), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (100, -50, -1000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (85, -50,  -1000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (70, -50,  -1000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (55, -50,  -1000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (40, -50,  -1000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -900), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -900), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -900), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -900),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -900),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -1000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -1000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -1000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -1000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-110, -50, -1000),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-125, -50, -900), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-140, -50, -900), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-155, -50, -900), Vector3(0, 0, 100)));
//
//
//
//	redCones.push_back(RedCone(Vector3 (145, -50,  -1200), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (130, -50,  -1200), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (115, -50,  -1200), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (100, -50, -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (85, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (70, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (55, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (40, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -1200),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -1200),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-110, -50, -1300),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-125, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-140, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-155, -50, -1200), Vector3(0, 0, 100)));
//	}
//
//	if ( mode >= 17 )// mode == 18 || mode == 19) {
//	{
//
//	redCones.push_back(RedCone(Vector3 (0, -50, -700), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-15, -50, -675), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (15, -50, -675), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-30, -50, -650),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (30, -50, -650),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-15, -50, -625), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (15, -50, -625), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (0, -50, -600), Vector3(0, 0, 90)));
//
//
//	redCones.push_back(RedCone(Vector3 (40, -50, -900), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (55, -50, -875), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -875), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (7 , -50, -850), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (70, -50, -850), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (55, -50, -825), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -825), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (40, -50, -800), Vector3(0, 0, 90)));
//
//	redCones.push_back(RedCone(Vector3 (-40, -50, -900), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-55, -50, -875), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-25, -50, -875), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-7 , -50, -850), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-70, -50, -850), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-55, -50, -825), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-25, -50, -825), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-40, -50, -800), Vector3(0, 0, 90)));
//
//	//----------------------------------
//	redCones.push_back(RedCone(Vector3 (145, -50,  -1200), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (130, -50,  -1200), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (115, -50,  -1200), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (100, -50, -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (85, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (70, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (55, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (40, -50,  -1300), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -1200),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -1200),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -1300), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-110, -50, -1300),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-125, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-140, -50, -1200), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-155, -50, -1200), Vector3(0, 0, 100)));
//	//----------------
//	redCones.push_back(RedCone(Vector3 (0, -50, -1500), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-15, -50, -1475), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (15, -50, -1475), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-30, -50, -1450),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (30, -50, -1450),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-15, -50, -1425), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (15, -50, -1425), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (0, -50, -1400), Vector3(0, 0, 90)));
//
//
//	redCones.push_back(RedCone(Vector3 (40, -50, -1700), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (55, -50, -1675), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -1675), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (7 , -50, -1650), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (70, -50, -1650), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (55, -50, -1625), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -1625), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (40, -50, -1600), Vector3(0, 0, 90)));
//
//	redCones.push_back(RedCone(Vector3 (-40, -50, -1700), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-55, -50, -1675), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-25, -50, -1675), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-7 , -50, -1650), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-70, -50, -1650), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-55, -50, -1625), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-25, -50, -1625), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-40, -50, -1600), Vector3(0, 0, 90)));
//
//
//	redCones.push_back(RedCone(Vector3 (80, -50, -1500), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (65, -50, -1475), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (95, -50, -1475), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (15, -50, -1450),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (110, -50, -1450),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (65, -50, -1425), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (95, -50, -1425), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (80, -50, -1400), Vector3(0, 0, 90)));
//
//	redCones.push_back(RedCone(Vector3 (-80, -50, -1500), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -1475), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -1475), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-15, -50, -1450),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-110, -50, -1450),Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -1425), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -1425), Vector3(0, 0, 90)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -1400), Vector3(0, 0, 90)));
//
//	//------------------------------
//
//
//		redCones.push_back(RedCone(Vector3 (145, -50,  -2100), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (130, -50,  -2100), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (115, -50,  -2100), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (100, -50, -2000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (85, -50,  -2000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (70, -50,  -2000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (55, -50,  -2000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (40, -50,  -2000), Vector3(0, 0,100)));
//	redCones.push_back(RedCone(Vector3 (25, -50, -2100), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (10, -50, -2100), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-5, -50, -2100), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-20, -50, -2100),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-35, -50, -2100),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-50, -50, -2000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-65, -50, -2000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-80, -50, -2000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-95, -50, -2000), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-110, -50, -2000),Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-125, -50, -2100), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-140, -50, -2100), Vector3(0, 0, 100)));
//	redCones.push_back(RedCone(Vector3 (-155, -50, -2100), Vector3(0, 0, 100)));
//	 }
//



}
void InitGreenBullets()
{
	Vector3 v(0, 0, 10), p(100, -50, -500);
	HealthPacket b;
	
	b = HealthPacket(p, v);

	b.V.z = game.RandFloat(30, 1000);
	greenBullets.push_back(b);
	p.x -= 50;
	v.z += 10;

	b = HealthPacket(p, v);
	b.V.z = game.RandFloat(30, 1000);
	greenBullets.push_back(b);
	p.x -= 50;
	v.z += 20;

	b = HealthPacket(p, v);
	b.V.z = game.RandFloat(30, 1000);
	greenBullets.push_back(b);
	p.x -= 50;
	v.z += 20;

	b = HealthPacket(p, v);
	b.V.z = game.RandFloat(30, 1000);
	greenBullets.push_back(b);
	p.x -= 50;
	v.z += 20;

	b = HealthPacket(p, v);
	b.V.z = game.RandFloat(30, 1000);
	greenBullets.push_back(b);
	p.x -= 50;
	v.z += 30;
}
void RenderRedCones()
{
	for (int i = 0; i < redCones.size(); i++)
			redCones[i].Render();
}
void UpdateRedCones(float dt)
{
	for (int i = 0; i < redCones.size(); i++)
		redCones[i].Update(dt);
}

void greenBulletsCheck()
{
	for (int i = 0; i < greenBullets.size(); i++)
	{
		if (greenBullets[i].P.z >= 60)
		{
			greenBullets[i].Reset();
			greenBullets[i].V.z = game.RandFloat(30, 1000);
			greenBullets[i].V.x = game.RandFloat(30, 1000);
		}
	}
}

void RedConesCheck()
{
	/*if (redCones.size() == 0 && game.heroHealth != 0)
	{
		if (MessageBox(NULL, "Congrats!!! \nYOU WON", "GAME OVER", MB_OK) == IDOK)
			exit(0);
	}*/
	int done = 0 ; 
	if (game.heroHealth == 0)
	{
		if (MessageBox(NULL, "YOU LOSE :(\nDo you want to play again??", "Game Over!!", MB_YESNO) == IDYES)
		{
			mode = 0;
			InitGraphics(largc, largs);
			//game.Reset(Vector3(0,-50,-50) , 10, 0, Vector3(-50, 0, -30), Vector3(50, 0, -150),Currentangle);
			return;
		}
		else
			exit(0);
	}

	if (redCones.size() == 0)
	{
		if (getLevel(mode) == 3)
		{
			string score = toString(game.heroScore);
			string msg = "Congrats YOU WON :D :D :D\nYour Score : ";
			msg += score;
			msg += "\nDo you want to play again??";
			if (MessageBox(NULL, msg.c_str(), "Game Over!!", MB_YESNO) == IDYES)
			{
				mode = 0;
				InitGraphics(largc, largs);
				//game.Reset(Vector3(0,-50,-50) , 10, 0, Vector3(-50, 0, -30), Vector3(50, 0, -150),Currentangle);
				return;
			}
			else
				exit(0);
		}
		if (MessageBox(NULL, "Do you want to move to the next level??", "Next Level", MB_YESNO) == IDYES)
		{
			mode+=2;
			InitRedCones();
		}
		else
			exit(0);
		
	}
	int size = redCones.size() ;
	for (size_t i = 0 ; i < redCones.size(); i ++ ) {
		if (redCones[i].p.z >= 50){
			redCones[i].Reset();
		//redCones.erase(redCones.begin()+i);
		done++ ; 
		}
	}
	/*if (done == size ){ 
		mode++ ; 
		InitRedCones();
	}*/
	
		
}

void CheckCollisions()
{

	size_t nEnems = redCones.size();
	for(int enemInd = nEnems-1; enemInd>=0; enemInd--) {
		AABoundingBox enemBox = redCones[enemInd].GetAABB();
		size_t nBulls = game.heroBullets.size();
		Bullet b(redCones[enemInd].p, redCones[enemInd].v);
		b.pinit = redCones[enemInd].pinit;
		b.vinit = redCones[enemInd].vinit;
		for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
			if(game.heroBullets[bullInd].GetAABB().IntersectsWith(enemBox)) {
				//game.healthPCs.push_back(HealthPacket(enemBox.Center, 20));
				//greenBullets.push_back(b);
				game.heroBullets.erase(game.heroBullets.begin() + bullInd);
				redCones.erase(redCones.begin() + enemInd);
				game.heroScore += 100;
				break;
			}
		}
	}

	//Check collisons with green bullets
	AABoundingBox heroBox(game.GetHeroPos(), Vector3(10, 10, 10));
	size_t numGB = greenBullets.size();
	for(int i=numGB-1; i>=0; i--) {
		AABoundingBox hpBox = greenBullets[i].GetAABB();
		if(hpBox.IntersectsWith(heroBox)) {
			greenBullets.erase(greenBullets.begin() + i);
			game.heroHealth += 20;
			//game.healthPCs.erase(game.healthPCs.begin() + i);
		}
	}
	//Check hero collisons
	size_t redConesCount = redCones.size();
	for(int bInd = redConesCount-1; bInd>=0; bInd--) 
		if(redCones[bInd].GetAABB().IntersectsWith(heroBox)) {
			game.heroHealth = max(game.heroHealth - 20, 0);
			redCones.erase(redCones.begin() + bInd);
			if(game.heroHealth == 0) game.gameOver = true;
		}

		AdvancedBulletsWithRedCones();

	//size_t greenBulletsCount = greenBullets.size();
	//for( greenBulletsCount = greenBulletsCount-1; greenBulletsCount>=0; greenBulletsCount--) {
	//	for(int bullInd = nBulls-1; bullInd>=0; bullInd--) {
	//		AABoundingBox greenBuleetBox = greenBullets[greenBulletsCount].GetAABB();
	//		if(game.heroBullets[bullInd].GetAABB().IntersectsWith(greenBuleetBox)) {
	//			//game.healthPCs.push_back(HealthPacket(enemBox.Center, 20));
	//			
	//			size_t nBulls = game.heroBullets.size();
	//			RedCone rc(greenBullets[greenBulletsCount].P, greenBullets[greenBulletsCount].V);
	//			rc.pinit = greenBullets[greenBulletsCount].pinit;
	//			rc.vinit = greenBullets[greenBulletsCount].vinit;
	//			redCones.push_back(rc);
	//			game.heroBullets.erase(game.heroBullets.begin() + bullInd);
	//			greenBullets.erase(greenBullets.begin() + greenBulletsCount);
	//			game.heroScore -= 20;
	//			break;
	//		}
	//	}
	//}
		
}

void AdvancedBulletsWithRedCones()
{
	//Check Collisions with ABup
	for (size_t i = 0; i < redCones.size(); i++)
	{
		for (size_t j = 0; j < game.ABup.size(); j++)
		{
			if(game.ABup[j].GetAABB().IntersectsWith(redCones[i].GetAABB())) 
			{
				//greenBullets.push_back(Bullet(redCones[i].p, redCones[i].v));
				game.ABup.erase(game.ABup.begin() + j);
				redCones.erase(redCones.begin() + i);
				game.heroScore += 100;
				break;
				
				
			}
		}
	}

	//Check Collisions with ABright
	for (size_t i = 0; i < redCones.size(); i++)
	{
		for (size_t j = 0; j < game.ABright.size(); j++)
		{
			if(game.ABright[j].GetAABB().IntersectsWith(redCones[i].GetAABB())) 
			{
				//greenBullets.push_back(Bullet(redCones[i].p, redCones[i].v));
				game.ABright.erase(game.ABright.begin() + j);
				redCones.erase(redCones.begin() + i);
				game.heroScore += 100;
				break;
			}
		}
	}

	//Check Collisions with ABleft
	for (size_t i = 0; i < redCones.size(); i++)
	{
		for (size_t j = 0; j < game.ABleft.size(); j++)
		{
			if(game.ABleft[j].GetAABB().IntersectsWith(redCones[i].GetAABB())) 
			{
				//greenBullets.push_back(Bullet(redCones[i].p, redCones[i].v));
				game.ABleft.erase(game.ABleft.begin() + j);
				redCones.erase(redCones.begin() + i);
				game.heroScore += 100;
				break;
			}
		}
	}
}

