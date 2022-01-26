// 씬 매니저
// 씬 관리 업데이트 및 레이트 업데이트 그리고 씬 교체 처리
// 참고 사항 : 씬 업데이트에서 키 처리를 하고 있음

#include "..\Headers\SceneManager.h"
#include "CollisionComponent.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
{
}

// 씬 교체
// iSceneID : 교체할 씬 번호
// pCurrentScene : 교체할 씬 객체 포인터
HRESULT CSceneManager::SetUpCurrentScene(_int iSceneID, CScene * pCurrentScene)
{
	if (nullptr == pCurrentScene)
		return E_FAIL;
	// 현재 씬 번호와 교체할 씬 번호가 다르면
	if (m_iSceneID != iSceneID)
	{
		// 현재 씬 메모리 해제
		SafeRelease(m_pCurrentScene);
		// 현재 씬으로 교체할 씬 포인터 저장
		m_pCurrentScene = pCurrentScene;
		// 현재 씬 번호 교체할 씬 번호로 저장
		m_iSceneID = iSceneID;
	}

	return S_OK;
}

_uint CSceneManager::UpdateScene(float fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->UpdateScene(fDeltaTime);
}

_uint CSceneManager::LateUpdateScene()
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->LateUpdateScene();
}

void CSceneManager::Free()
{
	SafeRelease(m_pCurrentScene);
}
