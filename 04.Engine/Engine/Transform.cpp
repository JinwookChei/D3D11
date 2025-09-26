#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
	: scale_(1, 1, 1, 0),
	rotation_(0, 0, 0, 0),
	position_(0, 0, 0, 1),
	quaternion_(0, 0, 0, 1)
{
	MatrixIdentity(scaleMatrix_);
	MatrixIdentity(rotationMatrix_);
	MatrixIdentity(positionMatrix_);
	MatrixIdentity(worldMatrix_);
}

Transform::~Transform()
{
}

void Transform::SetScale(const float4& scale)
{
	scale_ = scale;

	TransformUpdate();
}

void Transform::SetRotation(const float4& rotation)
{
	rotation_ = rotation;

	TransformUpdate();
}

void Transform::AddRotaionX(float addRotation)
{
	rotation_.X += addRotation;

	TransformUpdate();
}

void Transform::AddRotaionY(float addRotation)
{
	rotation_.Y += addRotation;

	TransformUpdate();
}

void Transform::AddRotationZ(float addRotation)
{
	rotation_.Z += addRotation;

	TransformUpdate();
}

void Transform::SetPosition(const float4& position)
{
	position_ = position;

	TransformUpdate();
}

void Transform::AddPosition(const float4& deltaPosition)
{
	VectorAdd(position_, position_, deltaPosition);

	TransformUpdate();
}

const float4x4& Transform::GetWorldMatrix() const
{
	return worldMatrix_;
}

const float4x4 Transform::GetWorldMatrixTranspose() const
{
	float4x4 ret;
	MatrixTranspose(ret, worldMatrix_);
	return ret;
}

const float4& Transform::GetPosition() const
{
	return position_;
}

float4 Transform::ForwardVector() const
{
	float4 normal;
	VectorNormalize(normal, worldMatrix_.r[0]);
	return normal;
}

float4 Transform::UpVector() const
{
	float4 normal;
	VectorNormalize(normal, worldMatrix_.r[2]);
	return normal;
}

float4 Transform::RightVector() const
{
	float4 normal;
	VectorNormalize(normal, worldMatrix_.r[1]);
	return normal;
}

void Transform::TransformUpdate()
{
	MatrixCompose(worldMatrix_, scale_, rotation_, position_);

	/*float4 roQ;
	MatrixDecomposeFromRotQ(worldMatrix_, roQ);

	float4 roDeg;
	QuaternionToEulerDeg(roDeg, roQ);

	float4x4 tmpMatrix;
	MatrixCompose(tmpMatrix, scale_, roDeg, position_);*/
}
