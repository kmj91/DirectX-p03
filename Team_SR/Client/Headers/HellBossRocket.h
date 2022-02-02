// 헬보스 터보 사탄 로켓
// 맵에 부딪히면 폭발
// 몸체 가로 세로 십자 형태 2개 뒷쪽 추진기 1개 총 3개의 텍스처로 구성되어있음

#pragma once
#ifndef __HELLBOSSROCKET_H__

#include "Bullet.h"
USING(Engine)
class CHellBossRocket final : public CBullet
{
private:
	explicit CHellBossRocket(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossRocket() = default;
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
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CHellBossRocket* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	mat m_matHorizontalRot;		// 가로 회전
	mat m_matVerticalRot;		// 세로 회전
	mat m_matBackRot;			// 뒤 회전
	CTexture* m_pHorizontalTexture;	// 가로 텍스처
	CTexture* m_pVerticalTexture;	// 세로 텍스처
	CTexture* m_pBackTexture;		// 뒤 텍스처
	TRANSFORM_DESC m_pTransformVertical;	// 세로 위치
	TRANSFORM_DESC m_pTransformBack;		// 뒤 위치
};

#define __HELLBOSSROCKET_H__
#endif