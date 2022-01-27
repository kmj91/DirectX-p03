// 게임 스테이지 4 씬
// 사용하지 않음...
// 스테이지가 너무 많아서 삭제됨

#pragma once


#include "Stage.h"

USING(Engine)
class CStage4th final : public CStage
{
private:
	explicit CStage4th(LPDIRECT3DDEVICE9 pDevice);
public:
	using Super = CStage;
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;
protected:
	virtual _uint KeyProcess(float fDeltaTime) override;
	virtual void PlayerKeyProcess(class CPlayer* const _CurrentPlayer, float fDeltaTime)override;
public:
	static CStage4th* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
public:
};
