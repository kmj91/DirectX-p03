// 타임 매니저
// CPU 틱으로 델타타임 반환

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
	LARGE_INTEGER m_CPUCount;		// 초당 카운터 빈도 수
	LARGE_INTEGER m_LastTime;		// 마지막 시간
	LARGE_INTEGER m_CurrentTime;	// 현재 시간

};
END

#define __TIME_MANAGER_H__
#endif