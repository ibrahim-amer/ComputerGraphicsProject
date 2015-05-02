#include <cstdlib>
#include "GameManager.h"
#include<vector>
#include"BadObject.h"
using namespace std;

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <ctime>
#include <cstdio>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

// Global Variables ***/


void GameManager::Reset(const Vector3 &heroPos, int teapotSize, size_t numEnemies, const Vector3 &minPos, const Vector3 &maxPos , float angle) {
	hero.P = heroPos;	
	hero.angle = angle ; 
	gameOver = false;
	heroScore = 0;
	heroHealth = 100;
}


void GameManager::HandleCollisions() {
	AABoundingBox heroBox(GetHeroPos(), Vector3(10, 10, 10));
}


void GameManager::Update(float dtSec) {
	if(gameOver) return;
	HandleCollisions();
	//update bullets
	size_t nb = heroBullets.size();
	for(int b=nb-1; b>=0; b--) {
		heroBullets[b].Update(dtSec);
		if(heroBullets[b].HasExpired())
			heroBullets.erase(heroBullets.begin() + b);
	}

	UpdateAdvancedBullets(dtSec);

}


void GameManager::Render()  {
	//draw Hero
	
	hero.Render();

	size_t nb = heroBullets.size();
	for(int b=0; b<nb && weaponType == 0; b++)
		heroBullets[b].Render();


	//render Advanced Bullets
	if (weaponType == 3)
	{
		for (size_t i = 0; i < ABup.size(); i++)
		{
			ABup[i].Render();
		}
		for (size_t j = 0; j < ABright.size(); j++)
		{
			ABright[j].Render();
		}
		for (size_t k = 0; k < ABleft.size(); k++)
		{
			ABleft[k].Render();
		}
	}

}
