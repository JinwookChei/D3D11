#pragma once

struct float4 {
	union {
		struct {
			float X;
			float Y;
			float Z;
			float W;
		};
		struct {
			float R;
			float G;
			float B;
			float A;
		};
		float Arr1D[4];
	};

	float4()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {
	}

	float4(float _x, float _y, float _z)
		: X(_x), Y(_y), Z(_z), W(1.0f) {
	}

	float4(float _x, float _y, float _z, float _w)
		: X(_x), Y(_y), Z(_z), W(_w) {
	}
};

struct float3 {
	union {
		struct {
			float X;
			float Y;
			float Z;
		};
		float Arr1D[3];
	};

	float3()
		: X(0.0f), Y(0.0f), Z(0.0f) {
	}

	float3(float _x, float _y, float _z)
		: X(_x), Y(_y), Z(_z) {
	}
};

struct float2 {
	union {
		struct {
			float X;
			float Y;
		};
		float Arr1D[2];
	};

	float2()
		: X(0.0f), Y(0.0f) {
	}

	float2(float _x, float _y)
		: X(_x), Y(_y) {
	}
};

typedef float4 Vector;
using Color = float4;

namespace Math
{
	const static float PIE = 3.14159265f;
	const static float DegToRadFloat = PIE / 180.0f;
	const static float RadToDegFloat = 180.0f / PIE;

	static const float4 Left = { 0.0f, -1.0f,  0.0f, 0.0f };
	static const float4 Right = { 0.0f,  1.0f,  0.0f, 0.0f };
	static const float4 Up = { 0.0f,  0.0f,  1.0f, 0.0f };
	static const float4 Down = { 0.0f,  0.0f, -1.0f, 0.0f };
	static const float4 Front = { 1.0f,  0.0f,  0.0f, 0.0f };
	static const float4 Back = { -1.0f,  0.0f,  0.0f, 0.0f };

	static const float4 DegToRad(DegToRadFloat, DegToRadFloat, DegToRadFloat, 1.0f);
}

struct __declspec(align(16)) float4x4
{
	float4 r[4];
};

SYSTEM_API void VectorDot(float& out, const float4& lhs, const float4& rhs);
SYSTEM_API void VectorCross(float4& out, const float4& lhs, const float4& rhs);
SYSTEM_API void VectorAdd(float4& out, const float4& lhs, const float4& rhs);
SYSTEM_API void VectorSub(float4& out, const float4& lhs, const float4& rhs);
SYSTEM_API void VectorMultiply(float4& out, const float4& lhs, const float4& rhs);
SYSTEM_API void VectorScale(float4& out, const float4& lhs, float scale);
SYSTEM_API void VectorNormalize(float4& out, const float4& lhs);
SYSTEM_API float VectorLength(const float4& lhs);

SYSTEM_API void VectorDot(float& out, const float3& lhs, const float3& rhs);
SYSTEM_API void VectorCross(float3& out, const float3& lhs, const float3& rhs);
SYSTEM_API void VectorAdd(float3& out, const float3& lhs, const float3& rhs);
SYSTEM_API void VectorSub(float3& out, const float3& lhs, const float3& rhs);
SYSTEM_API void VectorMultiply(float3& out, const float3& lhs, const float3& rhs);
SYSTEM_API void VectorScale(float3& out, const float3& lhs, float scale);
SYSTEM_API void VectorNormalize(float3& out, const float3& lhs);
SYSTEM_API float VectorLength(const float3& lhs);

SYSTEM_API void QuaternionToEulerDeg(float4& out, const float4& Q);
SYSTEM_API void QuaternionToEulerRad(float4& out, const float4& Q);

SYSTEM_API void MatrixIdentity(float4x4& out);
SYSTEM_API void MatrixTranspose(float4x4& out, const float4x4& src);
SYSTEM_API void MatrixCompose(float4x4& out, const float4& scale, const float4& rotAngle, const float4& pos);
SYSTEM_API void MatrixDecompose(const float4x4& matrx, float4& scale, float4& rotQ, float4& pos);
SYSTEM_API void MatrixDecomposeFromRotQ(const float4x4& matrx, float4& rotQ);
SYSTEM_API void MatrixLookToLH(float4x4& out, const float4& eyePos, const float4& eyeDir, const float4& eyeUp);
SYSTEM_API void MatrixPerspectiveFovLH(float4x4& out, float fovDegAngle, float aspectRatio, float nearZ, float farZ);

SYSTEM_API float ConvertDegToRad(float deg);
SYSTEM_API float ConvertRadToDeg(float rad);