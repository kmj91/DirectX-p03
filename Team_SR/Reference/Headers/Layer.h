// 게임 오브젝트 레이어
// 매니저에서 게임 오브젝트를 종류별로 나누는데 사용

#pragma once
#ifndef __LAYER_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
	typedef list<class CGameObject*>	GAMEOBJECTS;
public:
	class CGameObject* GetGameObject(_uint iIndex = 0);
	GAMEOBJECTS	 GetGameObjects();
public:
	// 레이어에 오브젝트 추가
	HRESULT AddGameObjectInLayer(class CGameObject* pGameObject);
	// 오브젝트들 업데이트
	_uint UpdateGameObject(float fDeltaTime);
	// 오브젝트들 레이트 업데이트
	_uint LateUpdateGameObject(float fDeltaTime);
public:
	// 2020.12.16 11:50 KMJ
	// Remove 플래그가 ON된 게임 오브젝트 삭제
	void RemoveGameObject();

public:
	static CLayer* Create();
	virtual void Free() override;
	static const std::wstring Tag;
private:
	GAMEOBJECTS	m_GameObjects;	/* 게임 오브젝트의 클론들을 보관 */
};
END

#define __LAYER_H__
#endif