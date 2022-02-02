#include "stdafx.h"
#include "..\Headers\Bullet.h"
#include "Camera.h"
#include "NormalUVVertexBuffer.h"
#include "Monster.h"
#include "Player.h"
CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_fFrameSpeed(10.f)
	, m_pTexture(nullptr), m_uiCountRelay(0)
{
}

// 프로토타입 초기화
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

// 복제 초기화
// pArg : 인수
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CBullet::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		// 2020.12.16 17:35 KMJ
		// 아규먼트 메모리 해제
		// 구조체 크기 검사
		if (sizeof(BulletBasicArgument) == *static_cast<_uint*>(pArg)) {
			BulletBasicArgument* pArgument = static_cast<BulletBasicArgument*>(pArg);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			m_vLook = pArgument->vDir;
			m_uiCountRelay = pArgument->uiCountRelay;
			delete pArg;
		}
	}

	return S_OK;
}

// 업데이트
// fDeltaTime : 델타 타임
// 반환 값 : 사용하지 않음
_uint CBullet::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

// 레이트 업데이트
// 카메라 방향으로 바라보도록 빌보드 처리
// fDeltaTime : 델타 타임
// 반환 값 : 사용하지 않음
_uint CBullet::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)-1, L"Layer_MainCamera");
	if (nullptr == pCamera)
		return E_FAIL;

	const auto& _TransformDesc = m_pTransformCom->m_TransformDesc;
	vec3 BillboardRotation = _TransformDesc.vRotation;
	BillboardRotation.y += pCamera->GetTransform()->GetRotation().y;
	m_pTransformCom->m_TransformDesc.matWorld = MATH::WorldMatrix(_TransformDesc.vScale, BillboardRotation, _TransformDesc.vPosition);

	return _uint();
}

// 렌더
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CBullet::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	const mat World = m_pTransformCom->m_TransformDesc.matWorld;
	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			IDirect3DBaseTexture9* const  DiffuseTexture = m_pTexture->GetTexture((_uint)m_fFrameCnt);

			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), DiffuseTexture);
		}
		// 1.       그냥 세팅을 안하거나
		{
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", 0);
		}
		// 2. 세팅을 하고 난 이후의                                   ↑↑↑↑↑↑↑↑↑↑     TRUE 로 바꾸어주기.
		{
			// m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"),SpecularTexture);
			// m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"),NormalTexture);
		}
	}
	// 월드 행렬 바인딩
	_Effect.SetVSConstantData(m_pDevice, "World", World);
	// 광택 설정 
	_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;

	//if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	return S_OK;
}

// 맵에 충돌함
// _PlaneInfo : 바닥 정보
// _CollisionInfo : 충돌 정보
void CBullet::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
}

// 공격 충돌 처리 검사
// 아직 충돌하지 않았다면 Attack 함수를 호출해 플레이어 충돌체와 검사
// _fAttack : 피해량
// 반환 값 : 공격 성공 true, 이미 공격한 상태면 실패 false
bool CBullet::Bullet_Attack(float _fAttack)
{
	if (!m_bOneHit)
	{
		if (Attack(_CollisionComp->_Sphere, _fAttack))
		{
			m_bOneHit = true;
			return true;
		}
	}
	return false;
}

// 플레이어 충돌체와 검사
// 플레이어 충돌체와 충돌하였는지 검사
// _Sphere : 충돌체
// Attack : 피해량
// 반환 값 : 공격 성공 true, 플레이어 충돌체가 비활성화거나 충돌하지 않았다면 false
bool CBullet::Attack(const Sphere _Sphere, const float Attack)&
{
	auto _Player = dynamic_cast<CPlayer* const>(m_pManagement->GetGameObject(-1, L"Layer_Player", 0));
	if (false == _Player->_CollisionComp->bCollision)return false;

	Sphere TargetSphere = _Player->_CollisionComp->_Sphere;
	auto OCollision = Collision::IsSphereToSphere(_CollisionComp->_Sphere, TargetSphere);
	if (OCollision.first)
	{
		this->CurrentAttack = Attack;
		_Player->Hit(this, OCollision.second);
		return true;
	};
	return false;
};

// 컴포넌트 추가
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CBullet::AddComponents()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		(CComponent**)&_VertexBuffer)))
		return E_FAIL;

	return S_OK;
}

// 텍스처 프레임 이동
void CBullet::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

void CBullet::Free()
{
	SafeRelease(_VertexBuffer);
	SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	SafeRelease(m_pTexture);		// 텍스처

	CGameObject::Free();
}


