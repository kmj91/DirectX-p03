// 헬보스 카코 데빌 스폰 볼
// 몬스터를 소환함

#pragma once
#ifndef __HELLBOSSSPAWNBALL_H__

#include "Bullet.h"
USING(Engine)
class CHellBossSpawnBall final : public CBullet
{
private:
	explicit CHellBossSpawnBall(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossSpawnBall() = default;
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
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
	void SpawnMonster();					// 몬스터 생성

private:
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CHellBossSpawnBall* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
};

#define __HELLBOSSSPAWNBALL_H__
#endif