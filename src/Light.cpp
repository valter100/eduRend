#include "Light.h"
#include <iostream>

Light::Light(vec3f newPosition, bool _isAnimated, ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context)
{
	position = newPosition;

	positions.push_back(newPosition);
	isAnimated = _isAnimated;
	positionIndex = 0;

	lightSphere = new OBJModel("assets/objects/sphere/sphere.obj", dxdevice, dxdevice_context);
	MLightSphere = (mat4f::translation(1, 1, 1),
		mat4f::rotation(0, 0.0f, 1.0f, 0.0f),
		mat4f::scaling(0.1f));

	lightSphere->SetBasicMaterialValues(
		vec3f{ 1.0f,1.0f,1.0f },
		vec3f{ 1.0f,1.0f,1.0f },
		vec3f{ 1.0f,1.0f,1.0f },
		0.0f
	);
}

void Light::Update()
{
	double distance = sqrt(pow(position.x - goalVector.x, 2) + pow(position.y - goalVector.y, 2) + pow(position.z - goalVector.z, 2));

	position.x += (float)movementSpeed.x / (144 * 2);
	position.y += (float)movementSpeed.y / (144 * 2);
	position.z += (float)movementSpeed.z / (144 * 2);

	if (distance < 1)
	{
		positionIndex++;
		if (positionIndex >= positions.size())
		{
			positionIndex = 0;
		}

		goalVector = positions[positionIndex];
		movementSpeed = (goalVector - position);
	}
}

void Light::Animate(vec3f second)
{
	positions.push_back(second);

	goalVector = positions[++positionIndex];
	movementSpeed = (goalVector - position);
}

bool Light::IsAnimated()
{
	return isAnimated;
}

void Light::AddAnimationPath(vec3f newPosition)
{
	if (positions.size() == 1)
	{
		Animate(newPosition);
	}

	positions.push_back(newPosition);
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

void Light::Render()
{
	lightSphere->Render();
}

mat4f Light::GetTransformMatrix()
{
	return mat4f::translation(position) * MLightSphere;
}


