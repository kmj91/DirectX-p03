// 컴포넌트 인터페이스
// 컴포넌트들의 부모

#include "..\Headers\Component.h"

USING(Engine)
const std::wstring CComponent::Tag = L"Component_";	// static 멤버 변수 초기화
CComponent::CComponent(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	SafeAddRef(m_pDevice);
}

HRESULT CComponent::ReadyComponentPrototype()
{
	return S_OK;
}

HRESULT CComponent::ReadyComponent(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	SafeRelease(m_pDevice);
}
