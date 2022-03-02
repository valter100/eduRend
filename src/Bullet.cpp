#include "Bullet.h"

Bullet::Bullet(const std::string& objfile, ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context, mat4f startTransform) : OBJModel(objfile, dxdevice, dxdevice_context, false)
{
	transformMatrix = startTransform;
	speed = 1.5f;
	timeAlive = 0;
	lifeTime = 4;
}

void Bullet::Update()
{
	SetTransformMatrix(transformMatrix * mat4f::translation(0.0f, -1.8f * (timeAlive / lifeTime), -speed));
	timeAlive += (float)1 / 144;
}

void Bullet::SetTransformMatrix(mat4f matrix)
{
	transformMatrix = matrix;
}

void Bullet::SetTransformMatrix(mat4f _transform, mat4f _rotation, mat4f _scale) //current
{
	transform = _transform;
	rotation = _rotation;
	scale = _scale;

	transformMatrix = transform * rotation * scale;
}

mat4f Bullet::GetTransformMatrix()
{
	return transformMatrix;
}

Bullet::~Bullet()
{
	std::cout << "Bullet destroyed!" << std::endl;
}

bool Bullet::IsActive()
{
	return timeAlive < lifeTime;
}