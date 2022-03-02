#include "Gun.h"

Gun::Gun(const std::string& objfile, ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context, Camera* newParent, float _fireRate) : OBJModel(objfile, dxdevice, dxdevice_context, false)
{
	camera = newParent;
	fireRate = _fireRate;
	timeSinceLastShot = 0;
}

void Gun::Update()
{
	timeSinceLastShot += (float)1/144;
}

void Gun::SetTransformMatrix(mat4f matrix)
{
	transformMatrix = matrix;
}

void Gun::SetTransformMatrix(mat4f parentTransform, mat4f _transform, mat4f _rotation, mat4f _scale) //current
{
	cameraTransformMatrix = parentTransform;

	transform = _transform;
	rotation = _rotation;
	scale = _scale;

	firePoint = cameraTransformMatrix * mat4f::translation(1.35f, -0.475f, -6.75f) * mat4f::rotation(1, 0.0f, 0.0f, 0.0f) * mat4f::scaling(0.2f);
	transformMatrix = cameraTransformMatrix * transform * rotation * scale;
}

mat4f Gun::GetTransformMatrix()
{
	return transformMatrix;
}

Camera* Gun::GetCamera()
{
	return camera;
}

void Gun::Shoot()
{
	std::cout << "Shoot!" << std::endl;
	Bullet* newBullet = new Bullet("assets/objects/sphere/sphere.obj", dxdevice, dxdevice_context, firePoint);
	bullets.push_back(newBullet);
}

bool Gun::CanShoot()
{
	if (timeSinceLastShot > fireRate)
	{
		timeSinceLastShot = 0;
		std::cout << "Amount of bullets: " << bullets.size() << std::endl;
		return true;
	}

	return false;
}

std::vector<Bullet*> Gun::GetBullets()
{
	return bullets;
}