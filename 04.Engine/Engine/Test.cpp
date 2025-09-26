#include "stdafx.h"
#include "Test.h"

bool CreateSphere(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float radius /*= 0.5f*/)
{
	if (nullptr == outVertices || nullptr == outIndices)
	{
		return false;
	}

	const int SPHERE_STACKS = 20;
	const int SPHERE_SLICES = 20;
	const float PI = 3.14159265359f;

	for (int stack = 0; stack <= SPHERE_STACKS; ++stack)
	{
		float phi = PI * stack / SPHERE_STACKS; // 0 ~ PI
		float z = cosf(phi);
		float r = sinf(phi);

		for (int slice = 0; slice <= SPHERE_SLICES; ++slice)
		{
			float theta = 2.0f * PI * slice / SPHERE_SLICES; // 0 ~ 2PI
			float x = r * cosf(theta);
			float y = r * sinf(theta);

			float3 pos = float3(x * radius, y * radius, z * radius);
			float3 normal = float3(x, y, z);
			Color color = Color(
				0.5f + 0.5f * x,
				0.5f + 0.5f * y,
				0.5f + 0.5f * z,
				1.0f
			);

			float u = (float)slice / SPHERE_SLICES;
			float v = (float)stack / SPHERE_STACKS;

			float2 uv = float2(u, v);
			outVertices->push_back({ pos, color, normal, uv });
		}
	}

	for (int stack = 0; stack < SPHERE_STACKS; ++stack)
	{
		for (int slice = 0; slice < SPHERE_SLICES; ++slice)
		{
			int first = (stack * (SPHERE_SLICES + 1)) + slice;
			int second = first + SPHERE_SLICES + 1;

			// »ï°¢Çü 1
			outIndices->push_back((WORD)first);
			outIndices->push_back((WORD)second);
			outIndices->push_back((WORD)(first + 1));

			// »ï°¢Çü 2
			outIndices->push_back((WORD)second);
			outIndices->push_back((WORD)(second + 1));
			outIndices->push_back((WORD)(first + 1));
		}
	}

	std::vector<float3> tan1(outVertices->size(), float3(0, 0, 0));
	std::vector<float3> tan2(outVertices->size(), float3(0, 0, 0));

	for (size_t n = 0; n < outIndices->size(); n += 3)
	{
		WORD i0 = (*outIndices)[n];
		WORD i1 = (*outIndices)[n + 1];
		WORD i2 = (*outIndices)[n + 2];

		const float3& p0 = (*outVertices)[i0].position;
		const float3& p1 = (*outVertices)[i1].position;
		const float3& p2 = (*outVertices)[i2].position;

		const float2& uv0 = (*outVertices)[i0].UV;
		const float2& uv1 = (*outVertices)[i1].UV;
		const float2& uv2 = (*outVertices)[i2].UV;

		float x1 = p1.X - p0.X;
		float y1 = p1.Y - p0.Y;
		float z1 = p1.Z - p0.Z;

		float x2 = p2.X - p0.X;
		float y2 = p2.Y - p0.Y;
		float z2 = p2.Z - p0.Z;

		float s1 = uv1.X - uv0.X;
		float t1 = uv1.Y - uv0.Y;

		float s2 = uv2.X - uv0.X;
		float t2 = uv2.Y - uv0.Y;

		float r = (s1 * t2 - s2 * t1);
		if (fabs(r) < 1e-6f)
		{
			r = 1.0f;
		}

		float invR = 1.0f / r;

		float3 sdir(
			(t2 * x1 - t1 * x2) * invR,
			(t2 * y1 - t1 * y2) * invR,
			(t2 * z1 - t1 * z2) * invR
		);

		float3 tdir(
			(s1 * x2 - s2 * x1) * invR,
			(s1 * y2 - s2 * y1) * invR,
			(s1 * z2 - s2 * z1) * invR
		);

		tan1[i0].X += sdir.X;
		tan1[i0].Y += sdir.Y;
		tan1[i0].Z += sdir.Z;

		tan1[i1].X += sdir.X;
		tan1[i1].Y += sdir.Y;
		tan1[i1].Z += sdir.Z;

		tan1[i2].X += sdir.X;
		tan1[i2].Y += sdir.Y;
		tan1[i2].Z += sdir.Z;

		tan2[i0].X += tdir.X;
		tan2[i0].Y += tdir.Y;
		tan2[i0].Z += tdir.Z;

		tan2[i1].X += tdir.X;
		tan2[i1].Y += tdir.Y;
		tan2[i1].Z += tdir.Z;

		tan2[i2].X += tdir.X;
		tan2[i2].Y += tdir.Y;
		tan2[i2].Z += tdir.Z;
	}

	for (size_t n = 0; n < outVertices->size(); ++n)
	{
		const float3& normal = (*outVertices)[n].normal;
		const float3& tangent = tan1[n];
		const float3& tangent2 = tan2[n];

		float dotResult = 0.0f;
		VectorDot(dotResult, normal, tangent);

		float3 scaleResult;
		VectorScale(scaleResult, normal, dotResult);

		float3 subResult;
		VectorSub(subResult, tangent, scaleResult);

		float3  finalTangent;
		VectorNormalize(finalTangent, subResult);

		float3 crossResult;
		VectorCross(crossResult, normal, tangent);

		float dotResult2 = 0.0f;
		VectorDot(dotResult2, crossResult, tangent2);

		float handedness = (dotResult2 < 0.0f) ? -1.0f : 1.0f;

		(*outVertices)[n].tangent = float4(finalTangent.X, finalTangent.Y, finalTangent.Z, handedness);

	}
	return true;
}

bool CreateRect(std::vector<MergeVertex>* outVertices, std::vector<WORD>* outIndices)
{
	if (nullptr == outVertices || nullptr == outIndices)
	{
		return false;
	}

	outVertices->resize(4);

	(*outVertices)[0] = { {-1.0f, 1.0f}, {0.0f, 0.0f} };
	(*outVertices)[1] = { {1.0f, 1.0f}, {1.0f, 0.0f} };
	(*outVertices)[2] = { {1.0f, -1.0f}, {1.0f, 1.0f} };
	(*outVertices)[3] = { {-1.0f, -1.0f}, {0.0f, 1.0f} };

	outIndices->push_back(0);
	outIndices->push_back(1);
	outIndices->push_back(2);
	outIndices->push_back(0);
	outIndices->push_back(2);
	outIndices->push_back(3);

	return true;
}
