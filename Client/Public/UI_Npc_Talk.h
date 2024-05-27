#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
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

private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
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
	wstring m_DisplayText;
	_float m_TextPosX = 50.0f; // �ؽ�Ʈ x ��ǥ
	_float m_TextPosY = 50.0f; // �ؽ�Ʈ y ��ǥ
	_float m_fTextUpdateTime = {0.f};
	_float m_fTextDisplayInterval= { 0.1f };
	size_t m_CurrentCharIndex = { 0 };
public:
	/* ������ü�� �����Ѵ�. */
	static CUI_Npc_Talk* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END