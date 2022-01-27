// 게임 스테이지 2 씬
// 참고 사항 : 씬 업데이트에서 키 처리를 하고 있음

#pragma once


#include "Stage.h"

USING(Engine)
class CStage2nd final : public CStage
{
private:
	explicit CStage2nd(LPDIRECT3DDEVICE9 pDevice);
public:
	using Super = CStage;
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;
protected:
	// 키 처리
	virtual _uint KeyProcess(float fDeltaTime) override;
	// 플레이어 키 처리
	virtual void PlayerKeyProcess(class CPlayer* const _CurrentPlayer, float fDeltaTime)override;
public:
	static CStage2nd* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
public:
};
