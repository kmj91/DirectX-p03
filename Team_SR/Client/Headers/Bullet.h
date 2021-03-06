// 총알 인터페이스
// 투사체들의 부모 클래스

#pragma once
#ifndef __BULLET_H__

#include "GameObject.h"
USING(Engine)
class CBullet abstract : public CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBullet() = default;
public:
	// CGameObject을(를) 통해 상속됨
	// 프로토타입 초기화
	virtual HRESULT ReadyGameObjectPrototype() = 0;
	// 복제 초기화
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0;
	// 업데이트
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	// 레이트 업데이트
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	// 렌더
	virtual HRESULT RenderGameObject() = 0;
public:
	// 맵에 충돌함
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;
	// 공격 충돌 처리 검사
	bool Bullet_Attack(float _fAttack);
	// 플레이어 충돌체와 검사
	bool Attack(const Sphere _Sphere, const float Attack) &;
protected:
	// 컴포넌트 추가
	virtual HRESULT AddComponents();
protected:
	// 텍스처 프레임 이동
	void Frame_Move(float fDeltaTime);
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
protected:
	float m_fFrameCnt;		// 프레임 번호
	float m_fStartFrame;	// 프레임 시작
	float m_fEndFrame;		// 프레임 끝
	float m_fFrameSpeed;	// 프레임 속도
	vec3 m_vLook;			// 방향
	CTexture* m_pTexture;	// 텍스처
	BulletStatus m_stOriginStatus;	// 총알 원본 스텟
	BulletStatus m_stStatus;		// 총알 스텟
	bool m_bOneHit = false;
	_uint m_uiCountRelay;		// 특정 총알 전용 전달 카운트 변수
public:
	float Shine = 20.f;
	class CNormalUVVertexBuffer * _VertexBuffer{ nullptr };
};

#define  __BULLET_H__
#endif
