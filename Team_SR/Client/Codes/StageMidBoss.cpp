// 게임 중간 보스 씬
// 참고 사항 : 씬 업데이트에서 키 처리를 하고 있음

#include "stdafx.h"
#include "..\Headers\StageMidBoss.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlayerInfoUI.h"
#include "Eyebat.h"
#include "MapMidBoss.h"
#include "Stage5th.h"
#include "Monster.h"


CStageMidBoss::CStageMidBoss(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{};

// 씬 초기화
HRESULT CStageMidBoss::ReadyScene()
{
	// 현재 씬 번호
	CurrentSceneID = ESceneID::StageMidBoss;
	// 다음 씬 번호
	NextSceneID = ESceneID::Stage5th;
	using MapType = CMapMidBoss;
	// 배경 음악 이름
	BgmKey = L"037 Egyptian - Osiris.wav";

	Super::ReadyScene();

	CPlayer::InitInfo _InitInfo;
	_InitInfo.SceneID = CurrentSceneID;
	_InitInfo.Location = { 7,7,2 };

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

	MonsterBasicArgument stArg;
	stArg.uiSize = sizeof(MonsterBasicArgument);
	stArg.pPlayer = m_pPlayer;
	stArg.vPosition = { 0.f, 8.f, 95.f };

	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Shark",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		reinterpret_cast<CGameObject**>(&m_pBoss),
		static_cast<void*>(&stArg))))
		return E_FAIL;


	stArg.vPosition = { 35.f, 5.f, 90.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	stArg.vPosition = { -30.f, 5.f, 90.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	stArg.vPosition = { 10.f, 5.f, 110.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	stArg.vPosition = { -10.f, 5.f, 110.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	stArg.vPosition = { 10.f, 5.f, 65.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	stArg.vPosition = { 10.f, 5.f, 75.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	stArg.vPosition = { 10.f, 5.f, 75.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	stArg.vPosition = { 10.f, 5.f, 75.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;




	stArg.vPosition = { 0.f, 5.f, 50.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Spider",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;
	
	// 맵 정보
	// 하드 코딩임...
	BYTE byMap[55][40] = {
	//	  1 2 3 4 5 6 7 8 9 0|1 2 3 4 5 6 7 8 9 0|1 2 3 4 5 6 7 8 9 0|1 2 3 4 5 6 7 8 9 0
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 1
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 2
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 3
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 4
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 5
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 6
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 7
		{ 1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1 },	// 8
		{ 1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1 },	// 9
		{ 1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1 },	// 10
		{ 1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1 },	// 11
		{ 1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1 },	// 12
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1 },	// 13
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 },	// 14
		{ 1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 },	// 15
		{ 1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },	// 16
		{ 1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },	// 17
		{ 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1 },	// 18
		{ 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1 },	// 19
		{ 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1 },	// 20
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },	// 21
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },	// 22
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 },	// 23
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 },	// 24
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 },	// 25
		{ 1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 },	// 26
		{ 1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1 },	// 27
		{ 1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1 },	// 28
		{ 1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1 },	// 29
		{ 1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1 },	// 30
		{ 1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1 },	// 31
		{ 1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1 },	// 32
		{ 1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1 },	// 33
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 34
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 35
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 36
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 37
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 38
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 39
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 40
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 41
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 42
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 43
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 44
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 45
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 46
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 47
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 48
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 49
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 50
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 51
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 52
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 53
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },	// 54
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }		// 55
	};

	// 점프 포인트 서치 초기화 (길찾기 알고리즘)
	JumpPointSearch::Get_Instance()->ReadyMap(byMap[0], 40, 55, 21, 54, 2.5f, 5);
	// 아이템 오브젝트 로드
	LoadObjects(L"..\\Resources\\Map\\MidBoss\\DecoItemData.obj", vec3{ 2.5,2.5,2.5 });

	return S_OK;
}

// 업데이트
// fDeltaTime : 델타 타임
_uint CStageMidBoss::UpdateScene(float fDeltaTime)
{
	return Super::UpdateScene(fDeltaTime);
}

// 레이트 업데이트
_uint CStageMidBoss::LateUpdateScene()
{
	// 2021.01.11 19:19 KMJ
	// 리턴하지않고 아래에 로직 수행
	//return 	Super::LateUpdateScene();
	Super::LateUpdateScene();

	// 2021.01.11 19:19 KMJ
	// 보스 몬스터가 죽고나서 생성함
	if (!m_bBossDead && m_pBoss->IsDead()) {
		m_bBossDead = true;

		// 다음 스테이지로 넘어가는 포탈
		WormholeArgument stArg;
		stArg.vPosition = { -2.5f, 5.f, 110.f };
		stArg.eReplaceSceneID = ESceneID::Stage5th;
		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Wormhole",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Bullet",
			nullptr,
			static_cast<void*>(&stArg))))
			return E_FAIL;
	}
}

// 키 처리
// 부모 CStage 에서 플레이어 관련 공용 키 처리를 함
// fDeltaTime : 델타 타임
// 반환 값 : 사용하지 않음...
// 반환 값인 CHANGE_SCNENE 전혀 사용하지 않고 있음
// 그런데도 씬 교체에 문제가 없는 상황... 운이 좋았음
_uint CStageMidBoss::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

	if (m_pKeyMgr->Key_Down(VK_SHIFT))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->SetUpCurrentScene((_int)NextSceneID,
			CStage5th::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}

		return CHANGE_SCNENE;
	}
	if (m_pKeyMgr->Key_Down('B'))
	{
		int random = rand() % 100 - 50;
		MonsterBasicArgument stArg;
		stArg.uiSize = sizeof(MonsterBasicArgument);
		stArg.pPlayer = m_pPlayer;
		stArg.vPosition = { 0.f, 5.f, 95.f };
		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Spider",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Monster",
			nullptr, static_cast<void*>(&stArg))))
			return E_FAIL;
	}

	return _uint();
}

// 플레이어 키 처리
// _CurrentPlayer : 플레이어 포인터
// fDeltaTime : 델타 타임
void CStageMidBoss::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);
}

CStageMidBoss* CStageMidBoss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStageMidBoss* pInstance = new CStageMidBoss(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStageMidBoss::Free()
{
	// 2021.01.11 19:08 KMJ
	// 보스 메모리 해제
	SafeRelease(m_pBoss);

	Super::Free();
}
