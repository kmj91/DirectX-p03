// 게임 오브젝트 인터페이스
// 기본적으로 트랜스폼 컴포넌트를 생성합니다.
// 참고 사항
// 게임 오브젝트를 삭제할 때 바로 삭제하는 방식이 아님 (댕글링 포인터 문제)
// Remove 플래그 확인하고 게임 로직 한곳에서 삭제합니다.

#include "..\Headers\GameObject.h"
#include "Management.h"
#include "Scene.h"


USING(Engine)

const std::wstring CGameObject::Tag = L"GameObject_";

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice), m_pManagement(CManagement::Get_Instance()), m_byObjFlag(0)
{
	SafeAddRef(m_pDevice);
}

CComponent* CGameObject::GetComponent(const wstring& ComponentTag)
{
	auto iter_find = m_Components.find(ComponentTag);
	if (m_Components.end() == iter_find)
		return nullptr;

	return iter_find->second;
}

// 프로토타입 초기화
HRESULT CGameObject::ReadyGameObjectPrototype()
{
	return S_OK;
}

// 복제 초기화
// pArg : 인자
HRESULT CGameObject::ReadyGameObject(void* pArg)
{
	// 기본 컴포넌트 추가
	AddStaticComponents();

	return S_OK;
}

// 업데이트
// fDeltaTime : 델타 타임
_uint CGameObject::UpdateGameObject(float fDeltaTime)
{
	// 중력 플래그가 ON 이면
	if (bGravity)
	{
		// y축 좌표로 중력 값 적용
		m_pTransformCom->m_TransformDesc.vPosition.y -= MATH::Gravity;
	}
	// 트랜스폼 업데이트
	m_pTransformCom->UpdateTransform();

	
	return _uint();
}

// 레이트 업데이트
// fDeltaTime : 델타 타임
_uint CGameObject::LateUpdateGameObject(float fDeltaTime)
{
	
	return _uint();
}

// 렌더
HRESULT CGameObject::RenderGameObject()
{
	/*m_pDevice->SetVertexShader(nullptr);
	m_pDevice->SetPixelShader(nullptr);*/
	// 장치에 월드 행렬 값 적용
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	return S_OK;
}

void CGameObject::Hit(CGameObject* const _Target, const Collision::Info& _CollisionInfo)
{
}

void CGameObject::MapHit(const PlaneInfo& _PlaneInfo, const Collision::Info& _CollisionInfo)
{
}

void CGameObject::ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)
{
}

void CGameObject::SetLocation(const vec3 Location)
{
	 m_pTransformCom->SetLocation(Location); 
}

void CGameObject::Free()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pTransformCom);
	// 2020.12.17 11:26 KMJ
	SafeRelease(_CollisionComp);		// 충돌 컴포넌트

	for (auto& Pair : m_Components)
	{
		SafeRelease(Pair.second);
	}

	m_Components.clear();
}

HRESULT CGameObject::AddComponent(
	_int iSceneIndex,
	const wstring& PrototypeTag,
	const wstring& ComponentTag,
	CComponent** ppComponent,
	void* pArg)
{
	CComponent* pClone = m_pManagement->CloneComponentPrototype(iSceneIndex, PrototypeTag, pArg);
	if (nullptr == pClone)
		return E_FAIL;

	m_Components.insert(make_pair(ComponentTag, pClone));

	if (ppComponent)
	{
		*ppComponent = pClone;
		SafeAddRef(pClone);
	}

	return S_OK;
}

HRESULT CGameObject::AddStaticComponents()
{
	/* For.Com_Transform */
	if (FAILED(CGameObject::AddComponent(
		STATIC,
		CComponent::Tag + TYPE_NAME<CTransform>(),
		CComponent::Tag + TYPE_NAME<CTransform>(),
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}
