#ifndef __DIRECTXHELPER_HPP__
#define __DIRECTXHELPER_HPP__

#include <DirectXMath.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#endif

static bool	operator==(DirectX::XMUINT3 const& first, DirectX::XMUINT3 const& second)
{
	return (first.x == second.x && second.y == first.y && first.z == second.z);
}

static DirectX::XMFLOAT3	operator+(DirectX::XMFLOAT3 const& first, DirectX::XMFLOAT3 const& second)
{
	return DirectX::XMFLOAT3(first.x + second.x, first.y + second.y, first.z + second.z);
}

static DirectX::XMFLOAT3	operator-(DirectX::XMFLOAT3 const& first, DirectX::XMFLOAT3 const& second)
{
	return DirectX::XMFLOAT3(first.x - second.x, first.y - second.y, first.z - second.z);
}

static	DirectX::XMFLOAT3	EulerAnglesRad(DirectX::XMFLOAT4 const& quat)
{
	DirectX::XMFLOAT3 euler;
	euler.x = atan2f((2.0f * (quat.y * quat.z + quat.w * quat.x)), (quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z));
	euler.y = asin(-2.0f * (quat.x * quat.z - quat.w * quat.y));
	euler.z = atan2f(2.0f * (quat.x * quat.y + quat.w * quat.z), (quat.w * quat.w + quat.x * quat.x - quat.y * quat.y - quat.z * quat.z));
	return euler;
}

static	DirectX::XMFLOAT3	EulerAnglesDeg(DirectX::XMFLOAT4 const& quat)
{
	DirectX::XMFLOAT3 euler;
	euler.x = atan2f((2.0f * (quat.y * quat.z + quat.w * quat.x)), (quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z));
	euler.y = asin(-2.0f * (quat.x * quat.z - quat.w * quat.y));
	euler.z = atan2f(2.0f * (quat.x * quat.y + quat.w * quat.z), (quat.w * quat.w + quat.x * quat.x - quat.y * quat.y - quat.z * quat.z));
	euler.x = DirectX::XMConvertToDegrees(euler.x);
	euler.y = DirectX::XMConvertToDegrees(euler.y);
	euler.z = DirectX::XMConvertToDegrees(euler.z);
	return euler;
}

static	DirectX::XMFLOAT3	Vector3ToRad(DirectX::XMFLOAT3 const& vector)
{
	DirectX::XMFLOAT3	ret;
	ret.x = DirectX::XMConvertToRadians(vector.x);
	ret.y = DirectX::XMConvertToRadians(vector.y);
	ret.z = DirectX::XMConvertToRadians(vector.z);
	return ret;
}

#endif /*__DIRECTXHELPER_HPP__*/