#pragma once

class Transform final {
public:
	Transform();

	~Transform();

	void SetScale(const float4& scale);

	ENGINE_API void SetRotation(const float4& rotation);

	ENGINE_API void AddRotaionX(float addRotation);

	ENGINE_API void AddRotaionY(float addRotation);

	ENGINE_API void AddRotationZ(float addRotation);

	ENGINE_API void SetPosition(const float4& position);

	ENGINE_API void AddPosition(const float4& deltaPosition);

	const float4x4& GetWorldMatrix() const;

	const float4x4 GetWorldMatrixTranspose() const;

	ENGINE_API const float4& GetPosition() const;

	ENGINE_API float4 ForwardVector() const;
	ENGINE_API float4 UpVector() const;
	ENGINE_API float4 RightVector() const;

private:
	void TransformUpdate();

	float4 scale_;
	float4 rotation_;
	float4 quaternion_;
	float4 position_;

	float4x4 scaleMatrix_;
	float4x4 rotationMatrix_;
	float4x4 positionMatrix_;
	float4x4 worldMatrix_;
};