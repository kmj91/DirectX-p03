#include "..\Headers\Management.h"
#include "CollisionComponent.h"


USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pGraphic_Dev(CGraphic_Device::Get_Instance())
	, m_pTimeManager(CTime_Manager::Get_Instance())
	, m_pSceneManager(CSceneManager::Get_Instance())
	, m_pGameObjectManager(CGameObjectManager::Get_Instance())
	, m_pComponentManager(CComponentManager::Get_Instance())
{

#ifdef _DEBUG
	bDebug = true;
#endif // DEBUG

	SafeAddRef(m_pGraphic_Dev);
	SafeAddRef(m_pTimeManager);
	SafeAddRef(m_pSceneManager);
	SafeAddRef(m_pGameObjectManager);
	SafeAddRef(m_pComponentManager);
}

// 초기화
// 디바이스 및 매니저 초기화
// CMainApp에서 게임 로직 시작전 초기화하며 호출
// hWnd : 윈도우 핸들
// iWinCX : 윈도우 가로 길이
// iWinCY : 윈도우 세로 길이
// eDisplaymode : 화면 모드(풀스크린 or 창모드)
// iSceneCount : 씬의 수
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CManagement::ReadyEngine(
	HWND hWnd, 
	_uint iWinCX, 
	_uint iWinCY, 
	EDisplayMode eDisplaymode,
	_uint iSceneCount)
{
	MATH::RandomInit();
	// 디바이스 초기화
	if (FAILED(m_pGraphic_Dev->Ready_Graphic_Device(hWnd, iWinCX, iWinCY, eDisplaymode)))
		return E_FAIL;
	// 오브젝트 매니저 사이즈 초기화
	if (FAILED(m_pGameObjectManager->ReserveSizeContainer(iSceneCount)))
		return E_FAIL;
	// 컴포넌트 매니저 사이즈 초기화
	if (FAILED(m_pComponentManager->ReserveSizePrototypeContainer(iSceneCount)))
		return E_FAIL;
	// 렌더러 생성
	m_pRenderer = CRenderer::Create(m_pGraphic_Dev->Get_Device());
	if (nullptr == m_pRenderer)
	{
		PRINT_LOG(L"Error", L"Failed To Create Renderer");
		return E_FAIL;
	}

	return S_OK;
}

// 업데이트
// 게임의 전반적인 로직 수행 Update, LateUpdate
_uint CManagement::UpdateEngine()
{
	if (nullptr == m_pSceneManager)
		return 0;

	/* For.DeltaTime */
	// 델타타임
	float fDeltaTime = m_pTimeManager->GetDeltaTime();

	// 2020.12.16 16:27 KMJ
	// 예약된 처리
	if (FAILED(ScheduledProcessing())) {
		PRINT_LOG(L"Error", L"Failed To ScheduledProcessing");
	}

	/* For.Update */
	// 씬 업데이트
	m_iUpdateEvent = m_pSceneManager->UpdateScene(fDeltaTime);
	if (CHANGE_SCNENE == m_iUpdateEvent)
		return m_iUpdateEvent;
	// 게임 오브젝트 업데이트
	m_iUpdateEvent = m_pGameObjectManager->UpdateGameObject(fDeltaTime);
	// 파티클 업데이트
	if (_ParticleUpdate) _ParticleUpdate(fDeltaTime);

	/* For.LateUpdate */
	// 씬 레이트 업데이트
	m_iUpdateEvent = m_pSceneManager->LateUpdateScene();
	if (CHANGE_SCNENE == m_iUpdateEvent)
		return m_iUpdateEvent;
	// 게임 오브젝트 레이트 업데이트
	m_iUpdateEvent = m_pGameObjectManager->LateUpdateGameObject(fDeltaTime);
	// 파티클 레이트 업데이트
	if (_ParticleLateUpdate) _ParticleLateUpdate(fDeltaTime);
	// ?
	CCollisionComponent::CollisionUpdate(m_pGraphic_Dev->Get_Device());
	// 파티클 충돌 처리
	if (_ParticleCollision) _ParticleCollision();

	
	return _uint();
}

// 렌더
HRESULT CManagement::RenderEngine(HWND hWnd)
{
	if (CHANGE_SCNENE == m_iUpdateEvent)
		return S_OK;

	if (nullptr == m_pRenderer)
		return E_FAIL;	
	// 렌더러에서 렌더
	return m_pRenderer->Render(hWnd);
}

// 씬 클리어
HRESULT CManagement::ClearForScene(_int iSceneIndex)
{
	// 게임 오브젝트 메모리 해제
	if (FAILED(m_pGameObjectManager->ClearForScene(iSceneIndex)))
		return E_FAIL;
	// 컴포넌트 메모리 해제
	if (FAILED(m_pComponentManager->ClearForScene(iSceneIndex)))
		return E_FAIL;
	// 맵 충돌 정보 메모리 해제
	CCollisionComponent::CleanUpMapCollisionInfo();

	return S_OK;
}

LPDIRECT3DDEVICE9 CManagement::GetDevice()
{
	if (nullptr == m_pGraphic_Dev)
		return nullptr;

	return m_pGraphic_Dev->Get_Device();
}

LPDIRECT3D9 CManagement::GetSDK()
{
	if (nullptr == m_pGraphic_Dev)
		return nullptr;

	return m_pGraphic_Dev->Get_SDK();
}

D3DPRESENT_PARAMETERS& CManagement::GetD3Dpp()
{
	return m_pGraphic_Dev->GetD3Dpp();
}

// 씬 교체
// iSceneID : 교체할 씬 번호
// pCurrentScene : 교체할 씬 객체 포인터
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CManagement::SetUpCurrentScene(_int iSceneID, CScene * pCurrentScene)
{
	if (nullptr == m_pSceneManager)
		return E_FAIL;
	// 게임 오브젝트 메모리 해제(프로토타입은 해제하지 않음)
	m_pGameObjectManager->ClearForSceneClone(CurrentSceneIdx);
	//m_pComponentManager->ClearForScene(CurrentSceneIdx);
	// 2020.12.21 11:45 KMJ
	// 생성예정인 오브젝트 리스트 비움
	m_listScheduledObjInfo.clear();
	// 현재 씬 번호 저장
	CurrentSceneIdx = iSceneID;
	// 씬 매니저에서 씬 교체
	return m_pSceneManager->SetUpCurrentScene(iSceneID, pCurrentScene);
}

CGameObject * CManagement::GetGameObject(_int iSceneIndex, const wstring & LayerTag, _uint iIndex)
{
	if (nullptr == m_pGameObjectManager)
		return nullptr;

	if (iSceneIndex == -1)
	{
		iSceneIndex = CurrentSceneIdx;
	}

	return m_pGameObjectManager->GetGameObject(iSceneIndex, LayerTag, iIndex);
}

CComponent * CManagement::GetComponent(
	_int iSceneIndex, 
	const wstring& LayerTag, 
	const wstring& ComponentTag, 
	_uint iIndex)
{
	if (nullptr == m_pGameObjectManager)
		return nullptr;

	if (iSceneIndex == -1)
	{
		iSceneIndex = CurrentSceneIdx;
	}

	auto pGameObject = m_pGameObjectManager->GetGameObject(
		iSceneIndex, LayerTag, iIndex);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject->GetComponent(ComponentTag);
}

std::list<class CGameObject*> CManagement::GetGameObjects(_int iSceneIndex, const wstring& LayerTag)
{
	if (nullptr == m_pGameObjectManager)
		return {};

	if (iSceneIndex == -1)
	{
		iSceneIndex = CurrentSceneIdx;
	}

	return m_pGameObjectManager->GetGameObjects(iSceneIndex, LayerTag);
}

// 게임 오브젝트 프로토타입 추가
// iSceneIndex : 추가할 씬 번호
// GameObjectTag : 프로토타입 이름
// pPrototype : 추가할 프로토타입 객체 포인터
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CManagement::AddGameObjectPrototype(
	_int iSceneIndex, 
	 wstring  GameObjectTag, 
	CGameObject * pPrototype)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;
	// 씬 번호가 '-1' 이면 현재 씬에 프로토타입 추가
	if (iSceneIndex == -1)
	{
		iSceneIndex = CurrentSceneIdx;
	}
	// 게임 오브젝트 매니저 프로토타입 추가 함수 호출
	return m_pGameObjectManager->AddGameObjectPrototype(iSceneIndex, GameObjectTag, pPrototype);
}

// 게임 오브젝트 추가
// 프로토타입 오브젝트를 복제하여 레이어에 추가
// iFromSceneIndex : 복제할 프로토타입 씬 번호
// GameObjectTag : 복제할 프로토타입 이름
// iToSceneIndex : 복사할 씬 번호
// LayerTag : 복사할 레이어 이름
// ppGameObject : 외부로 나갈 Out 변수 복사된 객체 포인터
// pArg : 복사한 게임 오브젝트에 전달할 인수
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CManagement::AddGameObjectInLayer(
	_int iFromSceneIndex, 
	const wstring& GameObjectTag, 
	_int iToSceneIndex, 
	const wstring& LayerTag, 
	CGameObject** ppGameObject,
	void* pArg)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;

	// 게임 오브젝트 매니저 오브젝트 추가 함수 호출
	return m_pGameObjectManager->AddGameObjectInLayer(iFromSceneIndex, GameObjectTag, iToSceneIndex, LayerTag, ppGameObject, pArg);
}

// 2020.12.16 15:31 KMJ
// 예약된 게임 오브젝트 추가 - 다음 프레임 Update 전 처음에 생성됩니다
// iFromSceneIndex : 추가할 씬 번호
// GameObjectTag : 게임 오브젝트 이름
// LayerTag : 추가할 레이어 이름
// ppGameObject : 외부로 나갈 Out 변수 복사된 객체 포인터
// pArg : 복사한 게임 오브젝트에 전달할 인수
void CManagement::AddScheduledGameObjectInLayer(_int iFromSceneIndex, const wstring & GameObjectTag, const wstring & LayerTag, CGameObject ** ppGameObject, void * pArg)
{
	ScheduledGameObjectInfo stScheduledObjInfo;
	stScheduledObjInfo.iFromSceneIndex = iFromSceneIndex;
	stScheduledObjInfo.wstrGameObjectTag = GameObjectTag;
	stScheduledObjInfo.iToSceneIndex = m_pSceneManager->GetCurrentScene()->GetSceneIndex();	// 현제 씬의 씬 인덱스를 가져옴
	stScheduledObjInfo.wstrLayerTag = LayerTag;
	stScheduledObjInfo.ppGameObject = ppGameObject;
	stScheduledObjInfo.pArg = pArg;
	m_listScheduledObjInfo.push_back(stScheduledObjInfo);
}

// 2021.01.11 16:33 KMJ
// 예약된 씬 교체 - 다음 프레임 Update 전 처음에 교체됩니다
// iSceneID : 교체할 씬 번호
// pCurrentScene : 교체할 씬 객체 포인터
void CManagement::AddScheduledReplaceScene(_int iSceneID, CScene * pCurrentScene)
{
	m_iReplaceSceneID = iSceneID;
	m_pReplaceScene = pCurrentScene;
}

// 컴포넌트 프로토타입 추가
// iSceneIndex : 추가할 씬 번호
// ComponentTag : 컴포넌트 이름
// pPrototype : 추가할 컴포넌트 객체 포인터
// 반환 값 : 성공 S_OK, 실패 E_FAIL
HRESULT CManagement::AddComponentPrototype(_int iSceneIndex, const wstring & ComponentTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponentManager)
		return E_FAIL;
	// 컴포넌트 매니저 컴포넌트 프로토타입 추가 함수 호출
	return m_pComponentManager->AddComponentPrototype(iSceneIndex, ComponentTag, pPrototype);
}

// 프로토타입 컴포넌트 복사
// iSceneIndex : 복사할 씬 번호
// ComponentTag : 복사할 컴포넌트 프로토타입 이름
// pArg : 복사할 컴포넌트에 전달할 인수
// 반환 값 : 프로토타입을 복사한 컴포넌트 객체 포인터
CComponent * CManagement::CloneComponentPrototype(
	_int iSceneIndex, 
	const wstring & ComponentTag, 
	void * pArg)
{
	if (nullptr == m_pComponentManager)
		return nullptr;
	// 컴포넌트 매니저 프로토타입 컴포넌트 복사 함수 호출
	return m_pComponentManager->CloneComponentPrototype(iSceneIndex, ComponentTag, pArg);
}

// 렌더러 리스트에 렌더할 오브젝트 추가
HRESULT CManagement::AddGameObjectInRenderer(ERenderID eID, CGameObject * pGameObject, ERenderPlace ePlace/* = ERenderPlace::NONE_PLACE*/)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;
	// 렌더러 매니저에 렌더 오브젝트 추가 함수 호출
	return m_pRenderer->AddGameObjectInRenderer(eID, pGameObject, ePlace);
}

void CManagement::RegistLight(const D3DLIGHT9& Light)
{
	m_pRenderer->RegistLight(Light);
}

// 2020.12.16 16:44 KMJ
// 예정된 처리
// 게임 오브젝트 삭제 및 생성 그리고 씬 교체 처리를 함
HRESULT CManagement::ScheduledProcessing()
{
	HRESULT retVa = S_OK;

	// 삭제 예약된 게임 오브젝트 삭제
	m_pGameObjectManager->RemoveGameObject();

	// 생성 예약된 게임 오브젝트 생성
	for (auto& schedule : m_listScheduledObjInfo) {
		// 오브젝트 레이어에 추가
		if (FAILED(m_pGameObjectManager->AddGameObjectInLayer(
			schedule.iFromSceneIndex,
			schedule.wstrGameObjectTag,
			schedule.iToSceneIndex,
			schedule.wstrLayerTag,
			schedule.ppGameObject,
			schedule.pArg))) {
			retVa = E_FAIL;
		}
	}
	// 비우기
	m_listScheduledObjInfo.clear();

	// 씬 교체
	if (m_pReplaceScene != nullptr) {
		if (FAILED(SetUpCurrentScene(m_iReplaceSceneID, m_pReplaceScene)))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			retVa = E_FAIL;
		}
		m_iReplaceSceneID = 0;
		m_pReplaceScene = nullptr;
	}

	return retVa;
}

void CManagement::Free()
{
	SafeRelease(m_pRenderer);
	SafeRelease(m_pGameObjectManager);
	SafeRelease(m_pComponentManager);
	SafeRelease(m_pSceneManager);
	SafeRelease(m_pTimeManager);
	SafeRelease(m_pGraphic_Dev);
}

void CManagement::ReleaseEngine()
{
	if(CManagement::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CManagement");

	if (CGameObjectManager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CGameObjectManager");

	if (CComponentManager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CComponentManager");

	if (CSceneManager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CSceneManager");

	if (CTime_Manager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CTime_Manager");

	if (CGraphic_Device::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CGraphic_Device");
}
