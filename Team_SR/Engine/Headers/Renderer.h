// 렌더 매니저
// 게임 오브젝트를 렌더 순서에 따라 처리

#pragma once
#ifndef __RENDERER_H__

#include "Base.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CRenderer() = default;

public:
	// 렌더 리스트에 게임 오브젝트 추가
	HRESULT AddGameObjectInRenderer(ERenderID eID, class CGameObject* pGameObject, ERenderPlace ePlace);
	// 렌더
	HRESULT Render(HWND hWnd = nullptr);
	void RegistLight(const D3DLIGHT9& Light);
	void SetAmbient(const DWORD Ambient) { this->Ambient = Ambient; };
	D3DCAPS9 GetCaps() { return _Caps9; };
private:
	// 우선 순위 렌더할 오브젝트
	HRESULT RenderPriority();
	// 알파가 없는 오브젝트
	HRESULT RenderNoAlpha();
	// 알파가 들어가는 오브젝트
	HRESULT RenderAlpha();
	// UI
	HRESULT RenderUI();
	HRESULT RenderScreenPostEffect();
public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
public:
	D3DCAPS9 _Caps9;
	size_t MaxTexState = 8;
	std::function<void()> _ParticleRender;
private:
	DWORD Ambient = 0x00202020;
	LPDIRECT3DDEVICE9	m_pDevice;
	typedef list<class CGameObject*> GAMEOBJECTS;
	GAMEOBJECTS	m_GameObjects[(_uint)ERenderID::MaxCount];
};
END

#define __RENDERER_H__
#endif