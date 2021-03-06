#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"
#include "Player.h"
#include "ImGuiHelper.h"
#include "MainCamera.h"
#include "Glacier.h"
#include "GlacierBullet.h"
#include "GlacierParticle.h"
#include "CollisionComponent.h"
#include "DXWrapper.h"
#include "PlyerInfoUI.h"
#include "WeaponAmmoInfoUI.h"

#include "Terret.h"
#include "Blood.h"
#include "Spider.h"
#include "Eyebat.h"
#include "EyebatBullet.h"
#include "NormalUVVertexBuffer.h"
#include "Fire.h"
#include "BatGrey.h"	// 박쥐
#include "BatSpit.h"	// 박쥐 총알
#include "FloorBlood.h"
#include "UIManager.h"	// UI 매니저
#include "Item.h"		// 아이템
#include "Hangman.h"	// 행맨
#include "HangmanBomb.h" // 행맨 폭탄
#include "Hellhound.h"	// 헬 하운드
#include "ParticleSystem.h"

#include "Particle.h"	// 파티클
#include "Decorator.h"	// 장식품
#include "Shark.h"
#include "SharkBullet.h"

CMainApp::CMainApp()
	: m_pManagement(CManagement::Get_Instance())
{
	SafeAddRef(m_pManagement);
}

HRESULT CMainApp::ReadyMainApp()
{
	if (FAILED(m_pManagement->ReadyEngine(g_hWnd, WINCX, WINCY,
		EDisplayMode::Window, (_uint)ESceneID::MaxCount)))
	{
		PRINT_LOG(L"Error", L"Failed To ReadyEngine");
		return E_FAIL;
	}

	m_pDevice = m_pManagement->GetDevice();
	if (nullptr == m_pDevice)
		return E_FAIL;

	ImGuiHelper::Init(g_hWnd, m_pDevice);

	//SafeAddRef(m_pDevice);	// ImGui Init함수 안에서 이미 증가시키고 있음

	if (FAILED(ReadyStaticResources()))
		return E_FAIL;

	if (FAILED(ReadyDefaultSetting()))
		return E_FAIL;

	ParticleSystem::Initialize(m_pManagement, m_pDevice);
	auto& _ParticleSys = ParticleSystem::Instance();



	if (FAILED(m_pManagement->SetUpCurrentScene((_int)ESceneID::Logo,
		CLogo::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
		return E_FAIL;
	}

	// UI 초기화
	if (FAILED(CUIManager::Get_Instance()->ReadyUI())) {
		PRINT_LOG(L"Error", L"Failed To ReadyUI");
		return E_FAIL;
	}

	srand(0);	// 랜덤 시드값

	CSoundMgr::Get_Instance()->Initialize();

	return S_OK;
}

int CMainApp::UpdateMainApp()
{
	ImGuiHelper::UpdateStart();
	m_pManagement->UpdateEngine();
	ImGuiHelper::Update();
	ImGuiHelper::DebugInfo(g_hWnd);
	ImGui::Checkbox("Debug ?", &m_pManagement->bDebug);
	ImGuiHelper::CheckBoxCall();
	ImGuiHelper::UpdateEnd();
	m_pManagement->RenderEngine();
	ImGuiHelper::Render(m_pDevice);
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, g_hWnd, nullptr);

	Effect::ClearRegisteredLighting();

	return 0;
}

HRESULT CMainApp::ReadyStaticResources()
{
	/* For.GameObject */
	// 플레이어
#pragma region GameObject_Player
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		/*L"Layer_Player"*/
		CGameObject::Tag + TYPE_NAME<CPlayer>(),
		CPlayer::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 카메라
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CMainCamera>(),
		CMainCamera::Create(m_pDevice))))
		return E_FAIL;

	// 박쥐
#pragma region GameObject_BatGrey
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CBatGrey>(),
		CBatGrey::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 플레이어 UI
#pragma region GameObject_PlayerInfoUI
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CPlyerInfoUI>(),
		CPlyerInfoUI::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 무기 총알 UI
#pragma region GameObject_WeaponAmmoInfoUI
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CWeaponAmmoInfoUI>(),
		CWeaponAmmoInfoUI::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 글레이서
#pragma  region GameObject_Glacier
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CGlacier>(),
		CGlacier::Create(m_pDevice))))
		return E_FAIL;


#pragma endregion

	// 글레이서 총알
#pragma  region GameObject_GlacierBullet
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CGlacierBullet>(),
		CGlacierBullet::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 박쥐 총알
#pragma  region GameObject_BatSpit
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CBatSpit>(),
		CBatSpit::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 글레이서 파티클
#pragma region GameObject_GlacierParticle
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CGlacierParticle>(),
		CGlacierParticle::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
	
	// 눈깔 박쥐
#pragma region GameObject_Eyebat
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CEyebat>(),
		CEyebat::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 눈깔 박쥐 총알
#pragma region GameObject_EyebatBullet
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CEyebatBullet>(),
		CEyebatBullet::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
	
	// 불
#pragma region GameObject_Fire
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CFire>(),
		CFire::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Spider
		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CSpider>(),
			CSpider::Create(m_pDevice))))
			return E_FAIL;
#pragma endregion

#pragma region GameObject_Blood
		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CBlood>(),
			CBlood::Create(m_pDevice))))
			return E_FAIL;
#pragma endregion

#pragma region GameObject_FloorBlood
		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CFloorBlood>(),
			CFloorBlood::Create(m_pDevice))))
			return E_FAIL;
#pragma endregion

#pragma region GameObject_Terret
		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CTerret>(),
			CTerret::Create(m_pDevice))))
			return E_FAIL;
	// 아이템 오브젝트
#pragma endregion
#pragma region GameObject_Shark
		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CShark>(),
			CShark::Create(m_pDevice))))
			return E_FAIL;
#pragma endregion
#pragma region GameObject_Fire
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CItem>(),
		CItem::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	

	// 행맨 오브젝트
#pragma region GameObject_Hangman
	// 행맨
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CHangman>(),
		CHangman::Create(m_pDevice))))
		return E_FAIL;
	// 투사체 폭탄
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CHangmanBomb>(),
		CHangmanBomb::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 헬 하운드 오브젝트
#pragma region GameOBject_Hellhound
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CHellhound>(),
		CHellhound::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 파티클 오브젝트
#pragma region GameOBject_Particle
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CParticle>(),
		CParticle::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


	// 장식품 오브젝트
#pragma region GameObject_Decorator
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CDecorator>(),
		CDecorator::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Sharkicile
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CSharkBullet>(),
		CSharkBullet::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


		

	/* For.Component */
	// 렉트 텍스처
#pragma region Component_VIBuffer_RectTexture
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		CVIBuffer_RectTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 트랜스폼
#pragma region Component_Transform
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CTransform>(),
		CTransform::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
#pragma region Component_CCollision
	if (FAILED(m_pManagement->AddComponentPrototype(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CCollisionComponent::Create(m_pDevice))))
	{
		return E_FAIL;
	}
#pragma endregion

#pragma region Component_NormalUVVertexBuffer
	if (FAILED(m_pManagement->AddComponentPrototype(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CNormalUVVertexBuffer::Create(m_pDevice))))
	{
		return E_FAIL;
	}


#pragma region Component_Texture_Player

#pragma endregion

	// 글레이서 텍스처들
#pragma region Component_Texture_Glacier
	wstring wstrTextureGlacier = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CGlacier>();
#pragma region Move
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureGlacier + L"Move",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Glacier/Move/Move%d.png", 16))))
		return E_FAIL;
#pragma endregion
#pragma region Death
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureGlacier + L"Death",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Glacier/Death/Death%d.png", 9))))
		return E_FAIL;
#pragma endregion

#pragma region Hurt
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureGlacier + L"Hurt",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Glacier/Hurt/Hurt%d.png", 16))))
		return E_FAIL;
#pragma endregion

#pragma region Attack
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureGlacier + L"Attack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Glacier/Attack/Attack%d.png", 11))))
		return E_FAIL;
#pragma endregion

#pragma region Bullet
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureGlacier + L"Bullet",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Glacier/Bullet/Bullet%d.png", 4))))
		return E_FAIL;
#pragma endregion

#pragma region Particle
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureGlacier + L"Particle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Glacier/Particle/Particle%d.png", 5))))
		return E_FAIL;
#pragma endregion

#pragma endregion	// Component_Texture_Glacier

	// 박쥐 텍스처들
#pragma region Component_Texture_BatGrey
	// 플라이
#pragma region Component_Texture_BatGreyFly
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyFly",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/BatGrey/Fly/batGreyFly%d.png", 8))))
		return E_FAIL;
#pragma endregion
	// 원거리 공격
#pragma region Component_Texture_BatGreyShoot
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyShoot",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/BatGrey/Shoot/batGreyShoot%d.png", 5))))
		return E_FAIL;
#pragma endregion
	// 근접 공격
#pragma region Component_Texture_BatGreyAttack
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyAttack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/BatGrey/Attack/batGreyattack%d.png", 4))))
		return E_FAIL;
#pragma endregion
	// 뒤돌아봄
#pragma region Component_Texture_BatGreyBack
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyBack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/BatGrey/Back/batGreyFly_back%d.png", 7))))
		return E_FAIL;
#pragma endregion
	// 피격
#pragma region Component_Texture_BatGreyHit
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyHit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/BatGrey/Hit/batGreyHit_%d.png", 2))))
		return E_FAIL;
#pragma endregion
	// 죽음
#pragma region Component_Texture_BatGreyDeath
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyDeath",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/BatGrey/Death/batGreydeath%d.png", 11))))
		return E_FAIL;
#pragma endregion
	// 박쥐 침
#pragma region Component_Texture_BatGreySpit
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreySpit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/BatGrey/Spit/bat_spit%d.png", 8))))
		return E_FAIL;
#pragma endregion

#pragma endregion	// Component_Texture_BatGrey

	// 눈깔 박쥐 텍스처들
#pragma region Component_Texture_Eyebat
	wstring wstrTextureEyebat = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CEyebat>();
#pragma region Fly
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureEyebat + L"Fly",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Eyebat/Fly/Fly%d.png", 7))))
		return E_FAIL;
#pragma endregion

	Effect::EffectInitialize(m_pDevice);
#pragma region Attack
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureEyebat + L"Attack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Eyebat/Attack/Attack%d.png", 7))))
		return E_FAIL;
#pragma endregion

#pragma region Bullet
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureEyebat + L"Bullet",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Eyebat/Bullet/Bullet%d.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Death
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureEyebat + L"Death",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Eyebat/Death/Death%d.png", 12))))
		return E_FAIL;
#pragma endregion
#pragma endregion	// Component_Texture_Eyebat

	// 불
#pragma region Component_Texture_Fire
	wstring wstrTextureFire = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CFire>();
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureFire,
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Fire/Fire%d.png", 22))))
		return E_FAIL;
#pragma endregion

	// 플레이어 UI 텍스처
#pragma region Component_Texture_PlayerInfoUI
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_PlayerInfoUI",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bottom_left.png", 1))))
		return E_FAIL;
#pragma endregion
	
	// 무기 탄약 UI 텍스처
#pragma region Component_Texture_WeaponAmmoInfoUI
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_WeaponAmmoInfoUI",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bottom_right.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texutre_Spider
#pragma region Spider_Attack
	wstring wstrTextureSpider = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CSpider>();
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Attack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Spider/Attack/Attack%d.png", 14))))
		return E_FAIL;
#pragma endregion

#pragma region Spider_Death
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Death",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Spider/Death/Death%d.png", 14))))
		return E_FAIL;
#pragma endregion

#pragma region Spider_Hit
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Hit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Spider/Hit/Hit%d.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Spider_Walk
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Walk",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Spider/Walk/Walk%d.png", 4))))
		return E_FAIL;
#pragma endregion

#pragma region Spider_Wep
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Wep",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Spider/Wep/Wep%d.png", 1))))
		return E_FAIL;
#pragma endregion
#pragma endregion

#pragma region Component_Texture_Blood
	wstring wstrTextureBlood = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CBlood>();
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureBlood,
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Effect/Blood/Blood%d.png", 8))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_FloorBlood
	wstring wstrTextureFloorBlood = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CFloorBlood>();
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureFloorBlood,
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Effect/FloorBlood/FloorBlood%d.png", 4))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Terret
	wstring wstrTextureTerret = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CTerret>();
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrTextureTerret,
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Terret/Fire/Fire%d.png", 8))))
		return E_FAIL;
#pragma endregion
	// 아이템 텍스처들
#pragma region Component_Texture_Item
	// 겁나큰 체력 포션
#pragma region Health_Big
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Health_Big",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/TEXTURE_HEALTH_BIG_000%d.png", 4))))
		return E_FAIL;
#pragma endregion
	// 작은 체력 포션
#pragma region Health_Small
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Health_Small",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/TEXTURE_HEALTH_SMALL_000%d.png", 4))))
		return E_FAIL;
#pragma endregion
	// 겁나큰 마나 포션
#pragma region Mana_Big
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Mana_Big",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/WAND_AMMO_BIG_000%d.png", 4))))
		return E_FAIL;
#pragma endregion
	// 작은 마나 포션
#pragma region Mana_Small
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Mana_Small",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/TEXTURE_WAND_AMMO_000%d.png", 4))))
		return E_FAIL;
#pragma endregion
	// 총알
#pragma region Ammo_Box
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Ammo_Box",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/ammo_box_pistol.png", 1))))
		return E_FAIL;
#pragma endregion
	// 파란 열쇠
#pragma region Key_Blue
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Key_Blue",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/TEXTURE_KEY_BLUE.png", 1))))
		return E_FAIL;
#pragma endregion
	// 레드 열쇠
#pragma region Key_Red
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Key_Red",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/TEXTURE_KEY_RED.png", 1))))
		return E_FAIL;
#pragma endregion
	// 노랑 열쇠
#pragma region Key_Yellow
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Key_Yellow",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/TEXTURE_KEY_YELLOW.png", 1))))
		return E_FAIL;
#pragma endregion
	// 업그레이드 재화
#pragma region Upgrade
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Upgrade",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Item/upgrade_pickup000%d.png", 8))))
		return E_FAIL;
#pragma endregion
#pragma endregion

	// 행맨 텍스처들
#pragma region Component_Texture_Hangman
	// Idle Back
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Back",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Back/hangman_idle_back.png", 1))))
		return E_FAIL;

	// 근접 공격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Attack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Attack/hangman_attack%d.png", 13))))
		return E_FAIL;

	// 원거리 폭탄 공격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Bomb_Attack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Bomb/hangman_bomb%d.png", 15))))
		return E_FAIL;

	// 이동
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Walk",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Walk/hangman_walk000%d.png", 6))))
		return E_FAIL;

	// 피격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Hit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Hit/hangman_hit.png", 1))))
		return E_FAIL;

	// 손상
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Damage",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Damage/hangman_damage000%d.png", 6))))
		return E_FAIL;

	// 손상 근거리 공격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_DamageAttack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/DamageAttack/hangman_damaged_attack000%d.png", 10))))
		return E_FAIL;

	// 손상 피격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_DamagedHit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/DamagedHit/hangman_damaged_hit.png", 1))))
		return E_FAIL;

	// 손상 이동
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_DamagedWalk",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/DamagedWalk/hangman_damaged_walk000%d.png", 6))))
		return E_FAIL;

	// 죽음
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Death",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Death/hangman_damaged_death000%d.png", 10))))
		return E_FAIL;

	// 투사체 폭탄
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Bullet",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Bullet/bomb0000.png", 1))))
		return E_FAIL;

	// 내장
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Gib",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hangman/Gib/gib_hangman%d.png", 2))))
		return E_FAIL;
#pragma endregion	// Component_Texture_Hangman

	// 헬 하운드 텍스처들
#pragma region Component_Textrue_HellHound
	// 부화
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_EggHatch",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/EggHatch/egg_hatch_hellhound%d.png", 12))))
		return E_FAIL;

	// 대기
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Idle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/Idle/hellhound_idle.png", 1))))
		return E_FAIL;

	// 달리기
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Run",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/Run/hellhound_run000%d.png", 5))))
		return E_FAIL;

	// 근접 공격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Attack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/Attack/hellhound_attack000%d.png", 10))))
		return E_FAIL;

	// 아픔
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Hurt",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/Hurt/hellhound_hurt.png", 1))))
		return E_FAIL;

	// 손상
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Damage",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/Damage/hellhound_damage%d.png", 3))))
		return E_FAIL;

	// 손상 근접 공격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_DamagedAttack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/DamagedAttack/hellhound_damaged_attack000%d.png", 7))))
		return E_FAIL;

	// 손상 피격
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_DamagedHit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/DamagedHit/hellhound_damaged_hit.png", 1))))
		return E_FAIL;

	// 손상 이동
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_DamagedWalk",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/DamagedWalk/hellhound_damaged_walk%d.png", 6))))
		return E_FAIL;

	// 죽음
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Death",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/Death/hellhound_death%d.png", 12))))
		return E_FAIL;

	// 손상 내장
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_DamageGib",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/DamageGib/hellhound_dmagegib_%d.png", 3))))
		return E_FAIL;

	// 내장
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Gib",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Hellhound/Gib/hellhound_gib_%d.png", 5))))
		return E_FAIL;
#pragma endregion	// Component_Textrue_HellHound

	// 이펙트 텍스처들
#pragma region Component_Textrue_Effect
	// 큰 출혈 이펙트 1
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BloodHit_1_Big",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Effect/BloodHit_1_Big/bloodhit_big000%d.png", 8))))
		return E_FAIL;

	// 큰 출혈 이펙트 2
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BloodHit_2_Big",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Effect/BloodHit_2_Big/bloodhit_2_big000%d.png", 8))))
		return E_FAIL;
#pragma endregion	// Component_Textrue_Effect

	// 장식품 텍스처들
#pragma region Component_Texture_Decorator
	// 횃불
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Torch",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Torch/texture_misc_Torch_000%d.png", 9))))
		return E_FAIL;

	// 양초
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Candle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Candle/Candle%d.png", 5))))
		return E_FAIL;

	// 나무 통
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Barrel",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Barrel/Barrel%d.png", 2))))
		return E_FAIL;

	// 폭탄 통
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BarrelBomb",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/BarrelBomb/BarrelBomb%d.png", 2))))
		return E_FAIL;

	// 폐기물 통
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BarrelWaste",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/BarrelWaste/BarrelWaste%d.png", 3))))
		return E_FAIL;

	// 상자 1
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Box1",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Box1/Box1_%d.png", 2))))
		return E_FAIL;

	// 상자 2
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Box2",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Box2/Box2_%d.png", 2))))
		return E_FAIL;

	// 강철 상자 1
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BoxSteel1",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/BoxSteel1/BoxSteel1_%d.png", 2))))
		return E_FAIL;

	// 강철 상자 2
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BoxSteel2",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/BoxSteel2/BoxSteel2_%d.png", 2))))
		return E_FAIL;

	// 묘비 1
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Headstone1",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Headstone1/Headstone1_%d.png", 9))))
		return E_FAIL;

	// 묘비 2
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Headstone2",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Headstone2/Headstone2_%d.png", 9))))
		return E_FAIL;

	// 묘비 3
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Headstone3",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Headstone3/Headstone3_%d.png", 7))))
		return E_FAIL;

	// 가시 덩굴
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_ThornyVine",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/ThornyVine/ThornyVine%d.png", 6))))
		return E_FAIL;

	// 죽은 나무
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_TreeBlight",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/TreeBlight/TreeBlight%d.png", 5))))
		return E_FAIL;

	// 나무 1
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Tree1",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Tree1.png", 1))))
		return E_FAIL;

	// 나무 2
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Tree2",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/Tree2.png", 1))))
		return E_FAIL;

	// 죽은 갈렙 - 블러드 패러디
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Dead_Caleb",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/dead_caleb.png", 1))))
		return E_FAIL;

	// 죽은 둠가이 - 둠 패러디
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Dead_Doomguy",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/dead_doomguy.png", 1))))
		return E_FAIL;

	// 죽은 듀크 - 듀크 뉴켐 패러디
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Dead_Duke",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/dead_duke.png", 1))))
		return E_FAIL;

	// 죽은 샘 - 시리어스 샘 패러디
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Dead_Sam",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/dead_sam.png", 1))))
		return E_FAIL;

	// 죽은 왕 - 쉐도우 워리어 패러디
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Dead_Wang",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Decorator/dead_wang.png", 1))))
		return E_FAIL;
#pragma endregion	// Component_Texture_Decorator
#pragma region Component_Texutre_Shark

	wstring wstrShark = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CShark>();
	wstring wstrIcicle =CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CSharkBullet>();

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Death",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Death/Death%d.png", 13))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Howling",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Howling/Howling%d.png", 10))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrIcicle + L"icicleA",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/icicleA/icicleA%d.png", 18))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrIcicle + L"icicleB",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/icicleB/icicleB%d.png", 16))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrIcicle + L"icicleC",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/icicleC/icicleC%d.png", 18))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"LoseBothArm",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/LoseBothArm/LoseBothArm%d.png", 7))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"LoseRightArm",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/LoseRightArm/LoseRightArm%d.png", 6))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Melee",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Melee/Melee%d.png", 11))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Melee_1Phase",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Melee_1Phase/Melee_1Phase%d.png", 14))))
		return E_FAIL;
	

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Particle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Particle/Particle%d.png", 4))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"RangeAttack_1Phase",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/RangeAttack_1Phase/RangeAttack_1Phase%d.png", 12))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"RangeAttack_2Phase",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/RangeAttack_2Phase/RangeAttack_2Phase%d.png", 11))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Walk_1Phase",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Walk_1Phase/Walk_1Phase%d.png", 8))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Walk_2Phase",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Walk_2Phase/Walk_2Phase%d.png", 8))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		wstrShark + L"Walk_3Phase",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Monster/Shark/Walk_3Phase/Walk_3Phase%d.png", 9))))
		return E_FAIL;


#pragma endregion Component_Texutre_Shark


	return S_OK;
}

HRESULT CMainApp::ReadyDefaultSetting()
{
	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->ReadyMainApp()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMainApp");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	CUIManager::Destroy_Instance();
	Effect::EffectRelease();
	SafeRelease(m_pDevice);
	SafeRelease(m_pManagement);
	CKeyMgr::Destroy_Instance();
	ParticleSystem::Release();
	CManagement::ReleaseEngine();
	CSoundMgr::Get_Instance()->Release();
	CSoundMgr::Destroy_Instance();

}
