// ������Ʈ �Ŵ���
// ������Ʈ ������Ÿ�� ����

#pragma once
#ifndef __COMPONENTMANAGER_H__

#include "Base.h"
#include "Transform.h"
#include "VIBuffer_RectTexture.h"
//#include "VIBuffer_TerrainTexture.h"
#include "VIBuffer_CubeTexture.h"
#include "Texture.h"

BEGIN(Engine)
class CComponentManager : public CBase
{
	DECLARE_SINGLETON(CComponentManager)
private:
	CComponentManager();
	virtual ~CComponentManager() = default;
public:
	// map �迭 ������ �ʱ�ȭ
	HRESULT ReserveSizePrototypeContainer(_int iSceneCount);
	// ������Ÿ�� ������Ʈ �߰�
	HRESULT AddComponentPrototype(_int iSceneIndex, const wstring& ComponentTag, CComponent* pPrototype);
	// ������Ÿ�� ������Ʈ ����
	CComponent* CloneComponentPrototype(_int iSceneIndex, const wstring& ComponentTag, void* pArg = nullptr);
	// �޸� ����
	HRESULT ClearForScene(_int iSceneIndex);
public:
	virtual void Free() override;
private:
	typedef unordered_map<wstring, CComponent*> PROTOTYPES;
	PROTOTYPES*	m_pPrototypes = nullptr;
	_int m_iSceneCount = 0;
};
END

#define __COMPONENTMANAGER_H__
#endif // !

