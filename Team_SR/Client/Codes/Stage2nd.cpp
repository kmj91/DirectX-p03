// 게임 스테이지 2 씬
// 참고 사항 : 씬 업데이트에서 키 처리를 하고 있음

#include "stdafx.h"
#include "..\Headers\Stage2nd.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlayerInfoUI.h"
#include "Eyebat.h"
#include "Map2nd.h"
#include "Stage3rd.h"

CStage2nd::CStage2nd(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{}

// 씬 초기화
HRESULT CStage2nd::ReadyScene()
{
	// 현재 씬 번호
	CurrentSceneID = ESceneID::Stage2nd;
	// 다음 씬 번호
	NextSceneID = ESceneID::Stage3rd;
	using MapType = CMap2nd;
	// 배경 음악 이름
	BgmKey = L"018 Medieval Epilogue - Chloradyne.wav";

	Super::ReadyScene();

	CPlayer::InitInfo _InitInfo;
	_InitInfo.SceneID = CurrentSceneID;
	_InitInfo.Location = { 5,5,9};

	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CPlayer>(),
		(_int)CurrentSceneID,
		CLayer::Tag + TYPE_NAME<CPlayer>(),
		(CGameObject**)&m_pPlayer, &_InitInfo)))
		return E_FAIL;

	const wstring GameObjTag = CGameObject::Tag + TYPE_NAME<MapType>();
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)CurrentSceneID,
		GameObjTag,
		MapType::Create(m_pDevice))))
		return E_FAIL;

	const std::wstring LayerTag = CLayer::Tag + TYPE_NAME<MapType>();
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)CurrentSceneID,
		GameObjTag,
		(_int)CurrentSceneID,
		LayerTag,
		reinterpret_cast<CGameObject**>(&_CurrentMap), &CurrentSceneID)))
		return E_FAIL;

	// 다음 스테이지로 넘어가는 포탈
	WormholeArgument stArg;
	stArg.vPosition = { 47.5f, 5.f, -70.f };
	stArg.eReplaceSceneID = ESceneID::Stage3rd;
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Wormhole",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Bullet",
		nullptr,
		static_cast<void*>(&stArg))))
		return E_FAIL;

	// 맵 정보
	// 하드 코딩...
	BYTE byMap[44][44] = {
	//	  1 2 3 4 5 6 7 8 9 0|1 2 3 4 5 6 7 8 9 0|1 2 3 4 5 6 7 8 9 0|1 2 3 4 5 6 7 8 9 0|1 2 3 4
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 1
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 2
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 3
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 4
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 5
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 6
		{ 1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 7
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 8
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 9
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 10
		{ 1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 11
		{ 1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 12
		{ 1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1 },	// 13
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1 },	// 14
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1 },	// 15
		{ 1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1 },	// 16
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 17
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 18
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 19
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 20
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 21
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 22
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 23
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 24
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 25
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 26
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 27
		{ 1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 28
		{ 1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 29
		{ 1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 30
		{ 1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 31
		{ 1,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },	// 32
		{ 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 33
		{ 1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 34
		{ 1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 35
		{ 1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1 },	// 36
		{ 1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 37
		{ 1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 38
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 39
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 40
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 41
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 42
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 43
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }		// 44
	};

	// 점프 포인트 서치 초기화 (길찾기 알고리즘)
	JumpPointSearch::Get_Instance()->ReadyMap(byMap[0], 44, 44, 18, 7, 2.5f, 5);
	// 몬스터 오브젝트 로드
	LoadObjects(L"..\\Resources\\Map\\2\\GameObjectData.obj", vec3{ 2.5,2.5,2.5 });			
	// 촛불 오브젝트 로드
	LoadObjects(L"..\\Resources\\Map\\2\\CandleTorchData.obj", vec3{ 2.5,2.5,2.5 });
	// 아이템 오브젝트 로드
	LoadObjects(L"..\\Resources\\Map\\2\\DecoItemData.obj", vec3{ 2.5,2.5,2.5 });
	


	return S_OK;
}

// 업데이트
// fDeltaTime : 델타 타임
_uint CStage2nd::UpdateScene(float fDeltaTime)
{
	return Super::UpdateScene(fDeltaTime);
}

// 레이트 업데이트
_uint CStage2nd::LateUpdateScene()
{
	return 	Super::LateUpdateScene();
}

// 키 처리
// 부모 CStage 에서 플레이어 관련 공용 키 처리를 함
// fDeltaTime : 델타 타임
// 반환 값 : 사용하지 않음...
// 반환 값인 CHANGE_SCNENE 전혀 사용하지 않고 있음
// 그런데도 씬 교체에 문제가 없는 상황... 운이 좋았음
_uint CStage2nd::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

	if (m_pKeyMgr->Key_Down(VK_SHIFT))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;
		// 스테이지 3으로 씬 교체
		if (FAILED(pManagement->SetUpCurrentScene((_int)NextSceneID,
			CStage3rd::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}

		return CHANGE_SCNENE;
	}

	return _uint();
}

// 플레이어 키 처리
// _CurrentPlayer : 플레이어 포인터
// fDeltaTime : 델타 타임
void CStage2nd::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);
}

CStage2nd* CStage2nd::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage2nd* pInstance = new CStage2nd(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStage2nd::Free()
{
	Super::Free();
}
