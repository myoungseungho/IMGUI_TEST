#include "Animator.h"

#include"Texture.h"
#include"GameInstance.h"

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGrpahic_Device)
	:CComponent{ pGrpahic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

CAnimator::CAnimator(const CAnimator& pPrototype)
	:CComponent{ pPrototype }
	, m_pGameInstance{ pPrototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CAnimator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimator::Initialize(void* pArg)
{
	return S_OK;
}



HRESULT CAnimator::Add_Animator(_uint iLevel, const wstring& strComponentTag, const wstring& strTextureTag)
{
	CTexture* pTexture = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(iLevel, strComponentTag));

	if (pTexture == nullptr)
		return E_FAIL;

	m_pTexture.emplace(strTextureTag, pTexture);

	return S_OK;
}

HRESULT CAnimator::Play_Animator(const wstring& strTextureTag, _float _totalAnimPlayTime, _float fTimeDelta, _bool isLoop)
{
	CTexture* pTexture = Find_Texture(strTextureTag);

	if (pTexture == nullptr)
		return E_FAIL;

	//내가 재생하려는 애니메이션 텍스쳐가, 이전까지 돌렸던 애니메이션 텍스쳐와 다를 경우
	//내가 재생하려는 애니메이션 텍스쳐의 시작점 (인덱스 0)을 초기화하기 위한 코드
	if (m_pPrevTexture != pTexture)
		m_iIndex = 0;

	//이제 내가 재생하려는 애니메이션 텍스쳐로 변경
	m_pPrevTexture = pTexture;

	//tFrame은 해당 애니메이션 텍스트의 전체 이미지를 몇초동안 돌릴지
	//내가 재생하려는 애니메이션 텍스쳐의 장수
	//한 프레임에 걸린 시간
	//한번 돌지 반복적으로 돌지 판단

	Move_Frame(_totalAnimPlayTime, pTexture->m_iNumTextures, fTimeDelta, isLoop);


	//장치에 해당 프레임의 텍스쳐를 바인딩 성공했는지 안했는지 여부 리턴
	return	pTexture->Bind_Anim(m_iIndex);
}

void CAnimator::Move_Frame(_float _totalAnimPlayTime, _uint iNumTextures, _float fTimeDelta, _bool isLoop)
{
	//하나의 애니메이션을 돌릴때 누적 시간값
	m_fTimeAcc += fTimeDelta;

	//텍스쳐 한장이 그리는데 걸리는 시간
	//이 시간보다 애니메이션을 돌리는데 누적된 시간이 더 크다면
	//즉, 사진 하나를 돌리는 시간보다 더 커졌다면
	//다음 사진 렌더를 위해서, 누적값 초기화 및 인덱스 증가
	if (m_fTimeAcc >= (_float)(_totalAnimPlayTime / iNumTextures))
	{
		m_fTimeAcc = 0.f;
		m_iIndex++;
	}

	//모든 텍스쳐를 다 재생시켰고, 루프를 계속 돌려야하면
	//인덱스는 0이되고
	//루프를 반복안하면 마지막 인덱스에서 머무른다.
	if (m_iIndex >= iNumTextures)
		m_iIndex = isLoop ? 0 : iNumTextures - 1;
}

CTexture* CAnimator::Find_Texture(const wstring& strTextureTag)
{
	auto	iter = m_pTexture.find(strTextureTag);

	if (iter == m_pTexture.end())
		return nullptr;

	return iter->second;
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimator* pInstance = new CAnimator(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Animator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAnimator::Clone(void* pArg)
{
	CAnimator* pInstance = new CAnimator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Animator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimator::Free()
{
	__super::Free();

	for (auto& Pair : m_pTexture)
		Safe_Release(Pair.second);

	m_pTexture.clear();

	Safe_Release(m_pPrevTexture);
	Safe_Release(m_pGameInstance);
}
