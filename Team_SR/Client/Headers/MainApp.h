// 메인 앱
// 게임 로직의 시작 클래스

#pragma once

#ifndef __MAINAPP_H__

#include "Base.h"

USING(Engine)
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT ReadyMainApp();				// 초기화
public:
	int UpdateMainApp();				// 게임 로직 업데이트
private:
	HRESULT ReadyStaticResources();		// 리소스 로드
	HRESULT ReadyDefaultSetting();		// 디바이스 기본 설정
public:
	static CMainApp* Create();
	virtual void Free() override;
private:
	CManagement* m_pManagement = nullptr;
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};

#define __MAINAPP_H__
#endif