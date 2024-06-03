#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CKeyState;

END

BEGIN(Client)

class CUI_Npc_Talk final : public CUIObject
{	
private:
	struct ItemInfo {
		wstring name;
		wstring talk;
	};
private:
	CUI_Npc_Talk(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CUI_Npc_Talk(const CUI_Npc_Talk& Prototype); /* �纻���� �� */
	virtual ~CUI_Npc_Talk() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	void SetIsNpcTalkOn(_bool _isOn);
	void SetNpcTalkMessages(const vector<pair<wstring, wstring>>& messages);
	void AdvanceTalkCycle();
private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTexture*			m_pArrow_Down_TextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CKeyState* m_pKeyCom = { nullptr };
	LPD3DXFONT			m_pName_Font = { nullptr };
	LPD3DXFONT			m_pTalk_Font = { nullptr };

private:
	HRESULT Ready_Components();
	void Font_Initialize();
	
private:
	wstring m_WstringName = { TEXT("���ȣ ���� �ϱ�") };
	wstring m_WstringTalk = TEXT(
		"���� ���� ¥����� �Ծ����ϴ�.\n"
		"�����ε� �Ծ����ϴ�..\n"
		"���� ���� �谡 �θ��ϴ�.\n"
		"������� �԰�ͳ׿�"
	);

	vector<pair<wstring, wstring>> m_vecMessages;
	_uint m_CurrentMessageIndex = { 0 };
	wstring m_DisplayText;
	_float m_TextPosX = 50.0f; // �ؽ�Ʈ x ��ǥ
	_float m_TextPosY = 50.0f; // �ؽ�Ʈ y ��ǥ
	_float m_fTextUpdateTime = {0.f};
	_float m_fTextDisplayInterval= { 0.1f };
	size_t m_CurrentCharIndex = { 0 };

	//ũ�� �ִϸ��̼�
	_float m_fCreateTime = 0.0f; // ��ü�� ������ �ð�
	_float m_fGrowthDuration = 0.5f; // n�ʰ� Ŀ���� �ð�
	_bool m_bGrowthComplete = false; // ���� �Ϸ� ����
	_bool m_bIsShrinking = { false };
	
public:
	_bool m_bIsNpcTalkOn = { false };
private:
	_float m_fArrowOffsetY = 0.f; // ȭ��ǥ�� Y ������
	_float m_fArrowAnimDuration = 0.5f; // �ִϸ��̼� �ð�
	_float m_fArrowAnimTime = 0.0f; // �ִϸ��̼� ��� �ð�
	_float m_fArrowOffsetRange = 10.0f; // ȭ��ǥ�� Y �����
	_bool m_bArrowAnimUp = true; // ȭ��ǥ�� �ö󰡴� ������ ����
public:
	/* ������ü�� �����Ѵ�. */
	static CUI_Npc_Talk* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END