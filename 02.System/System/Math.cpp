#include "stdafx.h"
#include "Math.h"

void VectorDot(float& out, const float4& lhs, const float4& rhs)
{
	__m128 result = DirectX::XMVector3Dot(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	out = result.m128_f32[0];
}

void VectorCross(float4& out, const float4& lhs, const float4& rhs)
{
	__m128 result = DirectX::XMVector3Cross(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
}

void VectorAdd(float4& out, const float4& lhs, const float4& rhs)
{
	__m128 result = _mm_add_ps(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
	if (0 != out.W)
	{
		out.W = 1.0f;
	}
}

void VectorSub(float4& out, const float4& lhs, const float4& rhs)
{
	__m128 result = _mm_sub_ps(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
	if (lhs.W != 0)
	{
		out.W = 1.0f;
	}
}

void VectorMultiply(float4& out, const float4& lhs, const float4& rhs)
{
	__m128 result = _mm_mul_ps(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
	if (0 != out.W)
	{
		out.W = 1.0f;
	}
}

void VectorScale(float4& out, const float4& lhs, float scale)
{
	__m128 vResult = _mm_set_ps1(scale);
	__m128 result = _mm_mul_ps(vResult, _mm_loadu_ps(&lhs.X));
	_mm_storeu_ps(&out.X, result);
	if (0 != out.W)
	{
		out.W = 1.0f;
	}
}

void VectorNormalize(float4& out, const float4& lhs)
{
	__m128 result = DirectX::XMVector3Normalize(_mm_loadu_ps(&lhs.X));
	_mm_storeu_ps(&out.X, result);
}

float VectorLength(const float4& lhs)
{
	__m128 result = DirectX::XMVector3Length(_mm_loadu_ps(&lhs.X));
	return result.m128_f32[0];
}


static __m128 LoadFloat3(const float3& value)
{
	__m128 xy = _mm_castpd_ps(_mm_load_sd((const double*)&value));
	__m128 z = _mm_load_ss(&value.Z);
	return _mm_movelh_ps(xy, z);
}

static void StoreFloat3(const __m128& value, float3& dest)
{
	_mm_store_sd((double*)&dest, _mm_castps_pd(value));
	__m128 z = XM_PERMUTE_PS(value, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(&dest.Z, z);
}

void VectorDot(float& out, const float3& lhs, const float3& rhs)
{
	__m128 result = DirectX::XMVector3Dot(LoadFloat3(lhs), LoadFloat3(rhs));
	out = result.m128_f32[0];
}

void VectorCross(float3& out, const float3& lhs, const float3& rhs)
{
	__m128 result = DirectX::XMVector3Cross(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void VectorAdd(float3& out, const float3& lhs, const float3& rhs)
{
	__m128 result = _mm_add_ps(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void VectorSub(float3& out, const float3& lhs, const float3& rhs)
{
	__m128 result = _mm_sub_ps(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void VectorMultiply(float3& out, const float3& lhs, const float3& rhs)
{
	__m128 result = _mm_mul_ps(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void VectorScale(float3& out, const float3& lhs, float scale)
{
	__m128 vResult = _mm_set_ps1(scale);
	__m128 result = _mm_mul_ps(vResult, LoadFloat3(lhs));
	StoreFloat3(result, out);
}

void VectorNormalize(float3& out, const float3& lhs)
{
	__m128 result = DirectX::XMVector3Normalize(LoadFloat3(lhs));
	StoreFloat3(result, out);
}

float VectorLength(const float3& lhs)
{
	__m128 result = DirectX::XMVector3Length(LoadFloat3(lhs));
	return result.m128_f32[0];
}

void QuaternionToEulerDeg(float4& out, const float4& Q)
{
	float4 rad;
	QuaternionToEulerRad(rad, Q);
	VectorMultiply(out, rad, float4(Math::RadToDegFloat, Math::RadToDegFloat, Math::RadToDegFloat, 1.0f));
}

void QuaternionToEulerRad(float4& out, const float4& Q)
{
	float sinrCosp = 2.0f * (Q.W * Q.Z + Q.X * Q.Y);
	float cosrCosp = 1.0f - 2.0f * (Q.Z * Q.Z + Q.X * Q.X);
	out.Z = atan2f(sinrCosp, cosrCosp);

	float pitchTest = Q.W * Q.X - Q.Y * Q.Z;
	float asinThreshold = 0.4999995f;
	float sinp = 2.0f * pitchTest;

	if (pitchTest < -asinThreshold)
	{
		out.X = -(0.5f * Math::PIE);
	}
	else if (pitchTest > asinThreshold)
	{
		out.X = (0.5f * Math::PIE);
	}
	else
	{
		out.X = asinf(sinp);
	}

	float sinyCosp = 2.0f * (Q.W * Q.Y + Q.X * Q.Z);
	float cosyCosp = 1.0f - 2.0f * (Q.X * Q.X + Q.Y * Q.Y);
	out.Y = atan2f(sinyCosp, cosyCosp);
}

void QuaternionRotaionRollPitchYaw(float4& outQ, const float4& angle)
{
	__m128 result = DirectX::XMQuaternionRotationRollPitchYawFromVector(_mm_loadu_ps(&angle.X));
	_mm_storeu_ps(&outQ.X, result);
}

void MatrixIdentity(float4x4& out)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	memcpy_s(&out, sizeof(float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MatrixTranspose(float4x4& out, const float4x4& src)
{
	DirectX::XMMATRIX srcMatrix;
	memcpy_s(&srcMatrix, sizeof(DirectX::XMMATRIX), &src, sizeof(float4x4));

	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranspose(srcMatrix);

	memcpy_s(&out, sizeof(float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MatrixCompose(float4x4& out, const float4& scale, const float4& rotAngle, const float4& pos)
{
	__m128 convertRotAngle = _mm_loadu_ps(&rotAngle.X);
	__m128 quaternionVectorOrigin = DirectX::XMQuaternionRotationRollPitchYawFromVector(convertRotAngle);

	__m128 rotationVector = _mm_mul_ps(convertRotAngle, _mm_loadu_ps(&Math::DegToRad.X));
	__m128 quaternionVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	DirectX::XMMATRIX matrix = DirectX::XMMatrixAffineTransformation(_mm_loadu_ps(&scale.X), quaternionVectorOrigin, quaternionVector, _mm_loadu_ps(&pos.X));
	memcpy_s(&out, sizeof(float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MatrixDecompose(const float4x4& matrx, float4& scale, float4& rotQ, float4& pos)
{
	DirectX::XMMATRIX srcMatrix;
	memcpy_s(&srcMatrix, sizeof(DirectX::XMMATRIX), &matrx, sizeof(float4x4));
	DirectX::XMVECTOR outScale;
	DirectX::XMVECTOR outQ;
	DirectX::XMVECTOR outPos;
	DirectX::XMMatrixDecompose(&outScale, &outQ, &outPos, srcMatrix);
	_mm_storeu_ps(&scale.X, outScale);
	_mm_storeu_ps(&rotQ.X, outQ);
	_mm_storeu_ps(&pos.X, outPos);
}

void MatrixDecomposeFromRotQ(const float4x4& matrx, float4& rotQ)
{
	float4 tmpScale;
	float4 tmpPos;
	MatrixDecompose(matrx, tmpScale, rotQ, tmpPos);
}

void MatrixLookToLH(float4x4& out, const float4& eyePos, const float4& eyeDir, const float4& eyeUp)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixLookToLH(_mm_loadu_ps(&eyePos.X), _mm_loadu_ps(&eyeDir.X), _mm_loadu_ps(&eyeUp.X));
	memcpy_s(&out, sizeof(float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MatrixPerspectiveFovLH(float4x4& out, float fovDegAngle, float aspectRatio, float nearZ, float farZ)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixPerspectiveFovLH(fovDegAngle * Math::DegToRadFloat, aspectRatio, nearZ, farZ);
	memcpy_s(&out, sizeof(float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

float ConvertDegToRad(float deg)
{
	return deg * Math::DegToRadFloat;
}

float ConvertRadToDeg(float rad)
{
	return rad * Math::RadToDegFloat;
}
