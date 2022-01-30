// 게임 오브젝트 레이어
// 매니저에서 게임 오브젝트를 종류별로 나누는데 사용

#include "..\Headers\Layer.h"
#include "GameObject.h"

USING(Engine)

const std::wstring CLayer::Tag{L"Layer_"};

CLayer::CLayer()
{
};

CGameObject * CLayer::GetGameObject(_uint iIndex)
{
	if (m_GameObjects.size() <= iIndex)
		return nullptr;

	auto iter_begin = m_GameObjects.begin();
	for (_uint i = 0; i < iIndex; ++i, ++iter_begin);

	return *iter_begin;
}

typename CLayer::GAMEOBJECTS  CLayer::GetGameObjects()
{
	return 	m_GameObjects;
}

// 레이어에 오브젝트 추가
// 프로토타입을 복제한 오브젝트를 레이어에 저장
// pGameObject : 복제한 게임 오브젝트 포인터
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CLayer::AddGameObjectInLayer(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto iter_find = find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);
	if (m_GameObjects.end() != iter_find)
	{
		PRINT_LOG(L"Warning", L"Already exist in current layer");
		return E_FAIL;
	}

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

// 오브젝트들 업데이트
// 레이어에 있는 오브젝트들 업데이트
// fDeltaTime : 델타 타임
_uint CLayer::UpdateGameObject(float fDeltaTime)
{
	for (auto& pGameObject : m_GameObjects)
	{
		pGameObject->UpdateGameObject(fDeltaTime);
	}

	return _uint();
}

// 오브젝트들 레이트 업데이트
// 레이어에 있는 오브젝트들 레이트 업데이트
// fDeltaTime : 델타 타임
_uint CLayer::LateUpdateGameObject(float fDeltaTime)
{
	for (auto& pGameObject : m_GameObjects)
	{
		pGameObject->LateUpdateGameObject(fDeltaTime);
	}

	return _uint();
}

// 2020.12.16 11:50 KMJ
// Remove 플래그가 ON된 게임 오브젝트 삭제
void CLayer::RemoveGameObject()
{
	auto iter = m_GameObjects.begin();
	auto iter_end = m_GameObjects.end();
	while (iter != iter_end) {
		// Remove 플래그 검사
		if ((*iter)->GetOBjFlag() & static_cast<BYTE>(CGameObject::ObjFlag::Remove)) {
			SafeRelease(*iter);			// 게임 오브젝트 삭제
			iter = m_GameObjects.erase(iter);	// 이터레이터 삭제
			continue;
		}
		++iter;
	}
}

CLayer * CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
	{
		SafeRelease(pGameObject);
	}

	m_GameObjects.clear();
}
