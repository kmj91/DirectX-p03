// 트랜스폼 컴포넌트

#pragma once
#ifndef __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTransform final : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTransform() = default;

public:
	// CComponent을(를) 통해 상속됨
	// 컴포넌트 프로토타입 초기화
	virtual HRESULT ReadyComponentPrototype() override;
	// 컴포넌트 복제 초기화
	virtual HRESULT ReadyComponent(void * pArg = nullptr) override;

public:
	// 업데이트 트랜스폼
	HRESULT UpdateTransform();
	void GoStraight(float fDeltaTime);
	void GoSide(float fDeltaTime);
	void AddRotationX(float fDelaTime);
	void AddRotationY(float fDelaTime);
	void AddRotationZ(float fDelaTime);

	FORCEINLINE void SetLocation(const vec3 Location) { m_TransformDesc.vPosition = Location; };
	FORCEINLINE vec3 GetLocation() { return m_TransformDesc.vPosition; }
	FORCEINLINE vec3 GetScale() { return m_TransformDesc.vScale; };
	FORCEINLINE vec3 GetRotation() { return m_TransformDesc.vRotation; };
	vec3 GetLook()const&;
	vec3 GetRight()const&;
	vec3 GetUp()const&;
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public:
	TRANSFORM_DESC	m_TransformDesc;	// 트랜스폼
};
END

#define __TRANSFORM_H__
#endif
