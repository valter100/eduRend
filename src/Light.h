#pragma once
#include "vec/vec.h"
#include "vec\mat.h"

using namespace linalg;

class Light
{
	vec3f position;
	vec3f oldPosition;

	bool isAnimated;
	bool headingTowardsFirstPos;
	vec3f movementSpeed;

	vec3f firstPos;
	vec3f secondPos;

	vec3f goalVector;

public:

	Light(vec3f startPosition, bool _isAnimated);
	void Update();
	bool IsAnimated();
	void SetAnimationPath(vec3f second);
	void SetOldPositionToPosition();
	void SetGoalVector(vec3f newGoal);
	vec3f GetFirstPos();
	vec3f GetPosition();
	bool HasMoved();
};