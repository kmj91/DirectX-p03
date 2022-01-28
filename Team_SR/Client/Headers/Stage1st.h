// ���� �������� 1 ��
// ���� ���� : �� ������Ʈ���� Ű ó���� �ϰ� ����

#pragma once
#ifndef __STAGE1ST_H__

#include "Stage.h"

USING(Engine)
class CStage1st final : public CStage
{
private:
	explicit CStage1st(LPDIRECT3DDEVICE9 pDevice);
public:
	using Super = CStage;
	// �� �ʱ�ȭ
	virtual HRESULT ReadyScene() override;
	// ������Ʈ
	virtual _uint UpdateScene(float fDeltaTime) override;
	// ����Ʈ ������Ʈ
	virtual _uint LateUpdateScene() override;
protected:
	// Ű ó��
	virtual _uint KeyProcess(float fDeltaTime) override;
	// �÷��̾� Ű ó��
	virtual void PlayerKeyProcess(class CPlayer* const _CurrentPlayer, float fDeltaTime)override;
public:
	static CStage1st* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
public:
};

#define __STAGE1ST_H__
#endif
