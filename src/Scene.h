
#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Cube.h"
#include "Gun.h"
#include "Bullet.h"
#include "Light.h"

// New files
// Material
// Texture <- stb

// TEMP


class Scene
{
protected:
	ID3D11Device*			dxdevice;
	ID3D11DeviceContext*	dxdevice_context;
	int						window_width;
	int						window_height;

public:

	Scene(
		ID3D11Device* dxdevice,
		ID3D11DeviceContext* dxdevice_context,
		int window_width,
		int window_height);

	virtual void Init() = 0;

	virtual void Update(
		float dt,
		InputHandler* input_handler) = 0;
	
	virtual void Render() = 0;
	
	virtual void Release() = 0;

	virtual void WindowResize(
		int window_width,
		int window_height);
};

class OurTestScene : public Scene
{
	// Constant buffers (CBuffers) for data that is sent to shaders

	// CBuffer for transformation matrices
	ID3D11Buffer* transformation_buffer = nullptr;

	// + other CBuffers
	ID3D11Buffer* camera_and_light_buffer = nullptr;

	// CBuffer client-side definitions
	// These must match the corresponding shader definitions 

	struct TransformationBuffer
	{
		mat4f ModelToWorldMatrix;
		mat4f WorldToViewMatrix;
		mat4f ProjectionMatrix;
	};

	struct CameraAndLightBuffer
	{
		vec4f CamPosition;
		vec4f LightPosition1;
		vec4f LightPosition2;
	};

	//
	// Scene content
	//
	Camera* camera;
	Gun* gun;
	Bullet* bullet;

	std::vector <Cube*> cubes;
	std::vector <Bullet*> bullets;

	Light* source1;
	Light* source2;
	std::vector <Light*> lightSources;

	Cube* cube;
	Cube* secondCube;
	Cube* thirdCube;

	Cube* fourthCube;
	Cube* fifthCube;
	Cube* sixthCube;

	OBJModel* bush;
	OBJModel* sponza;
	OBJModel* character;
	OBJModel* train;
	OBJModel* sphere;

	//CompositeObject Model-toworld transformation matrices
	mat4f MBush;
	mat4f MCharacter;
	mat4f MTrain;
	mat4f MGun;
	mat4f MSphere;

	// Model-to-world transformation matrices
	mat4f Msponza;
	mat4f Mquad;

	// World-to-view matrix
	mat4f Mview;
	// Projection matrix
	mat4f Mproj;

	vec3f oldCamPosition;

	// Misc
	float angle = 0;			// A per-frame updated rotation angle (radians)...
	float angle_vel = fPI / 2;	// ...and its velocity (radians/sec)
	float camera_vel = 7.0f;	// Camera movement velocity in units/s
	float fps_cooldown = 0;
	bool isPressingKey = 0;
	bool cursorIsShowing = true;
	void InitTransformationBuffer();
	void InitCameraAndLightBuffer();

	void UpdateTransformationBuffer(
		mat4f ModelToWorldMatrix,
		mat4f WorldToViewMatrix,
		mat4f ProjectionMatrix);

	void UpdateCameraAndLightBuffer(vec4f CamPosition, vec4f LightPosition1, vec4f LightPosition2);

public:
	OurTestScene(
		ID3D11Device* dxdevice,
		ID3D11DeviceContext* dxdevice_context,
		int window_width,
		int window_height);

	void Init() override;

	void Update(
		float dt,
		InputHandler* input_handler) override;

	void Render() override;

	void Release() override;

	void WindowResize(
		int window_width,
		int window_height) override;
	void UpdateCubes(std::vector<Cube*>);
	void UpdateBullets();
	void ClipCursorToWindow();
	void ToggleClipping(bool);
};

#endif