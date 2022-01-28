// 게임 스테이지 1 씬
// 참고 사항 : 씬 업데이트에서 키 처리를 하고 있음

#pragma once
#ifndef __STAGE1ST_H__

#include "Stage.h"

USING(Engine)
class CStage1st final : public CStage
{
private:
	explicit CStage1st(LPDIRECT3DDEVICE9 pDevice);
public:
	using Super = CStage;
	// 씬 초기화
	virtual HRESULT ReadyScene() override;
	// 업데이트
	virtual _uint UpdateScene(float fDeltaTime) override;
	// 레이트 업데이트
	virtual _uint LateUpdateScene() override;
protected:
	// 키 처리
	virtual _uint KeyProcess(float fDeltaTime) override;
	// 플레이어 키 처리
	virtual void PlayerKeyProcess(class CPlayer* const _CurrentPlayer, float fDeltaTime)override;
public:
	static CStage1st* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
public:
};

#define __STAGE1ST_H__
#endif
