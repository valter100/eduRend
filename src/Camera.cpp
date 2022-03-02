#pragma once

#include "Camera.h"
#include <algorithm>
#include <iostream>

Camera::Camera(float vfov, float aspect, float zNear, float zFar) : vfov(vfov), aspect(aspect), zNear(zNear), zFar(zFar)
{
	rotationSpeed = 0.003; //fast
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 1.0f, 1.0f, 1.0f, 1 };
	this->zNear = zNear;
	this->zFar = zFar;
	mousedX = 0;
	mousedY = 0;
	currentPrePosIndex = 0;

	prePositions.push_back(std::make_pair(position, rotation));

	prePositions.push_back(std::make_pair(vec3f(6.2f, 29.4f, -49.0f),
		mat4f::rotation(3.0f, -0.0f, 1.0f, 0.2f)));

	prePositions.push_back(std::make_pair(vec3f(8.44f, 11.04f, 23.20f),
		mat4f::rotation(1.0f, -0.15f, 0.2f, 0.0f)));

	prePositions.push_back(std::make_pair(vec3f(-31.0f, 10.0f, 70.20f),
		mat4f::rotation(1.0f, -0.15f, -0.5f, 0.0f)));
}

void Camera::moveTo(const vec3f& p)
{
	position = p;
}

void Camera::move(const vec3f& v)
{
	position += v * rotation.get_3x3().inverse();
}

void Camera::Rotate(float x, float y)
{
	mousedX += x * rotationSpeed;
	if (mousedX > PI * 2)
		mousedX = 0;
	else if (mousedX < -PI * 2)
		mousedX = 0;

	mousedY += y * rotationSpeed;
	if (mousedY < -PI/5)
		mousedY = -PI/5;
	else if (mousedY > PI/5)
		mousedY = PI/5;
	rotation = mat4f::rotation(0, -mousedX, -mousedY);
}

void Camera::ChangeFoV(std::string zoom)
{
	if (zoom == "in")
		vfov -= 0.001;
	else if (zoom == "out")
		vfov += 0.001;
}

void Camera::ChangePrePos(int indexChange)
{
	SaveView();

	if (currentPrePosIndex + indexChange >= prePositions.size())
	{
		currentPrePosIndex = 0;
	}
	else if (currentPrePosIndex + indexChange < 0)
	{
		currentPrePosIndex = prePositions.size() - 1;
	}
	else
	{
		currentPrePosIndex += indexChange;
	}

	position = prePositions[currentPrePosIndex].first;
	rotation = prePositions[currentPrePosIndex].second;
}

mat4f Camera::GetRotation()
{
	return rotation;
}

vec3f Camera::GetPosition()
{
	return position;
}

mat4f Camera::Get_WorldToViewMatrix()
{
	viewToWorld = mat4f::translation(position) * rotation * mat4f::scaling(1);

	worldToView = viewToWorld.inverse();

	return worldToView;
}

mat4f Camera::GetViewToWorldMatrix()
{
	return mat4f::translation(position) * rotation * mat4f::scaling(1);
}

mat4f Camera::get_ProjectionMatrix()
{
	return mat4f::projection(vfov, aspect, zNear, zFar);
}

void Camera::SaveView()
{
	prePositions[currentPrePosIndex] = std::make_pair(position, rotation);
}
