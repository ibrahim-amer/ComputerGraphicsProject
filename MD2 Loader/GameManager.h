#pragma once

#include "EnemyBody.h"
#include "Vector3.h"
#include <windows.h>
#include <vector>
#include "HealthPacket.h"
#include <math.h>
#include "HeroTeapot.h"
#include<ctime>
#include "BadObject.h"

using namespace std;

class GameManager {
public:
	vector<Bullet> heroBullets;
	vector<Bullet> enemyBullets;
	vector<HealthPacket> greenBullets; 
	vector< vector<Bullet> > heroBulletsAdvanced;
	vector<Bullet> ABright;
	vector<Bullet> ABleft;
	vector<Bullet> ABup;
	int weaponType;
	double lineZaxis;
	vector<EnemyBody> enemyBodies;
	vector<HealthPacket> healthPCs;
	vector<BadObject> badObjects;
	HeroTeapot hero;

	int heroHealth; //0 -> 100
	int heroScore;
	bool gameOver;
	
	GameManager() : hero(Vector3(0,0,10), 10)
	{
		healthPCs.resize(0);
		weaponType = 0;
		lineZaxis = hero.P.z;
	}
	/**
	Returns a random real number betn min and max.
	*/
	static float RandFloat(float min, float max) {
		srand((unsigned)time(0));
		return rand()/((double)RAND_MAX + 1) * (max - min) + min;
		
	}

	void Reset(const Vector3 &heroPos, int teapotSize, size_t numEnemies, const Vector3 &minPos, const Vector3 &maxPos , float a );

	Vector3 GetHeroPos() const {
		return hero.P;
	}

	void HandleCollisions();

	void Update(float dtSec);

	void HeroShoot() {
		if(gameOver && weaponType != 0) return;
		Bullet hb(hero.P, Vector3(0,0,-90)); //hero bullet will go over the -ve z axis
		heroBullets.push_back(hb);
	}

	void Render() ;

	void RenderLine()
	{
		if (weaponType != 2)
			return;
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glColor3f(0, 1, 0);
		glBegin(GL_LINE);
		glVertex3d(-100, -50, lineZaxis);
		glVertex3d(100, -50, lineZaxis);
		glEnd();
	glPopMatrix();
	}
	void UpdateLine(float dt, Vector3 V)
	{
		if (weaponType != 2)
			return;
		lineZaxis += dt * V.z;
	}

	void ShootAdvancedBullets()
	{
		if (weaponType != 3)
			return;
		ABup.push_back(Bullet(hero.P, Vector3(0, 0, -90)));
		ABright.push_back(Bullet(hero.P, Vector3(90, 0, -90)));
		ABleft.push_back(Bullet(hero.P, Vector3(-90, 0, -90)));
	}

	void UpdateAdvancedBullets(float dt)
	{
		for (size_t i = 0; i < ABup.size() ; i++)
		{
			ABup[i].UpdateAdvancedBullet(dt);
		}
		for (size_t j = 0; j < ABright.size(); j++)
		{
			ABright[j].UpdateAdvancedBullet(dt);
		}
		for (size_t k = 0; k < ABleft.size(); k++)
		{
			ABleft[k].UpdateAdvancedBullet(dt);
		}
	}
};