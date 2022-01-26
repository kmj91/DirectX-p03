// 컴포넌트 매니저
// 컴포넌트 프로토타입 관리

#pragma once
#ifndef __COMPONENTMANAGER_H__

#include "Base.h"
#include "Transform.h"
#include "VIBuffer_RectTexture.h"
//#include "VIBuffer_TerrainTexture.h"
#include "VIBuffer_CubeTexture.h"
#include "Texture.h"

BEGIN(Engine)
class CComponentManager : public CBase
{
	DECLARE_SINGLETON(CComponentManager)
private:
	CComponentManager();
	virtual ~CComponentManager() = default;
public:
	// map 배열 사이즈 초기화
	HRESULT ReserveSizePrototypeContainer(_int iSceneCount);
	// 프로토타입 컴포넌트 추가
	HRESULT AddComponentPrototype(_int iSceneIndex, const wstring& ComponentTag, CComponent* pPrototype);
	// 프로토타입 컴포넌트 복사
	CComponent* CloneComponentPrototype(_int iSceneIndex, const wstring& ComponentTag, void* pArg = nullptr);
	// 메모리 해제
	HRESULT ClearForScene(_int iSceneIndex);
public:
	virtual void Free() override;
private:
	typedef unordered_map<wstring, CComponent*> PROTOTYPES;
	PROTOTYPES*	m_pPrototypes = nullptr;
	_int m_iSceneCount = 0;
};
END

#define __COMPONENTMANAGER_H__
#endif // !

