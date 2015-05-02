#include <cstdlib>
#include "GameManager.h"
#include<vector>
#include"BadObject.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <ctime>
#include <cstdio>
using namespace std;
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

// Global Variables ***/


void GameManager::Reset(const Vector3 &heroPos, int teapotSize, size_t numEnemies, const Vector3 &minPos, const Vector3 &maxPos , float angle) {
	hero.P = heroPos;
	hero.angle = angle ; 
	gameOver =  false;
	heroScore = 0;
	heroHealth = 100;
}


void GameManager::HandleCollisions() {
	AABoundingBox heroBox(GetHeroPos(), Vector3(10, 10, 10));
}


void GameManager::Update(float dtSec) {
	if(gameOver) return;
	HandleCollisions();
	size_t nb = heroBullets.size();
	for(int b=nb-1; b>=0; b--) {
		heroBullets[b].Update(dtSec);
	}
}


void GameManager::Render()  {	
	hero.Render();
	size_t nb = heroBullets.size();
	for(int b=0; b<nb ; b++)
		heroBullets[b].Render();


	rope1.P = Vector3 ( -40 , -45 , -100) ; 
	rope2.P = Vector3 ( +50 , -45 , +80) ; 
	rope1.Render();
	rope2.Render();

	x.Render();
	//car1.P=Vector3 ( +50 , -50 , -100) ;   // msh zahraa :@ 
	car1.Render();
	//car2.Render();
	/*car3.Render();
 	 car4.Render();*/



}
