#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBackGround::CBackGround(const CBackGround & Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	/* 사본객체의 초기화작업을 수행한다. */
	/* 원형객체에서 채워주지 못한 기타 등드으이 다른 데이터를 더 추가한다. */
	//BACKGROUND_DESC* pBackGroundDesc = static_cast<BACKGROUND_DESC*>(pArg);
	//m_iBackGroundData = pBackGroundDesc->iBackGroundData;	

	//pBackGroundDesc->iGameObjectData = 100;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	int a = 10;
}

HRESULT CBackGround::Render(_float fTimeDelta)
{
	/* 사각형위에 올리고 싶은 테긋쳐를 미리 장치에 바인딩한다.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	/* 로컬 정점을 변환하기위한 상태행렬(월드, 뷰, 투영)들을 셋팅한다. 가라!!!!!!! */
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	D3DXMatrixIdentity(&WorldMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

	D3DXMatrixLookAtLH(&ViewMatrix, &_float3(0.f, 0.f, -30.f), &_float3(0.f, 0.f, 0.f), &_float3(0.f, 1.f, 0.f));
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(60.0f), (_float)g_iWinSizeX / g_iWinSizeY, 0.1f, 1000.f);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::Ready_Components()
{
	///* 백그라운드 객체에게 필요한 컴포넌트들을 복제하여 저장해놓는다.*/
	//m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo")));
	//if (nullptr == m_pTextureCom)
	//	return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*		pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround*		pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);

	Safe_Release(m_pTextureCom);
}
