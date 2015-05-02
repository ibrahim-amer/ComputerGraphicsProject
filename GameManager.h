#pragma once

#include "Vector3.h"
#include <windows.h>
#include <vector>
#include <math.h>
#include "HeroTeapot.h"
#include<ctime>
#include "RedCone.h"
#include "enemyBody2.h"
#include "beerbarrel.h"
#include "buggy.h"
#include "heli.h"
#include "rope.h"
#include "car.h"
#include"Plant.h"
#include"phonex.h"


using namespace std;

class GameManager {
public:
	vector<Bullet> heroBullets;
	double lineZaxis;
	HeroTeapot hero;
	enemyBody2 Enemy;
	enemyBody2 Enemy2;
	enemyBody2 Enemy3;
	enemyBody2 Enemy4;
	enemyBody2 Enemy5;
	enemyBody2 Enemy6;
	enemyBody2 Enemy7;
	enemyBody2 Enemy8;
	enemyBody2 Enemy9;
	enemyBody2 Enemy10;
	enemyBody2 Enemy11;
	enemyBody2 Enemy12;

	enemyBody2 Enemy13;
	enemyBody2 Enemy14;
	enemyBody2 Enemy15;
	enemyBody2 Enemy16;
	heli h ;
	heli h2 ;
	heli h3 ;

	Plant p1;
	Plant p2;
	Plant p3;
	Plant p4;

	beerbarrel bb ; 

	buggy buggy1 ; 
	buggy buggy2 ; 

	rope rope1 ; 
	rope rope2 ; 

	car car1 ;
	car car2 ; 
	car car3 ;
	car car4 ;


	phonex x ; 


	//RedCone cone ; 
	

	int heroHealth; //0 -> 100
	int heroScore;
	bool gameOver;
	// al pos de domy 
	
	GameManager() : hero(Vector3(0,0,10), 10) , Enemy(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , Enemy2(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) ,Enemy3(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) ,
		Enemy4(Vector3(55,-50,-500), 10 , Vector3(0,0,0)),Enemy5(Vector3(55,-50,-500), 10 , Vector3(0,0,0)),Enemy6(Vector3(55,-50,-500), 10 , Vector3(0,0,0)),Enemy7(Vector3(55,-50,-500), 10 , Vector3(0,0,0)),
		Enemy8(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , Enemy9(Vector3(55,-50,-500), 10 , Vector3(0,0,0)), Enemy10(Vector3(55,-50,-500), 10 , Vector3(0,0,0))  , Enemy11(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , 
		Enemy12(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , Enemy13(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , Enemy14(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , Enemy15(Vector3(55,-50,-500), 10 , Vector3(0,0,0)),
		Enemy16(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , h(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , h2(Vector3(55,-50,-500), 10 , Vector3(0,0,0)),h3(Vector3(55,-50,-500), 10 , Vector3(0,0,0))
		,bb(Vector3(55,-50,-500), 10 , Vector3(0,0,0)) , buggy1(Vector3(-200,-50,100), 10 , Vector3(0,0,0)) , buggy2(Vector3(55,-50,-500), 10 , Vector3(0,0,0))
		, rope1(Vector3(30,-50,-60), 10 , Vector3(0,0,0)) , rope2(Vector3(0,-50,-100), 10 , Vector3(0,0,0)) 
	   , car1(Vector3(770, -250, 600), 10 , Vector3(0,0,0)) , car2(Vector3(-770, -250, -600), 10 , Vector3(0,0,0)) , car3(Vector3(0,-50,-100), 10 , Vector3(0,0,0)) , car4(Vector3(0,-50,-100), 10 , Vector3(0,0,0)), p1(Vector3(-100,-50,-120), 10 , Vector3(0,0,0)), 
	   p2(Vector3(90,-50,-140), 10 , Vector3(0,0,0)), p3(Vector3(-90,-50,140), 10 , Vector3(0,0,0)), p4(Vector3(-90,-50,-140), 10 , Vector3(0,0,0))
	, x(Vector3(-20 , -10 , -170) ,  10 , Vector3(0,0,0))
	{	//(-100,-50,-120)
		
		//buggy1.P= Vector3(70,-50,-600);
		lineZaxis = hero.P.z;
		p2.scl=4;  // just to scale the tree 
		p3.scl=4;
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

	void Render() ;

	



};