// 게임 스테이지 씬 인터페이스
// 게임 스테이지의 부모
// 상속받은 스테이지에서 LoadObjects 함수를 호출하여 게임 오브젝트들을 생성함

#pragma once
#ifndef __STAGE_H__

#include "Scene.h"
#include "JumpPointSearch.h"

USING(Engine)
class CStage abstract : public CScene
{
protected:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage() = default;
public:
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;
protected:
	// 키 처리
	virtual _uint KeyProcess(float fDeltaTime) override;
	// 플레이어 키 처리
	virtual void PlayerKeyProcess(class CPlayer*const _CurrentPlayer,float fDeltaTime);
public:
	virtual void Free() override;
public:
	class CPlayer* m_pPlayer = nullptr;
	class CMainCamera* _Camera{ nullptr };
	class CMapBase* _CurrentMap{ nullptr };
protected:
	ESceneID CurrentSceneID;
	ESceneID NextSceneID;
	std::wstring BgmKey{};
protected:
	// 오브젝트 생성 정보 파일 로드
	void LoadObjects(const std::wstring& FilePath, const vec3 WorldScale) & noexcept;
	// 오브젝트 생성
	void SpawnObjectFromName(const std::wstring& ObjectName, vec3 SpawnLocation) & noexcept;
	class CUIManager* m_pUIManager;
};

#define __STAGE_H__
#endif
