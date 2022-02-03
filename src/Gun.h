#pragma once
#include "Model.h"
#include "Camera.h"
#include "Bullet.h"

class Gun : public OBJModel
{
public:
	mat4f transform;
	mat4f rotation;
	mat4f scale;

	mat4f transformMatrix;
	mat4f firePointOffset;
	mat4f firePoint;

	Camera* camera;
	mat4f cameraTransformMatrix;
	std::vector<Bullet*> bullets;

	float fireRate;
	float timeSinceLastShot;

	Gun(const std::string&, ID3D11Device*, ID3D11DeviceContext*, Camera*, float);
	void Update();
	void SetTransformMatrix(mat4f matrix);
	void SetTransformMatrix(mat4f parentTransform, mat4f _transform, mat4f _rotation, mat4f _scale);
	mat4f GetTransformMatrix();
	bool CanShoot();
	Camera* GetCamera();
	std::vector<Bullet*> GetBullets();

	void Shoot();
};