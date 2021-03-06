// 헬보스 터보 사탄 체인건 총알

#include "stdafx.h"
#include "..\Headers\HellBossChainGunBullet.h"


CHellBossChainGunBullet::CHellBossChainGunBullet(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}

// 프로토타입 초기화
// 반환 값: 성공 S_OK, 실패 E_FAIL
HRESULT CHellBossChainGunBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

// 복제 초기화
// pArg : 인자
// 반환 값: 성공 S_OK, 실패 E_FAIL
HRESULT CHellBossChainGunBullet::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	// 스케일
	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 300.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 150.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 11;

	// 로컬에서 회전
	const float AngleY = 90.f;
	mat RotY;
	D3DXMatrixRotationY(&RotY, MATH::ToRadian(AngleY));

	vec3 CameraLook = { 0,0,-1 };
	vec3 Axis = MATH::Normalize(MATH::Cross(CameraLook, m_vLook));
	float Angle = std::acosf(MATH::Dot(m_vLook, CameraLook));
	mat RotAxis;	// 쏘려는 각도 행렬
	D3DXMatrixRotationAxis(&RotAxis, &Axis, Angle);
	m_matRot = RotY * RotAxis;	// 원본 각도 * 쏘려는 각도

	return S_OK;
}

// 업데이트
// fDeltaTime : 델타 타임
// 반환 값 : 사용하지 않음
_uint CHellBossChainGunBullet::UpdateGameObject(float fDeltaTime)
{
	//CBullet::UpdateGameObject(fDeltaTime);	// 기본 게임오브젝트 업데이트 X

	// 월드 행렬 셋팅
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pTransformCom->m_TransformDesc.vScale.x, m_pTransformCom->m_TransformDesc.vScale.y, m_pTransformCom->m_TransformDesc.vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pTransformCom->m_TransformDesc.vPosition.x, m_pTransformCom->m_TransformDesc.vPosition.y, m_pTransformCom->m_TransformDesc.vPosition.z);
	m_pTransformCom->m_TransformDesc.matWorld = matScale * m_matRot * matTrans;


	vec3 vMoveDstnc = m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition += vMoveDstnc;	// 이동
	m_stStatus.fRange -= D3DXVec3Length(&vMoveDstnc);			// 사거리 차감
	if (m_stStatus.fRange <= 0) {	// 사거리를 전부 차감했으면
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}

	_CollisionComp->Update(m_pTransformCom);
	Bullet_Attack(m_stStatus.fATK);	// 플레이어 충돌 처리

	return _uint();
}

// 레이트 업데이트
// fDeltaTime : 델타 타임
// 반환 값 : 사용하지 않음
_uint CHellBossChainGunBullet::LateUpdateGameObject(float fDeltaTime)
{
	//CBullet::LateUpdateGameObject(fDeltaTime);	// 빌보드 X

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	//Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

// 렌더
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CHellBossChainGunBullet::RenderGameObject()
{
	// 뒷면을 컬링하지 않습니다
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	// 시계 반대 방향을 컬링합니다
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

// 컴포넌트 추가
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CHellBossChainGunBullet::AddComponents()
{
	// CBullet.cpp에서
	// CNormalUVVertexBuffer, CVIBuffer_RectTexture 생성
	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	// 터보 사탄 체인건 총알 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_ChainGunBullet",
		L"Com_Texture_TurboSatan_ChainGunBullet",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::MonsterAttack;
	_Info.bWallCollision = true;
	_Info.bFloorCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

CHellBossChainGunBullet* CHellBossChainGunBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBossChainGunBullet* pInstance = new CHellBossChainGunBullet(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBossChainGunBullet");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBossChainGunBullet::Clone(void* pArg/* = nullptr*/)
{
	CHellBossChainGunBullet* pClone = new CHellBossChainGunBullet(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBossChainGunBullet");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBossChainGunBullet::Free()
{
	CBullet::Free();
}
