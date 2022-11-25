#include "Camera.h"
#include "MathUtils.h"

Camera* Camera::instance = nullptr;

Camera* Camera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}

	return instance;
}

Camera::Camera()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	posVector = XMLoadFloat3(&pos);
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
	UpdateVectors();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return posVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return rotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return rot;
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	posVector = pos;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	pos = XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	posVector += pos;
	XMStoreFloat3(&this->pos, posVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	rot = XMFLOAT3(x, y, z);
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	rotVector += rot;
	XMStoreFloat3(&this->rot, rotVector);
	UpdateViewMatrix();
	UpdateVectors();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	rot.x = clip(rot.x + x, -(XM_PI / 2.0f), XM_PI / 2.0f);
	rot.y = wrapAround(rot.y + y, XM_PI);
	rot.z = wrapAround(rot.z + z, XM_PI);
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
	UpdateVectors();
}

// Sugar for first person camera
void Camera::StrafeRight(float value)
{
	XMVECTOR deltaPos = XMVectorScale(right, value);
	posVector += deltaPos;
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}

void Camera::StrafeLeft(float value)
{
	posVector -= XMVectorScale(right, value);
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}

void Camera::MoveUp(float value)
{
	posVector += XMVectorScale(up, value);
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}

void Camera::MoveDown(float value)
{
	posVector -= XMVectorScale(up, value);
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}

void Camera::MoveForward(float value)
{
	posVector += XMVectorScale(front, value);
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}

void Camera::MoveBackward(float value)
{
	posVector -= XMVectorScale(front, value);
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}


void Camera::UpdateVectors()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(0.0, rot.y, rot.z);
	front = XMVector3Transform(defaultForwardVector, rotationMatrix);
	up = XMVector3Transform(defaultUpVector, rotationMatrix);
	right = XMVector3Cross(up,front);
}

void Camera::UpdateViewMatrix() 
{
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMVECTOR camTarget = XMVector3TransformCoord(defaultForwardVector, camRotationMatrix);
	camTarget += posVector;
	viewMatrix = XMMatrixLookAtLH(posVector, camTarget, up);
}