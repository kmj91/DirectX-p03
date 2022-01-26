// 씬 매니저
// 씬 관리 업데이트 및 레이트 업데이트 그리고 씬 교체 처리
// 참고 사항 : 씬 업데이트에서 키 처리를 하고 있음

#pragma once
#ifndef __SCENEMANAGER_H__

#include "Base.h"
#include "Scene.h"

BEGIN(Engine)
class CSceneManager final : public CBase
{
	DECLARE_SINGLETON(CSceneManager)

private:
	CSceneManager();
	virtual ~CSceneManager()= default;

public:
	// 씬 교체
	HRESULT SetUpCurrentScene(_int iSceneID, CScene* pCurrentScene);
	_uint UpdateScene(float fDeltaTime);
	_uint LateUpdateScene();
	FORCEINLINE CScene* GetCurrentScene() { return m_pCurrentScene; };
public:
	virtual void Free() override;

private:
	CScene*	m_pCurrentScene = nullptr;
	_int  m_iSceneID = -1;
};
END

#define __SCENEMANAGER_H__
#endif
