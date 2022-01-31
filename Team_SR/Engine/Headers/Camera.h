// 카메라 인터페이스
// 게임에 사용할 View 카메라
// Fov, Near, Far에 대한 정보를 가지고 있습니다

#pragma once
#ifndef __CAMERA_H__

#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CCamera abstract : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCamera() = default;

public:
	const CAMERA_DESC& GetCameraDesc() const;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() = 0;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) = 0;
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;

public:
	virtual CGameObject * Clone(void * pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	CAMERA_DESC	m_CameraDesc;
};
END

#define __CAMERA_H__
#endif