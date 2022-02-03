// 컴포넌트 인터페이스
// 컴포넌트들의 부모

#pragma once
#ifndef __COMPONENT_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CComponent abstract : public CBase
{
public:
	explicit CComponent(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CComponent() = default;

public:
	// 프로토타입 초기화
	virtual HRESULT ReadyComponentPrototype() = 0;
	// 복제 초기화
	virtual HRESULT ReadyComponent(void* pArg = nullptr) = 0;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

	static const std::wstring Tag;		// 이름에 사용할 컴포넌트 문자열 태그
protected:
	LPDIRECT3DDEVICE9	m_pDevice;		// 그래픽 디바이스
};
END

#define __COMPONENT_H__
#endif