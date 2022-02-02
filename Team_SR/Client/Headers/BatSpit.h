// 박쥐 침 투사체
// 회색 박쥐 몬스터의 원거리 공격 총알

#pragma once
#ifndef __BATSPIT_H__

#include "Bullet.h"
USING(Engine)
class CBatSpit final : public CBullet
{
private:
	explicit CBatSpit(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBatSpit() = default;
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
	static CBatSpit* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
};

#define __BATSPIT_H__
#endif