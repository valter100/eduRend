#pragma once
#include "vec/vec.h"
#include "vec\mat.h"
#include "Model.h"

using namespace linalg;

class Light
{
	vec3f position;
	vec3f oldPosition;

	bool isAnimated;
	bool headingTowardsFirstPos;
	vec3f movementSpeed;

	std::vector<vec3f> positions;
	float positionIndex;
	float movementRate;

	vec3f firstPos;
	vec3f secondPos;

	vec3f goalVector;

	OBJModel* lightSphere;
	mat4f MLightSphere;

public:

	Light(vec3f startPosition, bool _isAnimated, ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context);
	void Update();
	void Animate(vec3f second);
	bool IsAnimated();
	void AddAnimationPath(vec3f second);
	void SetOldPositionToPosition();
	void SetGoalVector(vec3f newGoal);
	vec3f GetFirstPos();
	vec3f GetPosition();
	bool HasMoved();
	void Render();
	mat4f GetTransformMatrix();
};