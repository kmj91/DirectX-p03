// 로고 씬
// 사실 아무런 기능이 없음...
// 검은화면에서 엔터를 누르면 게임 스테이지로 넘어감

#pragma once
#ifndef __LOGO_H__

#include "Scene.h"

USING(Engine)
class CLogo : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo() = default;

public:
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;

protected:
	virtual _uint KeyProcess(float fDeltaTime) override;

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

};

#define __LOGO_H__
#endif
