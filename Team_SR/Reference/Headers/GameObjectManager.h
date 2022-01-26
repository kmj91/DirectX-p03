// 게임 오브젝트 매니저
// 게임 오브젝트 프로토타입 및 복제 관리
// 참고 사항
// 게임 오브젝트를 삭제할 때 바로 삭제하는 방식이 아님 (댕글링 포인터 문제)
// Remove 플래그 확인하고 게임 로직 한곳에서 삭제합니다

#pragma once
#ifndef __GAMEOBJECTMANAGER_H__

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameObjectManager final : public CBase
{
	DECLARE_SINGLETON(CGameObjectManager)

private:
	CGameObjectManager();
	virtual ~CGameObjectManager() = default;

public:
	CGameObject* GetGameObject(_int iSceneIndex, const wstring& LayerTag, _uint iIndex = 0);
	std::list<class CGameObject*> GetGameObjects(_int iSceneIndex, const wstring& LayerTag);
public:
	// map 배열 사이즈 초기화
	HRESULT ReserveSizeContainer(_int iSceneCount);
	// 프로토타입 추가
	HRESULT AddGameObjectPrototype(_int iSceneIndex,  wstring GameObjectTag, CGameObject* pPrototype);
	// 프로토타입 복제
	HRESULT AddGameObjectInLayer(_int iFromSceneIndex, const wstring& GameObjectTag, _int iToSceneIndex, const wstring& LayerTag, CGameObject** ppGameObject = nullptr, void* pArg = nullptr);
	// 메모리 해제
	HRESULT ClearForScene(_int iSceneIndex);
	// 복제된 오브젝트만 해제
	HRESULT ClearForSceneClone(const _int iSceneIndex)&;
public:
	// 업데이트
	_uint UpdateGameObject(float fDeltaTime);
	// 레이트 업데이트
	_uint LateUpdateGameObject(float fDeltaTime);

public:
	// 2020.12.16 11:50 KMJ
	// Remove 플래그가 ON된 게임 오브젝트 삭제
	void RemoveGameObject();

private:
	// 사용하지 않음
	CGameObject* CloneGameObjectPrototype(_int iSceneIndex, const wstring& GameObjectTag, void* pArg = nullptr);

public:
	virtual void Free() override;

private:
	// 프로토타입 unordered_map
	typedef unordered_map<wstring, CGameObject*> PROTOTYPES;
	PROTOTYPES*	m_pPrototypes = nullptr;
	// 게임 오브젝트 unordered_map
	typedef unordered_map<wstring, class CLayer*>	LAYERS;
	LAYERS*	m_pLayers = nullptr;

	_int m_iSceneCount = 0;
	
};
END

#define __GAMEOBJECTMANAGER_H__
#endif

