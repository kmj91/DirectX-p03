// 헬보스 터보 사탄 눈알 빔
// 맵에 부딪히면 폭발

#pragma once
#ifndef __HELLBOSSEYEBLAST_H__

#include "Bullet.h"
USING(Engine)
class CHellBossEyeBlast final : public CBullet
{
private:
	explicit CHellBossEyeBlast(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossEyeBlast() = default;
public:
	// CMonster을(를) 통해 상속됨
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
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CHellBossEyeBlast* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	mat m_matRot;
};

#define __HELLBOSSEYEBLAST_H__
#endif