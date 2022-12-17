#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	~Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	static Camera* GetInstance();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(float x, float y, float z);

	void StrafeRight(float value);
	void StrafeLeft(float value);
	void MoveUp(float value);
	void MoveDown(float value);
	void MoveForward(float value);
	void MoveBackward(float value);

	void LookAt(float x, float y, float z);
private:
	Camera();
	void UpdateVectors();
	void UpdateViewMatrix();

	static Camera* instance;
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMVECTOR front;
	XMVECTOR up;
	XMVECTOR right;

public:
	const XMVECTOR defaultForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR defaultUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
};