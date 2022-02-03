
//
//  Camera.h
//
//	Basic camera class
//

#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#define min(a,b)            (((a) < (b)) ? (a) : (b))

#include "vec\vec.h"
#include "vec\mat.h"
#include "InputHandler.h"
#include <vector>

using namespace linalg;

class Camera
{

public:
	// Aperture attributes
	float vfov, aspect;
	float mousedX;
	float mousedY;
	float currentPrePosIndex;
	
	// Clip planes in view space coordinates
	// Evrything outside of [zNear, zFar] is clipped away on the GPU side
	// zNear should be > 0
	// zFar should depend on the size of the scene
	// This range should be kept as tight as possibly to improve
	// numerical precision in the z-buffer
	float zNear, zFar;	
	
	bool hasMoved;
	vec3f position;
	mat4f rotation;
	float rotationSpeed;
	mat4f viewToWorld;
	mat4f worldToView;

	std::vector<std::pair<vec3f, mat4f>> prePositions;

	Camera(float vfov, float aspect, float zNear, float zFar);
	void moveTo(const vec3f& p); //Move to absolute point
	void move(const vec3f& v); //Move relatively
	void Rotate(float x, float y); //Rotate camera with input from mouse
	void ChangeFoV(std::string zoom);
	void ChangePrePos(int indexChange);
	mat4f GetRotation(); //return rotation
	vec3f GetPosition();
	mat4f Get_WorldToViewMatrix(); //Return World-to-View matrix for this camera
	mat4f GetViewToWorldMatrix();
	void SaveView();
	//bool HasMoved();
	//void SetHasMoved(bool state);

	// Matrix transforming from View space to Clip space
	// In a performance sensitive situation this matrix should be precomputed
	// if possible
	mat4f get_ProjectionMatrix();
};

#endif