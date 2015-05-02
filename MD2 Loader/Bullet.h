/**
@file Bullet.h

The definition of the Bullet class. It allows to define moving 
Bullets objects of a certain lifetime.
*/

#pragma once
#include "Vector3.h"
#include "AABoundingBox.h"

/**
Represents a moving Bullet that has a certain lifetime.
*/
class Bullet
{
public:
	/**
	P: Position. V: Velocity.
	*/
	Vector3 P, V;
	Vector3 pinit, vinit;
	/**
	LifeTime: Remaining time to the death of the bullet (in sec).
	*/
	float LifeTime;

	static const float DefaultLifeTime;

	Bullet(const Vector3 &initPos, const Vector3 &velocity) : P(initPos), V(velocity)
	{
		LifeTime = DefaultLifeTime;
		pinit = P;
		vinit = V;
	}

	Bullet() {
		LifeTime = DefaultLifeTime;
	}
	void Reset()
	{
		P = pinit;
		V = vinit;
	}
	/**
	Moves the bullet and updates its lifetime.
	*/
	void Update(float dtSec);

	/**
	Renders the bullet as a sphere whose color changes according to lifetime
	(r: 1 -> 0,
	 g: 0, 
	 b: 0 -> 1).
	*/
	void Render() const;

	/**
	True if the bullet's lifetime is 0.
	*/
	bool HasExpired() const { return LifeTime <= 0; }


	AABoundingBox GetAABB() const {
		return AABoundingBox(P, Vector3(2, 2, 2));
	}

	void UpdateAdvancedBullet(float dt);
};