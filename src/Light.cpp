#include "Light.h"
#include <iostream>

Light::Light(vec3f newPosition, bool _isAnimated)
{
	position = newPosition;

	firstPos = position;
	isAnimated = _isAnimated;
	headingTowardsFirstPos = true;
}

void Light::Update()
{
	double distance = sqrt(pow(position.x - goalVector.x, 2) + pow(position.y - goalVector.y, 2) + pow(position.z - goalVector.z, 2));

	position.x += (float)movementSpeed.x / (144 * 5);
	position.y += (float)movementSpeed.y / (144 * 5);
	position.z += (float)movementSpeed.z / (144 * 5);

	//std::cout << distance << std::endl;

	if (distance < 2)
	{
		if (goalVector == firstPos)
		{
			goalVector = secondPos;
		}
		else if (goalVector == secondPos)
		{
			goalVector = firstPos;
		}
		movementSpeed = (goalVector - position);
	}
}

bool Light::IsAnimated()
{
	return isAnimated;
}

void Light::SetAnimationPath(vec3f second)
{
	secondPos = second;

	if (isAnimated)
	{
		goalVector = second;
		movementSpeed = (goalVector - position);
	}
}

void Light::SetGoalVector(vec3f newGoal)
{
	goalVector = newGoal;
}

vec3f Light::GetPosition()
{
	return position;
}

vec3f Light::GetFirstPos()
{
	return firstPos;
}

void Light::SetOldPositionToPosition()
{
	oldPosition = position;
}

bool Light::HasMoved()
{
	return oldPosition.vec != position.vec;
}

