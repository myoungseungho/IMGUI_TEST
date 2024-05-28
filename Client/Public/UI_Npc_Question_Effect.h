#pragma once

#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CUI_Npc_Question_Effect final : public CGameObject
{
public:
	typedef struct
	{
		_float3 position = {};
		_float3 scale = {};
	}EFFECT_DESC;

private:
	CUI_Npc_Question_Effect(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CUI_Npc_Question_Effect(const CUI_Npc_Question_Effect& Prototype); /* �纻���� �� */
	virtual ~CUI_Npc_Question_Effect() = default;

public:
	void SetIsOn(bool _isOn);
	

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	void AnimState(_float _fTimeDelta);
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();


private:
	CAnimator* m_pAnimCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float m_fAlpha = { 0.f };               // Current alpha value
	_bool m_bIncreasingAlpha = { true };     // Is alpha value increasing
	_float m_fElapsedTime = { 0.f };       // Elapsed time for alpha animation
	_float m_fAlphaAnimationDuration = { 1.f }; // Duration for alpha increase/decrease
	_float m_fMinAlpha = { 0.f };           // Minimum alpha value
	_float m_fMaxAlpha = { 255.f };         // Maximum alpha value


	//���� �ִϸ��̼� ��ġ
	_bool m_bIsOn = false;
	_bool m_bIsAnimatingUp = false;
	_bool m_bIsAnimatingDown = false;
	_float m_fAnimationDuration = 1.0f; // �ִϸ��̼� ���� �ð� (n��)
	_float m_fStartY = 0.0f; // �ʱ� Y ��ġ
	_float m_fEndY = 10.0f; // ��ǥ Y ��ġ (y������ 10��ŭ �̵�)

public:
	/* ������ü�� �����Ѵ�. */
	static CUI_Npc_Question_Effect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END