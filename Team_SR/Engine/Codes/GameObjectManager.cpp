// 게임 오브젝트 매니저
// 게임 오브젝트 프로토타입 및 복제 관리
// 참고 사항
// 게임 오브젝트를 삭제할 때 바로 삭제하는 방식이 아님 (댕글링 포인터 문제)
// Remove 플래그 확인하고 게임 로직 한곳에서 삭제합니다

#include "..\Headers\GameObjectManager.h"
#include "Layer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGameObjectManager)

CGameObjectManager::CGameObjectManager()
{
}

CGameObject * CGameObjectManager::GetGameObject(_int iSceneIndex, const wstring & LayerTag, _uint iIndex)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
		return nullptr;

	auto iter_find = m_pLayers[iSceneIndex].find(LayerTag);
	if (m_pLayers[iSceneIndex].end() == iter_find)
		return nullptr;

	return iter_find->second->GetGameObject(iIndex);
}

std::list<class CGameObject*> CGameObjectManager::GetGameObjects(_int iSceneIndex, const wstring& LayerTag)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
		return {};

	auto iter_find = m_pLayers[iSceneIndex].find(LayerTag);
	if (m_pLayers[iSceneIndex].end() == iter_find)
		return {};

	return iter_find->second->GetGameObjects();
}

// map 배열 사이즈 초기화
// 씬의 개수 만큼 동적으로 자료구조의 배열 생성
// iSceneCount : 씬의 개수
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CGameObjectManager::ReserveSizeContainer(_int iSceneCount)
{
	if (0 > iSceneCount)
		return E_FAIL;

	m_iSceneCount = iSceneCount;
	// 프로토타입 배열
	m_pPrototypes = new PROTOTYPES[m_iSceneCount];
	if (nullptr == m_pPrototypes)
	{
		PRINT_LOG(L"Error", L"Failed To ReserveSizeContainer");
		return E_FAIL;
	}
	// 오브젝트 배열
	m_pLayers = new LAYERS[m_iSceneCount];
	if (nullptr == m_pLayers)
	{
		PRINT_LOG(L"Error", L"Failed To ReserveSizeContainer");
		return E_FAIL;
	}

	return S_OK;
}

// 프로토타입 추가
// iSceneIndex : 추가할 씬 번호
// GameObjectTag : 추가할 프로토타입 이름
// pPrototype : 추가할 프로토타입 객체 포인터
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CGameObjectManager::AddGameObjectPrototype(
	_int iSceneIndex, 
	wstring GameObjectTag, 
	CGameObject * pPrototype)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObject PrototypeContainer");
		return E_FAIL;
	}

	// 추가하려는 프로토타입 이름이 없어야 함
	// 이미 동일한 이름의 프로토타입이 사용중이라면 iter_find 값이 end가 아님
	auto iter_find = m_pPrototypes[iSceneIndex].find(GameObjectTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		// 프로토타입 추가
		m_pPrototypes[iSceneIndex].insert(make_pair(GameObjectTag, pPrototype));
	}

	return S_OK;
}

// 프로토타입 복제
// 프로토타입 오브젝트를 복제해서 레이어에 추가합니다.
// iFromSceneIndex : 복제할 프로토타입 씬 번호
// GameObjectTag : 복제할 프로토타입 이름
// iToSceneIndex : 저장할 씬 번호
// LayerTag : 저장할 레이어 이름
// ppGameObject : 외부로 나갈 Out 변수 복사된 객체 포인터
// pArg : 복사한 게임 오브젝트에 전달할 인수
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CGameObjectManager::AddGameObjectInLayer(
	_int iFromSceneIndex, 
	const wstring& GameObjectTag, 
	_int iToSceneIndex, 
	const wstring& LayerTag, 
	CGameObject** ppGameObject,
	void* pArg)
{
	if (0 > iFromSceneIndex ||
		m_iSceneCount <= iFromSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObjectMagager Containers");
		return E_FAIL;
	}

	if (0 > iToSceneIndex ||
		m_iSceneCount <= iToSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObjectMagager Containers");
		return E_FAIL;
	}
	// 복제할 프로토타입 찾기
	auto iter_find = m_pPrototypes[iFromSceneIndex].find(GameObjectTag);
	if (m_pPrototypes[iFromSceneIndex].end() == iter_find)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Not found %s Prototype", GameObjectTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return E_FAIL;
	}
	// 프로토타입 복제
	auto pClone = iter_find->second->Clone(pArg);
	if (nullptr == pClone)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Failed To Clone %s Prototype", GameObjectTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return E_FAIL;
	}
	// out 변수가 있으면 복제된 프로토타입 포인터 저장
	if (ppGameObject)
	{
		*ppGameObject = pClone;
		SafeAddRef(pClone);
	}
	// 복제된 오브젝트를 저장할 레이어가 없으면 새로 레이어 생성
	auto iter_find_Layer = m_pLayers[iToSceneIndex].find(LayerTag);
	if (m_pLayers[iToSceneIndex].end() == iter_find_Layer)
	{
		// 레이어 생성
		CLayer* pLayer = CLayer::Create();
		m_pLayers[iToSceneIndex].insert(make_pair(LayerTag, pLayer));
	}
	// 레이어에 복제한 오브젝트 저장
	if (FAILED(m_pLayers[iToSceneIndex][LayerTag]->AddGameObjectInLayer(pClone)))
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Failed To AddGameObjectInLayer %s Clone", LayerTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return E_FAIL;
	}

	return S_OK;
}

// 메모리 해제
// 레이어 프로토타입 모두 해제
// iSceneIndex : 메모리 해제할 씬 번호
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CGameObjectManager::ClearForScene(_int iSceneIndex)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObject PrototypeContainer");
		return E_FAIL;
	}

	for (auto& Pair : m_pLayers[iSceneIndex])
	{
		SafeRelease(Pair.second);
	}

	m_pLayers[iSceneIndex].clear();

	for (auto& Pair : m_pPrototypes[iSceneIndex])
	{
		SafeRelease(Pair.second);
	}

	m_pPrototypes[iSceneIndex].clear();

	return S_OK;
}

// 복제된 오브젝트만 해제
// 레이어만 해제
// iSceneIndex : 메모리 해제할 씬 번호
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CGameObjectManager::ClearForSceneClone(const _int iSceneIndex)&
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObject PrototypeContainer");
		return E_FAIL;
	}

	for (auto& Pair : m_pLayers[iSceneIndex])
	{
		SafeRelease(Pair.second);
	}

	m_pLayers[iSceneIndex].clear();

	return S_OK;
}

// 업데이트
// fDeltaTime : 델타타임
_uint CGameObjectManager::UpdateGameObject(float fDeltaTime)
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->UpdateGameObject(fDeltaTime);
		}
	}

	return _uint();
}

// 레이트 업데이트
// fDeltaTime : 델타타임
_uint CGameObjectManager::LateUpdateGameObject(float fDeltaTime)
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->LateUpdateGameObject(fDeltaTime);
		}
	}

	return _uint();
}

// 2020.12.16 11:50 KMJ
// Remove 플래그가 ON된 게임 오브젝트 삭제
void CGameObjectManager::RemoveGameObject()
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->RemoveGameObject();
		}
	}
}

CGameObject * CGameObjectManager::CloneGameObjectPrototype(
	_int iSceneIndex,
	const wstring & GameObjectTag,
	void * pArg)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObject PrototypeContainer");
		return nullptr;
	}

	auto iter_find = m_pPrototypes[iSceneIndex].find(GameObjectTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Not found %s Prototype", GameObjectTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return nullptr;
	}

	return iter_find->second->Clone(pArg);
}

void CGameObjectManager::Free()
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			SafeRelease(Pair.second);
		}
		m_pLayers[i].clear();
	}

	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			SafeRelease(Pair.second);
		}
		m_pPrototypes[i].clear();
	}

	SafeDeleteArray(m_pLayers);
	SafeDeleteArray(m_pPrototypes);
}
