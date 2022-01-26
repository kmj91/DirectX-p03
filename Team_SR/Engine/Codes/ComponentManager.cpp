// 컴포넌트 매니저
// 컴포넌트 프로토타입 관리

#include "..\Headers\ComponentManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponentManager)

CComponentManager::CComponentManager()
{
}

// map 배열 사이즈 초기화
// 씬의 개수 만큼 동적으로 자료구조의 배열 생성
// iSceneCount : 씬의 개수
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CComponentManager::ReserveSizePrototypeContainer(_int iSceneCount)
{
	if (0 > iSceneCount)
		return E_FAIL;
	// 프로토타입 배열
	m_iSceneCount = iSceneCount;
	m_pPrototypes = new PROTOTYPES[m_iSceneCount];

	if (nullptr == m_pPrototypes)
	{
		PRINT_LOG(L"Error", L"Failed To ReserveSizePrototypeContainer");
		return E_FAIL;
	}

	return S_OK;
}

// 프로토타입 컴포넌트 추가
// iSceneIndex : 추가할 씬 번호
// ComponentTag : 추가할 프로토타입 이름
// pPrototype : 추가할 프로토타입 객체 포인터
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CComponentManager::AddComponentPrototype(
	_int iSceneIndex, 
	const wstring & ComponentTag, 
	CComponent * pPrototype)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range Component PrototypeContainer");
		return E_FAIL;
	}

	auto iter_find = m_pPrototypes[iSceneIndex].find(ComponentTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		m_pPrototypes[iSceneIndex].insert(make_pair(ComponentTag, pPrototype));
	}


	return S_OK;
}

// 프로토타입 컴포넌트 복사
// iSceneIndex : 복사할 씬 번호
// ComponentTag : 복사할 컴포넌트 프로토타입 이름
// pArg : 복사할 컴포넌트에 전달할 인수
// 반환 값 : 프로토타입을 복사한 컴포넌트 객체 포인터
CComponent* CComponentManager::CloneComponentPrototype(
	_int iSceneIndex, 
	const wstring & ComponentTag, 
	void* pArg)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range Component PrototypeContainer");
		return nullptr;
	}
	// 복사할 컴포넌트 프로토타입 찾기
	auto iter_find = m_pPrototypes[iSceneIndex].find(ComponentTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Not found %s Prototype", ComponentTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return nullptr;
	}
	// 프로토타입 복제 반환
	return iter_find->second->Clone(pArg);
}

// 메모리 해제
// iSceneIndex : 메모리 해제할 씬 번호
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CComponentManager::ClearForScene(_int iSceneIndex)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range Component PrototypeContainer");
		return E_FAIL;
	}

	for (auto& Pair : m_pPrototypes[iSceneIndex])
	{
		SafeRelease(Pair.second);
	}
	m_pPrototypes[iSceneIndex].clear();

	return S_OK;
};

void CComponentManager::Free()
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			SafeRelease(Pair.second);
		}
		m_pPrototypes[i].clear();
	}

	SafeDeleteArray(m_pPrototypes);	
}
