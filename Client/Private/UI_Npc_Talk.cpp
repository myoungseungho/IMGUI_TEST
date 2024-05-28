#include "stdafx.h"
#include "..\Public\UI_Npc_Talk.h"
#include <sstream>
#include "GameInstance.h"

CUI_Npc_Talk::CUI_Npc_Talk(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CUI_Npc_Talk::CUI_Npc_Talk(const CUI_Npc_Talk& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUI_Npc_Talk::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

void CUI_Npc_Talk::Font_Initialize()
{
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	// 폰트 설정 - m_pName_Font
	tFontInfo.Height = 40;
	tFontInfo.Width = 30;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	wcscpy_s(tFontInfo.FaceName, LF_FACESIZE, TEXT("Cafe24 Ssurround air OTF Light"));

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pName_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentPlayerMoney_Font Failed");
		return;
	}

	// 폰트 설정 - m_pTalk_Font
	tFontInfo.Height = 30;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	// 이미 tFontInfo가 초기화되어 있으므로, 필요한 부분만 변경합니다.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pTalk_Font)))
	{
		MSG_BOX(L"CreateFontIndirect for CurrentItemTitle_Font Failed");
		return;
	}
}

HRESULT CUI_Npc_Talk::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 760.f;
	m_fSizeY = 330.f;
	m_fX = -15.f;
	m_fY = -145.f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f)); // 초기 크기를 0으로 설정
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_fX, m_fY, 0.f));

	Font_Initialize();

	m_fAlpha = 190.f;
	return S_OK;
}

void CUI_Npc_Talk::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsNpcTalkOn) return; // m_bIsOn이 false이면 업데이트를 수행하지 않음
}

void CUI_Npc_Talk::Update(_float fTimeDelta)
{
	if (!m_bIsNpcTalkOn) return; // 업데이트가 비활성화된 경우 반환

	// 생성된 후 경과 시간 계산
	m_fCreateTime += fTimeDelta;

	// 화살표 애니메이션 처리
	if (m_bGrowthComplete)
	{
		m_fArrowAnimTime += fTimeDelta;
		if (m_fArrowAnimTime >= m_fArrowAnimDuration)
		{
			m_fArrowAnimTime = 0.0f;
			m_bArrowAnimUp = !m_bArrowAnimUp; // 방향 전환
		}

		_float progress = m_fArrowAnimTime / m_fArrowAnimDuration;
		if (m_bArrowAnimUp)
		{
			m_fArrowOffsetY = m_fArrowOffsetRange * progress; // 위로 이동
		}
		else
		{
			m_fArrowOffsetY = m_fArrowOffsetRange * (1.0f - progress); // 아래로 이동
		}
	}

	// Enter 키 입력 처리
	if (m_pKeyCom->Key_Down(VK_RETURN))
	{
		AdvanceTalkCycle();
	}

	// 창 커지거나 작아지는것
	if (m_bIsShrinking)
	{
		// 객체가 n초간 작아지면서 사라지도록 처리
		if (m_fCreateTime <= m_fGrowthDuration)
		{
			_float shrinkProgress = m_fCreateTime / m_fGrowthDuration;
			_float currentSizeX = m_fSizeX * (1.0f - shrinkProgress);
			_float currentSizeY = m_fSizeY * (1.0f - shrinkProgress);

			m_pTransformCom->Set_Scaled(_float3(currentSizeX, currentSizeY, 1.f));
		}
		else
		{
			// 축소 완료 후 크기 0으로 설정
			m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f)); // 초기 크기를 0으로 설정
			m_bIsNpcTalkOn = false; // 축소 애니메이션 완료 후에 비활성화
		}
	}
	else
	{
		// 객체가 n초간 커지면서 시작되도록 처리
		if (m_fCreateTime <= m_fGrowthDuration)
		{
			_float growthProgress = m_fCreateTime / m_fGrowthDuration;
			_float currentSizeX = m_fSizeX * growthProgress;
			_float currentSizeY = m_fSizeY * growthProgress;

			m_pTransformCom->Set_Scaled(_float3(currentSizeX, currentSizeY, 1.f));
		}
		else
		{
			// 성장 완료 후 최종 크기와 위치로 설정  
			m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
			m_bGrowthComplete = true; // 성장 완료 상태 설정
		}
	}






	if (GetAsyncKeyState('F') & 0x8000) {
		offsetX -= 5.f;
	}
	if (GetAsyncKeyState('H') & 0x8000) {
		offsetX += 5.f;
	}
	if (GetAsyncKeyState('T') & 0x8000) {
		offsetY += 5.f;
	}
	if (GetAsyncKeyState('G') & 0x8000) {
		offsetY -= 5.f;
	}
	if (GetAsyncKeyState('R') & 0x8000) {
		offsetZ -= 0.01f;
	}
	if (GetAsyncKeyState('Y') & 0x8000) {
		offsetZ += 0.01f;
	}
	if (GetAsyncKeyState('J') & 0x8000) {
		offsetXScale -= 5.f;
	}
	if (GetAsyncKeyState('K') & 0x8000) {
		offsetXScale += 5.f;
	}
	if (GetAsyncKeyState('N') & 0x8000) {
		offsetYScale -= 5.f;
	}
	if (GetAsyncKeyState('M') & 0x8000) {
		offsetYScale += 5.f;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_fAlpha += 1.f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		m_fAlpha -= 1.f;
	}



	//// 키 입력 처리
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//{
	//	m_TextPosY -= 10.f; // 위쪽으로 이동
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	m_TextPosY += 10.f; // 아래쪽으로 이동
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_TextPosX -= 10.f; // 왼쪽으로 이동
	//}
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_TextPosX += 10.f; // 오른쪽으로 이동
	//}
}

void CUI_Npc_Talk::Late_Update(_float fTimeDelta)
{
	if (!m_bIsNpcTalkOn)
		return;

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Npc_Talk::Render(_float fTimeDelta)
{
	if (!m_bIsNpcTalkOn && !m_bIsShrinking)
		return S_OK;

	__super::Begin_RenderState();

	wchar_t text[256];
	RECT rect;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	_float4x4 backUpWorldMatrix = m_pTransformCom->Get_WorldMatrix();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	/*_float3 _arrowPosition = { offsetX,offsetY,0 };
	_float3 _scale = { offsetXScale,offsetYScale,0 };*/

	

	if (m_bGrowthComplete)
	{
		// 텍스트 렌더링 - 이름
		swprintf_s(text, L"%s", m_WstringName.c_str());
		SetRect(&rect, static_cast<int>(280.f), static_cast<int>(375.f), 0, 0); // 텍스트를 출력할 위치 변경
		m_pName_Font->DrawText(
			NULL,
			text,
			-1,
			&rect,
			DT_NOCLIP,
			D3DCOLOR_ARGB(255, 169, 169, 144)
		);

		// 텍스트 렌더링 - CurrentItemExplain
		m_fTextUpdateTime += fTimeDelta;
		if (m_fTextUpdateTime >= m_fTextDisplayInterval)
		{
			m_fTextUpdateTime = 0.0f;
			if (m_CurrentCharIndex < m_WstringTalk.length())
			{
				m_DisplayText += m_WstringTalk[m_CurrentCharIndex];
				++m_CurrentCharIndex;
			}
		}

		wistringstream wiss(m_DisplayText);
		wstring line;
		int lineIndex = 0;
		int startY = 460; // 초기 Y 위치

		while (getline(wiss, line))
		{
			RECT lineRect = { 0, 0, 0, 0 };
			m_pTalk_Font->DrawText(
				NULL,
				line.c_str(),
				-1,
				&lineRect,
				DT_CALCRECT,
				0
			);
			int lineWidth = lineRect.right - lineRect.left;
			int centerX = 640; // 중앙 정렬 기준 X 좌표
			int startX = centerX - (lineWidth / 2);
			SetRect(&rect, startX, startY + (lineIndex * 30), startX + lineWidth, 0); // 각 줄의 Y 간격은 30 픽셀
			m_pTalk_Font->DrawText(
				NULL,
				line.c_str(),
				-1,
				&rect,
				DT_NOCLIP,
				D3DCOLOR_ARGB(255, 255, 255, 255)
			);
			lineIndex++;
		}

		_float3 _arrowPosition = { -10.f, -220.f + m_fArrowOffsetY, 0 };
		_float3 _scale = { 35.f, 25.f, 0 };

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_arrowPosition);
		m_pTransformCom->Set_Scaled(_scale);

		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_pArrow_Down_TextureCom->Bind_Texture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (_float3*)&backUpWorldMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, (_float3*)&backUpWorldMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, (_float3*)&backUpWorldMatrix.m[2][0]);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (_float3*)&backUpWorldMatrix.m[3][0]);

		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;
	}

	__super::End_RenderState();

	return S_OK;
}

void CUI_Npc_Talk::SetNpcTalkMessages(const vector<pair<wstring, wstring>>& messages)
{
	m_vecMessages = messages;
	m_CurrentMessageIndex = 0;
	AdvanceTalkCycle();
}

void CUI_Npc_Talk::AdvanceTalkCycle()
{
	if (m_CurrentMessageIndex < m_vecMessages.size())
	{
		m_WstringName = m_vecMessages[m_CurrentMessageIndex].first;
		m_WstringTalk = m_vecMessages[m_CurrentMessageIndex].second;
		m_DisplayText.clear();
		m_CurrentCharIndex = 0;
		m_fTextUpdateTime = 0.0f;
		m_CurrentMessageIndex++;
	}
	else
	{
		SetIsNpcTalkOn(false);
	}
}


void CUI_Npc_Talk::SetIsNpcTalkOn(_bool _isOn)
{
	m_fCreateTime = 0.0f;

	if (_isOn)
	{
		m_bIsShrinking = false;
		m_bGrowthComplete = false;
		m_CurrentCharIndex = 0;
		m_DisplayText.clear();
		m_bIsNpcTalkOn = true;
	}
	else
	{
		m_bIsShrinking = true;
		m_bGrowthComplete = false;
	}
}

HRESULT CUI_Npc_Talk::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Test_UITextBox_TextBox"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sprite_UI_MainComponents_ArrowDown"),
		TEXT("Com_Arrow_Texture"), reinterpret_cast<CComponent**>(&m_pArrow_Down_TextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_KeyState */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_KeyState"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CUI_Npc_Talk* CUI_Npc_Talk::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Npc_Talk* pInstance = new CUI_Npc_Talk(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Npc_Talk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Npc_Talk::Clone(void* pArg)
{
	CUI_Npc_Talk* pInstance = new CUI_Npc_Talk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Npc_Talk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Npc_Talk::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pArrow_Down_TextureCom);
	Safe_Release(m_pName_Font);
	Safe_Release(m_pTalk_Font);
	__super::Free();
}
