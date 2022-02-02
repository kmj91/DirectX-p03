// 포탈 웜홀
// 다음 스테이지로 넘어가기 위한 포탈
// Bullet을 상속받아서 구체 충돌함...

#pragma once
#ifndef __WORMHOLE_H__

/*
불렛을 상속받는 사실 불렛이 아님...
다음 스테이지로 넘어가기위한 포탈임
불렛의 플레이어 충돌처리를 이용해서 플레이어랑 출동 판단함
*/

#include "Bullet.h"
USING(Engine)
class CWormhole final : public CBullet
{
private:
	explicit CWormhole(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWormhole() = default;
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
	// 부모 클래스 Bullet의 호출을 막기 위함
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;

private:
	// 텍스처 프레임 이동
	void Frame_Move(float fDeltaTime);

private:
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CWormhole* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	ESceneID m_eReplaceSceneID;	// 교체할 씬 번호
};

#define __WORMHOLE_H__
#endif