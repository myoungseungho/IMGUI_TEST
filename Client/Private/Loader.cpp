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
#include "Mon_Copy_Koofu.h"
#include "Skill_Bug_SludgeWave.h"
#include "Mon_Trash_Slime.h"
#include "Mon_Bear_Solider.h"
#include "Mon_Bear_Cannon.h"
#include "Skill_Cannon_Ball.h"
#include "End_Stand.h"
#include "End_Orb.h"
#include "UnRotation_Orb.h"
#include "UnRotation_Stand.h"
#include "Rotation_Stand.h"
#include "Small_Orb.h"
#include "Rotation_Orb.h"
#include "Npc.h"
#include "UI_Npc_Talk.h"
#include "Laser.h"
#include "Un_Laser.h"
#include "Un_Small_Orb.h"
#include "Door.h"
#include "Effect_Bug_Bullet_Destroy.h"
#include "Effect_Bug_Dash.h"
#include "Effect_Bug_Line.h"
#include "Effect_Bug_Down.h"
#include "Effect_Mon_Destroy.h"
#include "Effect_Shield.h"
#include "Effect_Shadow.h"
#include "Effect_Stun.h"
#include "Effect_Light.h"
#include "Effect_Koofu_Smoke.h"
#include "Box.h"
#include "Item_Skill.h"
#include "Tree_Green.h"
#include "Wall_One.h"
#include "Wall_Two.h"
#include "Outside_RealShop.h"
#include "Shop.h"
#include "Skill_Arror.h"
#include "CSound.h"
#include "Effect_Mon_Hit.h"

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

	m_pGameInstance->SetLoadingLevelIndex(m_eLevelID);

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
	case LEVEL_TACHO:
		hr = Loading_For_Tacho();
		break;
	case LEVEL_JUNGLE:
		hr = Loading_For_Jungle();
		break;
	case LEVEL_SNOW:
		hr = Loading_For_Snow();
		break;
	case LEVEL_BUG:
		hr = Loading_For_Bug();
		break;
	case LEVEL_KOOFU:
		hr = Loading_For_Koofu();
		break;
	case LEVEL_EDIT:
		hr = Loading_For_Edit();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::Loading_For_Tacho()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_0.png"), 1))))
		return E_FAIL;

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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_UI_MobileInputs_ShopButton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_UI_MobileInputs_ShopButton.png"), 1))))
		return E_FAIL;
	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Sprite_TutorialVillager_Pig_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/NPC/0_Tutorial/Pig/Sprite_TutorialVillager_Pig_%d.png"), 6))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TACHO, TEXT("Prototype_Component_AnimTexture_Sprite_TutorialVillager_Pig_Bye"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/NPC/0_Tutorial/Pig/Sprite_TutorialVillager_Pig_React_%d.png"), 7))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	

	/* For.Prototype_GameObject_Skill_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Player"),
		CSkill_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Outside_RealShop"),
		COutside_RealShop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc"),
		CNpc::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shop"),
		CShop::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Jungle()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Tree_Green"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_TutorialObjects_TreeStreet1.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Wall_One"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_JungleMaze_CliffGate.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Wall_Two"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_JungleNewBossMap_RightCliffAdd.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Item_Skill"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Item/Skill/Sprite_ItemThumbnails_Quest_4_PowerSymbolPush.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Sprite_StonePushable"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_StonePushable.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Sprite_MonkeyStatue_Trigger"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Sprite_MonkeyStatue_Trigger.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_AnimTexture_Box_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Item/Box/Sprite_ChestWodden_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_AnimTexture_Box_Open"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Item/Box/Sprite_ChestWodden_%d.png"), 7))))
		return E_FAIL;

#pragma region Texture_Laser_Quiz

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Orb"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_OceanLightOrb_Orb.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_SmallOrb"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_OceanLightOrb_OrbSmall.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_EndOrb"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_OceanLightOrb_OrbEnd.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_RotationOrbStand"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_OceanLightOrb_StandStarter.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_UnRotationOrbStand"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_OceanLightOrb_Stand.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_EndOrbStand"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_OceanLightOrb_StandEnd.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_OrbEffect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_OceanLightOrb_OrbLight.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Laser"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Quiz/Laser/Sprite_CircleBlur.png"), 1))))
		return E_FAIL;


#pragma endregion

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Skill_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Player"),
		CSkill_Player::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonkeyStatue"),
		CMonkey_Statue::Create(m_pGraphic_Device))))
		return E_FAIL;
	
#pragma region For_Laser_Quiz
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rotation_Orb"),
		CRotation_Orb::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Small_Orb"),
		CSmall_Orb::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Un_Small_Orb"),
		CUn_Small_Orb::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UnRotation_Orb"),
		CUnRotation_Orb::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_End_Orb"),
		CEnd_Orb::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rotation_Stand"),
		CRotation_Stand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UnRotation_Stand"),
		CUnRotation_Stand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_End_Stand"),
		CEnd_Stand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Laser"),
		CLaser::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Un_Laser"),
		CUn_Laser::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Orb_Door"),
		CDoor::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Box"),
		CBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Skill"),
		CItem_Skill::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree_Green"),
		CTree_Green::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall_One"),
		CWall_One::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall_Two"),
		CWall_Two::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Snow()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Terrain_Snow"),
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Sprite_Snow_Ball"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/Snow_Ball/Snow_Ball.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Shadow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Shadow/Sprite_Shadow.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_Texture_Arror"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Arror/BearSoldierShield_%d.png"), 8))))
		return E_FAIL;

	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Snow_Fire_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Snow/Sprite_BonFire_%d.png"), 9))))
		return E_FAIL;

	/* Monster_Trash_Slime_Idle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/Down/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/Left/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/LeftDown/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/LeftUp/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/Right/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/RightDown/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/RightUp/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Idle_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Idle/Up/TrashSlime_%d.png"), 2))))
		return E_FAIL;

	/* Monster_Trash_Slime_Walk*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/Down/TrashSlime_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/Left/TrashSlime_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/LeftDown/TrashSlime_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/LeftUp/TrashSlime_%d.png"), 8))))
		return E_FAIL;																													
																																																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Right"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/Right/TrashSlime_%d.png"), 8))))
		return E_FAIL;																													
																																		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_RightDown"),			
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/RightDown/TrashSlime_%d.png"), 8))))
		return E_FAIL;																													  
																																		  
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_RightUp"),				  
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/RightUp/TrashSlime_%d.png"), 8))))
		return E_FAIL;																													  
																																		  
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Trash_Slime_Walk_Up"),					  
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/TrashSlime/Movement/Up/TrashSlime_%d.png"), 8))))
		return E_FAIL;

	/* Monster_Bear_Soldier_Attack*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/Down/BearSoldier1_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/Left/BearSoldier1_%d.png"), 6))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_LeftDown"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/LeftDown/BearSoldier1_%d.png"), 6))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_LeftUp"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/LeftUp/BearSoldier1_%d.png"), 6))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Right"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/Right/BearSoldier1_%d.png"), 6))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_RightDown"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/RightDown/BearSoldier1_%d.png"), 6))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_RightUp"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/RightUp/BearSoldier1_%d.png"), 6))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Attack_Up"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Attack/Up/BearSoldier1_%d.png"), 6))))
		return E_FAIL;

	/* Monster_Bear_Soldier_Walk*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/Down/BearSoldier1_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/Left/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_LeftDown"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/LeftDown/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_LeftUp"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/LeftUp/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Right"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/Right/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_RightDown"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/RightDown/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_RightUp"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/RightUp/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Walk_Up"),						
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Armor_Walk/Up/BearSoldier1_%d.png"), 4))))
		return E_FAIL;

	/* Monster_Bear_Soldier_Hit*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/Down/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Left"),						
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/Left/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_LeftDown"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/LeftDown/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_LeftUp"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/LeftUp/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Right"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/Right/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_RightDown"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/RightDown/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_RightUp"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/RightUp/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Hit_Up"),						
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Hit/Up/BearSoldier1_%d.png"), 4))))
		return E_FAIL;

	/* Monster_Bear_Soldier_Stun*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/Down/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Left"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/Left/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_LeftDown"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/LeftDown/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_LeftUp"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/LeftUp/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Right"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/Right/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_RightDown"),				
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/RightDown/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_RightUp"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/RightUp/BearSoldier1_%d.png"), 4))))
		return E_FAIL;																														
																																			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Stun_Up"),						
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Stun/Up/BearSoldier1_%d.png"), 4))))
		return E_FAIL;

	/* Monster_Bear_Soldier_Idle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearSoldier_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearSoldier/Idle/BearSoldier1_%d.png"), 6))))
		return E_FAIL;

	/* Monster_Bear_Cannon_Idle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/Down/BearCannon_%d.png"), 6))))
		return E_FAIL;																										
																															
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Left"),		
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/Left/BearCannon_%d.png"), 6))))
		return E_FAIL;																											
																																
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_LeftDown"),		
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/LeftDown/BearCannon_%d.png"), 6))))
		return E_FAIL;																											
																																
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_LeftUp"),		
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/LeftUp/BearCannon_%d.png"), 6))))
		return E_FAIL;																											
																																
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Right"),		
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/Right/BearCannon_%d.png"), 6))))
		return E_FAIL;																											
																																
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_RightDown"),	
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/RightDown/BearCannon_%d.png"), 6))))
		return E_FAIL;																											
																																
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_RightUp"),		
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/RightUp/BearCannon_%d.png"), 6))))
		return E_FAIL;																											
																																
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Idle_Up"),			
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Idle/Up/BearCannon_%d.png"), 6))))
		return E_FAIL;
	
	/* Monster_Bear_Cannon_Attack*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/Down/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;																													  
																																		  
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Left"),				  
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/Left/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;																													  
																																		  
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_LeftDown"),			  
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/LeftDown/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;																													  
																																		  
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_LeftUp"),				  
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/LeftUp/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;																													  
																																		  
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Right"),				  
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/Right/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;																													
																																		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_RightDown"),			
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/RightDown/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;																													  
																																		  
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_RightUp"),			  
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/RightUp/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;																													
																																		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_BearCannon_Attack_Up"),					
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BearCannon/Attack/Up/BearCannon_Attack_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Stun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Stun/Sprite_StunEffect_%d.png"), 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Mon_Destroy"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Mon_Destroy/Sprite_MonsterEffects_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SNOW, TEXT("Prototype_Component_AnimTexture_Mon_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Hit/HitEffect_%d.png"), 6))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//컴포넌트 로드한다.

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Skill_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Player"),
		CSkill_Player::Create(m_pGraphic_Device))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Trash_Slime"),
		CMon_Trash_Slime::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Bear_Solider"),
		CMon_Bear_Solider::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Bear_Cannon"),
		CMon_Bear_Cannon::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Cannon_Ball"),
		CSkill_Cannon_Ball::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Shadow"),
		CEffect_Shadow::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Mon_Destory"),
		CEffect_Mon_Destroy::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stun"),
		CEffect_Stun::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Arror"),
		CSkill_Arror::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Hit"),
		CEffect_Mon_Hit::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Koofu()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Down/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Left/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/LeftDown/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/LeftUp/Koofu_%d.png"), 5))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Right/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/RightDown/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/RightUp/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Idle_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Idle/Up/Koofu_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Death"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Death/Koofu_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Stun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Stun/Koofu_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Ready"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Ready/Koofu_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Cast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Cast/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Down/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Left/Koofu_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/LeftDown/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/LeftUp/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Right/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/RightDown/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/RightUp/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Throw_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Throw/Up/Koofu_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Down/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Left"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Left/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/LeftDown/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_LeftUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/LeftUp/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Right"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Right/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_RightDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/RightDown/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_RightUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/RightUp/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Koofu_Walk_Up"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/Koofu/Walk/Up/Koofu_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_FuitBounce"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/FuitBounce/FruitBounce_%d.png"), 4))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_RollingIce"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/RollingIce/RollingIce_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Bubble"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/Bubble/Bubble.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Texture_Shield"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Shield/Sprite_ShieldBlurALot.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_KOOFU, TEXT("Prototype_Component_Texture_Texture_Smoke"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/SmokeEffect/SmokeEffect_0.png"), 1))))
		return E_FAIL;

	/* 애니메이션 텍스쳐를 로드한다*/


	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//컴포넌트 로드한다.


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Skill_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Player"),
		CSkill_Player::Create(m_pGraphic_Device))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Koofu_Rolling"),
		CSkill_Koofu_Rolling::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Koofu_Fuit"),
		CSkill_Koofu_Fuit::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Koofu_Bubble"),
		CSkill_Koofu_Bubble::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_Koofu */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Koofu"),
		CBoss_Koofu::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_Koofu_Copy */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Koofu_Copy"),
		CMon_Copy_Koofu::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Shield"),
		CEffect_Shield::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Koofu_Smoke"),
		CEffect_Koofu_Smoke::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Bug()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Terrain_Bug"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Terrain/Orgu_Terrain_3.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Sprite_MoonForest_FloorStone"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FloorStone.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Sprite_MoonBoss_MothBossFrameThorn"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonBoss_MothBossFrameThorn.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Skill_Bug_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/BugBall/BugBall_%d.png"), 12))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_SludgeWave"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Animation/Monster/SludgeWave/SludgeWave_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Monster_Red_Turtle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/RedBeatle/Idle/Down/RedBeatle_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Monster_Green_Turtle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/GreenBeatle/Idle/Down/GreenBeatle_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Monster_Blue_Turtle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BlueBeatle/Idle/Down/BlueBeatle_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase1/Idle/Bugboss_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase1_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase1/Attack/Bugboss_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Ready"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Ready/Bugboss_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Regen"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Regen/Bugboss_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Attack/Bugboss_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Death"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Death/Bugboss_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_BugBoss_Phase2_Down"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Monster/BugBoss/Phase2/Down/Bugboss_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_Texture_Effect_Light"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Light/Eye_0.png"), 1))))
		return E_FAIL;

	/* 애니메이션 텍스쳐를 로드한다*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Fence_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornOff_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Fence_Die"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornOff_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Vertical_Fence_Idle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornVerticalOff_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Vertical_Fence_Die"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Enviorment/Moon/Sprite_MoonForest_FenceThornVerticalOff_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Effect_Bug_Bullet_Destroy"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Bug_Bullet_Destroy/BugBulletDestroy_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Effect_Bug_Dash"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Bug_Dash/Bug_Dash_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Effect_MothFlyLine"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Bug_Dash_Line/MothFlyLine_0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BUG, TEXT("Prototype_Component_AnimTexture_Effect_BugBossDown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Orgu_144_Resource/Textures/Effect/Monster/Boss_Bug_Down/Sprite_BugBossDown_%d.png"), 6))))
		return E_FAIL;

	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//컴포넌트 로드한다.


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Skill_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Player"),
		CSkill_Player::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMon_Pocket::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_Bug */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Bug"),
		CBoss_Bug::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* Prototype_GameObject_Mon_Turtle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Turtle"),
		CMon_Turtle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Bug_Bullet*/

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Bug_Bullet"),
		CSkill_Bug_Bullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Bug_SludgeWave */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Bug_SludgeWave"),
		CSkill_Bug_SludgeWave::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Bug_Bullet_Destroy */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Bug_Bullet_Destroy"),
		CEffect_Bug_Bullet_Destroy::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Bug_Dash"),
		CEffect_Bug_Dash::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Bug_Line"),
		CEffect_Bug_Line::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Bug_Down"),
		CEffect_Bug_Down::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Light"),
		CEffect_Light::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;
}

HRESULT CLoader::Loading_For_Edit()
{
	/* 텍스쳐를 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));


	/* 애니메이션 텍스쳐를 로드한다*/


	/* 모델을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));


	/* 객체원형을 로드한다. */
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
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
