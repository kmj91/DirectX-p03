// Ÿ�� �Ŵ���
// CPU ƽ���� ��ŸŸ�� ��ȯ

#pragma once
#ifndef __TIME_MANAGER_H__
#include "Base.h"
BEGIN(Engine)
class CTime_Manager final : public CBase
{
	DECLARE_SINGLETON(CTime_Manager)
private:
	CTime_Manager();
	virtual ~CTime_Manager() = default;

public:
	void Ready_Time_Manager(); 
	float GetDeltaTime(); 

public:
	virtual void Free() override;

private:
	LARGE_INTEGER m_CPUCount;		// �ʴ� ī���� �� ��
	LARGE_INTEGER m_LastTime;		// ������ �ð�
	LARGE_INTEGER m_CurrentTime;	// ���� �ð�

};
END

#define __TIME_MANAGER_H__
#endif