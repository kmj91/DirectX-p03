// 헬보스 리틀 데몬 링 총알

#pragma once
#ifndef __HELLBOSSRINGBULLET_H__

#include "Bullet.h"
USING(Engine)
class CHellBossRingBullet final : public CBullet
{
private:
	explicit CHellBossRingBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossRingBullet() = default;
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
	// 텍스처 프레임 이동
	void Frame_Move(float fDeltaTime);

private:
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CHellBossRingBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
};

#define __HELLBOSSRINGBULLET_H__
#endif