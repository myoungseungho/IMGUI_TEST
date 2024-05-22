#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "BackGround.h"
#include "Mon_Pocket.h"
#include "Player.h"
#include "Skill_Player.h"
#include "Camera.h"
#include "Terrain.h"
#include "Tree.h"
#include "Push_Stone.h"
#include "Monkey_Statue.h"
#include "RockBreakable.h"
#include "Block.h"
#include "Hole.h"
#include "Bush.h"
#include "Boss_Bug.h"
#include "Boss_Koofu.h"
#include "Skill_Bug_Bullet.h"
#include "Mon_Turtle.h"
#include "Sky.h"
#include "Skill_Koofu_Fuit.h"
#include "Skill_Koofu_Rolling.h"
#include "Outside_BreadShop.h"
#include "Outside_PotHorizontal1.h"
#include "Outside_PotHorizontal2.h"
#include "Outside_Shop.h"
#include "Outside_WaterTank.h"
#include "Outside_SuperMarket.h"
#include "TachoShop_Tile.h"
#include "Outside_Fence.h"
#include "TachoGround_Tile.h"
#include "Outside_TelePhonePole.h"
#include "Snow_Tree.h"
#include "Snow_Rock.h"
#include "Snow_GrassGround.h"
#include "Snow_Bush.h"
#include "Snow_CenterDirt.h"
#include "Snow_Fence.h"
#include "Snow_Vertical_Fence.h"
#include "Snow_House.h"
#include "Snow_Fire.h"
#include "Snow_FireWood.h"
#include "Koofu_Circle.h"
#include "Koofu_LeftCenter.h"
#include "Koofu_RightCenter.h"
#include "Koofu_LowerCenter.h"
#include "Koofu_LowerLeft.h"
#include "Koofu_LowerRight.h"
#include "Koofu_Upper.h"
#include "Koofu_UpperLeft.h"
#include "Koofu_UpperRight.h"
#include "Moon_BossFrame.h"
#include "Moon_Fence.h"
#include "Moon_FloorStone.h"
#include "Moon_VerticalFence.h"
#include "Skill_Koofu_Bubble.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{

	m_eLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr{};

	m_isFinished = false;

	switch (m_eLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_LogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;
	case LEVEL_EDIT:
		hr = Loading_For_Edit();
		break;
	case LEVEL_TACHO:
		hr = Loading_For_Tacho();
		break;
	case LEVEL_SNOW:
		hr = Loading_For_Snow();
		break;
	case LEVEL_KOOFU:
		hr = Loading_For_Koofu();
		break;
	case LEVEL_BUG:
		hr = Loading_For_Bug();
		break;
	case LEVEL_OCEAN:
		hr = Loading_For_Ocean();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::Loading_For_LogoLevel()
{
	/* 로고레벨용 자원을 로드하자. */

	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;


	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));



	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
	/* 게임플레이레벨용 자원을 로드하자. */

	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));


	/* For.Prototype_Component_AnimTexture_Player */

#pragma region AnimTexture_Player_Idle

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Up/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Right/Player_Idle_%d.png"), 7))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Down/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/Left/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/LeftUp/Player_Idle_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/RIghtUp/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/RightDown/Player_Idle_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Idle_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Idle/LeftDown/Player_Idle_%d.png"), 7))))
		return E_FAIL;
#pragma endregion

#pragma region Animation_Player_Walk

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Up/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Right/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Left/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/LeftUp/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/RIghtUp/Player_Walk_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/RightDown/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Walk_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/LeftDown/Player_Walk_%d.png"), 10))))
		return E_FAIL;
#pragma endregion

#pragma region Animation_Player_Skill

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Up/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Right/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Down/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/Left/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/LeftUp/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/RightUp/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/RightDown/Player_Smash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Skill_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Smash/LeftDown/Player_Smash_%d.png"), 8))))
		return E_FAIL;

#pragma endregion

#pragma region Animation_Player_Attack

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Up/Player_Swing_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Right/Player_Swing_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Down/Player_Swing_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/Left/Player_Swing_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/LeftUp/Player_Swing_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/RightUp/Player_Swing_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/RightDown/Player_Swing_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Attack_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Swing/LeftDown/Player_Swing_%d.png"), 9))))
		return E_FAIL;
#pragma endregion

#pragma region Animation_Player_Push

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Up/Player_Push_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Right/Player_Push_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Down/Player_Push_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Player_Push_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Push/Left/Player_Push_%d.png"), 10))))
		return E_FAIL;
#pragma endregion

#pragma region Texture_Player_Skill

	/* For.Prototype_Component_Texture_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Skill"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Effect/Smash/ForSkill/Sprite_OguSmashCrackBlur.png"), 1))))
		return E_FAIL;

#pragma endregion

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Tree"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tree_69.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sprite_StonePushable"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_StonePushable.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sprite_MonkeyStatue_Trigger"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_MonkeyStatue_Trigger.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sprite_RockBreakable"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_RockBreakable.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sprite_Block"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Blocking/Sprite_StoneBlockBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sprite_Bush"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_LootNaturalObjects_ColorBush0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Bug_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/BugBall/BugBall_%d.png"), 12))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster_Red_Turtle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/RedBeatle/Idle/Down/RedBeatle_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster_Green_Turtle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/GreenBeatle/Idle/Down/GreenBeatle_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster_Blue_Turtle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BlueBeatle/Idle/Down/BlueBeatle_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase1/Idle/Bugboss_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase1/Attack/Bugboss_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Ready"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Ready/Bugboss_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Regen"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Regen/Bugboss_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Attack/Bugboss_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Death"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Death/Bugboss_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Down/Bugboss_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FuitBounce"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/FuitBounce/FruitBounce_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_RollingIce"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/RollingIce/RollingIce_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bubble"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/Bubble/Bubble.png"), 1))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Down/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Left/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/LeftDown/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/LeftUp/Koofu_%d.png"), 5))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Right/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/RightDown/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/RightUp/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Idle_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Up/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Death"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Death/Koofu_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Stun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Stun/Koofu_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Ready"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Ready/Koofu_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Cast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Cast/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Down/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Left/Koofu_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/LeftDown/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/LeftUp/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Right/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/RightDown/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/RightUp/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Throw_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Up/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Down/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Left/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/LeftDown/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/LeftUp/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Right/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/RightDown/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/RightUp/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Koofu_Walk_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Up/Koofu_%d.png"), 10))))
		return E_FAIL;

	//애니메이션 로드한다
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Hole_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_Hole_Idle.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AnimTexture_Hole_Stone"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_Hole_%d.png"), 4))))
		return E_FAIL;


	//컴포넌트 로드한다.


	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

#pragma region  하이트맵 터레인
	/* For.Prototype_Component_VIBuffer_Terrain */
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/PlaneHeight.bmp")))))
		return E_FAIL;*/
#pragma endregion

		/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 100, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StonePushable"),
		CPush_Stone::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonkeyStatue"),
		CMonkey_Statue::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RockBreakable"),
		CRockBreakable::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Block"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMon_Pocket::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_Bug */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Bug"),
		CBoss_Bug::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Player"),
		CSkill_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_Koofu */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Koofu"),
		CBoss_Koofu::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Turtle"),
		CMon_Turtle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Bug_Bullet*/

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Bug_Bullet"),
		CSkill_Bug_Bullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Koofu_Rolling"),
		CSkill_Koofu_Rolling::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Koofu_Fuit"),
		CSkill_Koofu_Fuit::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Koofu_Bubble"),
		CSkill_Koofu_Bubble::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Edit()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_0.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Tree"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tree_69.png"), 1))))
		return E_FAIL;

	// 돌 에디터 용
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Sprite_StonePushable"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_StonePushable.png"), 1))))
	//	return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Sprite_MonkeyStatue_Trigger"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_MonkeyStatue_Trigger.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Sprite_RockBreakable"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_RockBreakable.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Sprite_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_Hole_Idle.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Texture_Sprite_Bush"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_LootNaturalObjects_ColorBush0.png"), 1))))
		return E_FAIL;

	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_AnimTexture_Block_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Blocking/Sprite_StoneBlockBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_AnimTexture_Block_UnIdle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Idle/Sprite_StoneBlockNotBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_AnimTexture_UnBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/UnBlock/Sprite_StoneUnBlock%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_AnimTexture_Block"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block/Sprite_StoneBlock%d.png"), 9))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 100, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StonePushable"),
		CPush_Stone::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonkeyStatue"),
		CMonkey_Statue::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RockBreakable"),
		CRockBreakable::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Block"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tacho()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_0.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Tree"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tree_69.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_StonePushable"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_StonePushable.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_MonkeyStatue_Trigger"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_MonkeyStatue_Trigger.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_RockBreakable"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_RockBreakable.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_Hole_Idle.png"), 1))))
		return E_FAIL;

	

	////////////

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_Shop"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/House/ModelHouse_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_BreadShop"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/House/ModelHouse_1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_WaterTank"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/Outside_WaterTank.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_PotHorizontal1"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/Outside_PotHorizontal1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_PotHorizontal2"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/Outside_PotHorizontal2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_Supermarket"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/Outside_Supermarket.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_TachoShop_Tile"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tile/Tile_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_TachoGround_Tile"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tile/Tile_183.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_Fence"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/Outside_FenceWoodenHorizontal.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_TelephonePole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Tacho/Outside_TelephonePole1.png"), 1))))
		return E_FAIL;

	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Block_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Blocking/Sprite_StoneBlockBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Block_UnIdle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Idle/Sprite_StoneBlockNotBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_UnBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/UnBlock/Sprite_StoneUnBlock%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Block"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block/Sprite_StoneBlock%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Sprite_Bush_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_LootNaturalObjects_ColorBush0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Sprite_Bush_Move"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_LootNaturalObjects_ColorBush%d.png"), 5))))
		return E_FAIL;


	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));
	

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StonePushable"),
		CPush_Stone::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonkeyStatue"),
		CMonkey_Statue::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RockBreakable"),
		CRockBreakable::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Block"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_Shop"),
		COutside_Shop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_BreadShop"),
		COutside_BreadShop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_WaterTank"),
		COutside_WaterTank::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_PotHorizontal1"),
		COutside_PotHorizontal1::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_PotHorizontal2"),
		COutside_PotHorizontal2::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_SuperMarket"),
		COutside_SuperMarket::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TachoShop_Tile"),
		CTachoShop_Tile::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TachoGround_Tile"),
		CTachoGround_Tile::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_Fence"),
		COutside_Fence::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_TelephonePole"),
		COutside_TelePhonePole::Create(m_pGraphic_Device))))
		return E_FAIL;


	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Snow()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_SnowTree"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowNaturalObject_Tree2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_StonePushable"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_StonePushable.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_SnowNaturalObject_RockMossHorizontal2"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowNaturalObject_RockMossHorizontal2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_SnowTundraGrass_2"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowTundraGrass_2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_SnowTownObjects2_BushSnow1"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowTownObjects2_BushSnow1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_SnowTown_CenterDirt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowTown_CenterDirt.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_SnowTownObjects2_FenceSnowHorizontal2"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowTownObjects2_FenceSnowHorizontal2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_SnowTownObjects2_FenceSnowVertical2"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowTownObjects2_FenceSnowVertical2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_SnowTownObjects1_HouseMediumRight"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_SnowTownObjects1_HouseMediumRight.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_BonFire_Wood"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_BonFire_Wood.png"), 1))))
		return E_FAIL;

	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Snow_Fire_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_BonFire_%d.png"), 9))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//컴포넌트 로드한다.

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StonePushable"),
		CPush_Stone::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_Tree"),
		CSnow_Tree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_Rock"),
		CSnow_Rock::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_GrassGround"),
		CSnow_GrassGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_Bush"),
		CSnow_Bush::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_CenterDirt"),
		CSnow_CenterDirt::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_Fence"),
		CSnow_Fence::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_Vertical_Fence"),
		CSnow_Vertical_Fence::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_House"),
		CSnow_House::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_Fire"),
		CSnow_Fire::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Snow_FireWood"),
		CSnow_FireWood::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Koofu()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_2.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Center"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_FloorCircle.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_CenterLeft.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_CenterRight.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Lower"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_LowerCenter.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_LowerLeft"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_LowerLeft.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_LowerRight"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_LowerRight.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Upper"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_UpperCenter.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Upeer_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_UpperLeft.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Upper_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_Snow_Platform_UpperRight.png"), 1))))
		return E_FAIL;


	/* 애니메이션 텍스쳐를 로드한다*/
	

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//컴포넌트 로드한다.


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_Circle"),
		CKoofu_Circle::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_LeftCenter"),
		CKoofu_LeftCenter::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_RightCenter"),
		CKoofu_RightCenter::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_LowerCenter"),
		CKoofu_LowerCenter::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_LowerLeft"),
		CKoofu_LowerLeft::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_LowerRight"),
		CKoofu_LowerRight::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_Upper"),
		CKoofu_Upper::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_UpperLeft"),
		CKoofu_UpperLeft::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_UpperRight"),
		CKoofu_UpperRight::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Bug()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_3.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Sprite_MoonForest_FloorStone"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FloorStone.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Sprite_MoonBoss_MothBossFrameThorn"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonBoss_MothBossFrameThorn.png"), 1))))
		return E_FAIL;


	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Fence_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornOff_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Fence_Die"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornOff_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_RockBreakable_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_RockBreakable0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_RockBreakable_Die"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_RockBreakable%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Vertical_Fence_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornVerticalOff_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Vertical_Fence_Die"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornVerticalOff_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Block_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Blocking/Sprite_StoneBlockBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Block_UnIdle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block_Idle/Sprite_StoneBlockNotBlocking.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_UnBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/UnBlock/Sprite_StoneUnBlock%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Block"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Block_States/Block/Sprite_StoneBlock%d.png"), 9))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//컴포넌트 로드한다.


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Moon_FloorStone"),
		CMoon_FloorStone::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Moon_BossFrame"),
		CMoon_BossFrame::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Moon_Fence"),
		CMoon_Fence::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Moon_VerticalFence"),
		CMoon_VerticalFence::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RockBreakable"),
		CRockBreakable::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Block"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Ocean()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Player/Player_Walk/Down/Player_Walk_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_3.png"), 1))))
		return E_FAIL;
	

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//컴포넌트 로드한다.


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVELID eNextLevelID)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pGraphic_Device);

	__super::Free();
}
