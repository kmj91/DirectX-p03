// 타임 매니저
// CPU 틱으로 델타타임 반환

#include "Time_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTime_Manager)

CTime_Manager::CTime_Manager()
{
	ZeroMemory(&m_CPUCount, sizeof(LARGE_INTEGER)); 
	ZeroMemory(&m_StartTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_GoalTime, sizeof(LARGE_INTEGER));

}

void CTime_Manager::Ready_Time_Manager()
{
	QueryPerformanceCounter(&m_StartTime);
	QueryPerformanceCounter(&m_GoalTime);
	QueryPerformanceFrequency(&m_CPUCount);
}

float CTime_Manager::Update_Time_Manager()
{
	// 초당 카운터 빈도 수
	QueryPerformanceFrequency(&m_CPUCount);
	// 고해상도 카운터 수
	QueryPerformanceCounter(&m_GoalTime);
	// (현재 시간 - 이전 시간) / 초당 카운터 수 = 델타타임
	float fDeltaTime = float(m_GoalTime.QuadPart - m_StartTime.QuadPart) / m_CPUCount.QuadPart; 
	m_StartTime.QuadPart = m_GoalTime.QuadPart;

	return fDeltaTime;
}

void CTime_Manager::Free()
{
}
