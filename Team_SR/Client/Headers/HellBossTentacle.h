// 헬보스 몰락한 군주 촉수
// 정해진 방향으로 땅속에서 나오면서 뻗어나감

#pragma once
#ifndef __HELLBOSSTENTACLE_H__

#include "Bullet.h"
USING(Engine)
class CHellBossTentacle final : public CBullet
{
private:
	explicit CHellBossTentacle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossTentacle() = default;
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

private:
	// 텍스처 프레임 이동
	void Frame_Move(float fDeltaTime);

private:
	// 컴포넌트 추가
	virtual HRESULT AddComponents() override;

public:
	static CHellBossTentacle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	bool bRelayFlag;	// 전달 여부
};

#define __HELLBOSSTENTACLE_H__
#endif