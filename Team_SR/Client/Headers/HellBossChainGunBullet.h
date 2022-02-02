// 헬보스 터보 사탄 체인건 총알

#pragma once
#ifndef __HELLBOSSCHAINGUNBULLET_H__

#include "Bullet.h"
USING(Engine)
class CHellBossChainGunBullet final : public CBullet
{
private:
	explicit CHellBossChainGunBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossChainGunBullet() = default;
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

private:
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CHellBossChainGunBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	mat m_matRot;
};

#define __HELLBOSSCHAINGUNBULLET_H__
#endif