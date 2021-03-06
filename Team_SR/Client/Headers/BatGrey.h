// 몬스터 회색 박쥐
// 원거리 공격 근접 공격을 합니다.
// 체력이 없으면 도망을 가기도 합니다.

#pragma once
#ifndef __BATGREY_H__

#include "Monster.h"
USING(Engine)
class CBatGrey final : public CMonster
{
private:
	explicit CBatGrey(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBatGrey() = default;
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
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo) override;	// 몬스터가 피해를 받음
	virtual void ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)override;
	virtual void FreezeHit() override;

private:
	void Update_AI(float fDeltaTime);		// 업데이트 AI

	void AI_NoAwareness();					// 플레이어를 인식하지 못함
	void AI_ActiveOffense();				// 적극적으로 공격
	void AI_PassiveOffense();				// 소극적으로 공격

	bool Action_Idle(float fDeltaTime);		// 행동 대기
	bool Action_Move(float fDeltaTime);		// 이동
	bool Action_Shoot(float fDeltaTime);	// 원거리 공격
	bool Action_Melee(float fDeltaTime);	// 근접 공격
	bool Action_Hit(float fDeltaTime);		// 공격받아서 경직
	bool Action_Dead(float fDeltaTime);		// 죽음

public:
	static CBatGrey* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	enum class AWARENESS { No, Yes, End };			// 인식 - 플레이어를 인식했는가
	enum class PHASE { HP_Full, HP_Half, End };		// 페이즈 - 현제 체력

	using AIFunc = void(CBatGrey::*)(void);		// AI 함수
	using ACTFunc = bool(CBatGrey::*)(float);	// 몬스터 행동 함수

private:
	float m_fCountdown;			// 행동 카운트
	float m_fNextAtkWait;		// 다음 공격 대기
	float m_fPlayerTrackCount;	// 플레이어 추적 카운트
	ACTFunc m_fpAction;			// 현재 몬스터 행동 함수 - 행동 완료시 true, 진행시 false
	AWARENESS m_eAwareness;		// 인식
	PHASE m_ePhase;				// 페이즈
	AIFunc m_fpMonsterAI[(int)AWARENESS::End][(int)PHASE::End];	// AI 함수 배열
};

#define __GLACIER_H__
#endif