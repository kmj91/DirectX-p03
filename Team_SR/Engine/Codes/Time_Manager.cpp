// Ÿ�� �Ŵ���
// CPU ƽ���� ��ŸŸ�� ��ȯ

#include "Time_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTime_Manager)

CTime_Manager::CTime_Manager()
{
	ZeroMemory(&m_CPUCount, sizeof(LARGE_INTEGER)); 
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurrentTime, sizeof(LARGE_INTEGER));

}

void CTime_Manager::Ready_Time_Manager()
{
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_CurrentTime);
	QueryPerformanceFrequency(&m_CPUCount);
}

float CTime_Manager::GetDeltaTime()
{
	// �ʴ� ī���� �� ��
	QueryPerformanceFrequency(&m_CPUCount);
	// ���ػ� ī���� ��
	QueryPerformanceCounter(&m_CurrentTime);
	// (���� �ð� - ���� �ð�) / �ʴ� ī���� �� = ��ŸŸ��
	float fDeltaTime = float(m_CurrentTime.QuadPart - m_LastTime.QuadPart) / m_CPUCount.QuadPart;
	m_LastTime.QuadPart = m_CurrentTime.QuadPart;

	return fDeltaTime;
}

void CTime_Manager::Free()
{
}
