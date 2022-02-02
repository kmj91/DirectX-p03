// 폭발
// Bullet을 상속받아서 구체 충돌함...

#pragma once
#ifndef __EXPLOSION_H__

#include "Bullet.h"
USING(Engine)
class CExplosion final : public CBullet
{
private:
	explicit CExplosion(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CExplosion() = default;
public:
	// CBullet을(를) 통해 상속됨
	// 프로토타입 초기화
	virtual HRESULT ReadyGameObjectPrototype() override;
	// 복제 초기화
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	// 업데이트
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	// 레이트 업데이트
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	// 렌더
	virtual HRESULT RenderGameObject() override;
	
public:
	// 맵에 부딪힘
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;

private:
	// 텍스처 프레임 이동
	void Frame_Move(float fDeltaTime);

private:
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CExplosion* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};

#define __EXPLOSION_H__
#endif