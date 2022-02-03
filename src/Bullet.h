#pragma once
#include "Model.h"

class Bullet : public OBJModel
{
public:
	mat4f transform;
	mat4f rotation;
	mat4f scale;

	float speed;
	float lifeTime;
	float timeAlive;

	mat4f transformMatrix;

	Bullet(const std::string&, ID3D11Device*, ID3D11DeviceContext*, mat4f);
	~Bullet();
	void Update();
	void SetTransformMatrix(mat4f matrix);
	void SetTransformMatrix(mat4f _transform, mat4f _rotation, mat4f _scale);
	mat4f GetTransformMatrix();
	bool IsActive();
};