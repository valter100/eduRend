
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

	std::vector<Model*> models;
	Texture cubeTexture;

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

	ID3D11SamplerState* samplerState = nullptr;
	ID3D11SamplerState* samplerState2 = nullptr;
	ID3D11SamplerState* samplerState3 = nullptr;
	ID3D11SamplerState* samplerState4 = nullptr;
	ID3D11SamplerState* samplerState5 = nullptr;
	ID3D11SamplerState* samplerState6 = nullptr;
	std::vector< ID3D11SamplerState*> samplerStates;
	float samplerStateIndex = 0;


	struct TransformationBuffer
	{
		mat4f ModelToWorldMatrix;
		mat4f WorldToViewMatrix;
		mat4f ProjectionMatrix;
	};

	struct CameraAndLightBuffer
	{
		vec4f CamPosition;
		vec4f LightPositions[2];
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
	Light* lightSources[1];

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
	OBJModel* skybox;

	//CompositeObject Model-toworld transformation matrices
	mat4f MBush;
	mat4f MCharacter;
	mat4f MTrain;
	mat4f MGun;
	mat4f MSphere;
	mat4f MSkybox;

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
	float camera_vel = 15.0f;	// Camera movement velocity in units/s
	float fps_cooldown = 0;
	bool isPressingKey = 0;
	bool cursorIsShowing = true;
	void InitTransformationBuffer();
	void InitCameraAndLightBuffer();
	void CreateSamplerState();
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