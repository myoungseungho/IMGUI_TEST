#include "..\Public\Texture.h"


CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{

}

CTexture::CTexture(const CTexture & rhs)
	: CComponent{ rhs }
	, m_iNumTextures{ rhs.m_iNumTextures }
	, m_Textures{ rhs.m_Textures }	
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Initialize_Prototype(TYPE eType, const _wstring & strTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;


	/* "../Bin/Resources/Textures/Explosion/Explosion%d.png" */
	/* "../Bin/Resources/Textures/Snow/Snow.png" */

	/* �ؽ��ĸ� �ε��Ͽ� m_Textures�� �����Ѵ�. */
	for (size_t i = 0; i < iNumTextures; i++)
	{
		LPDIRECT3DBASETEXTURE9			pTexture = { nullptr };

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, strTextureFilePath.c_str(), i);		

		HRESULT hr = TYPE_TEXTURE2D == eType ? D3DXCreateTextureFromFile(m_pGraphic_Device, szTextureFilePath, (LPDIRECT3DTEXTURE9*)&pTexture) : D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szTextureFilePath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
		if (FAILED(hr))
			return E_FAIL;

		m_Textures.emplace_back(pTexture);		
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_Texture(_uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

 	return m_pGraphic_Device->SetTexture(0, m_Textures[iTextureIndex]);	
}

HRESULT CTexture::Bind_Anim(_uint fFrame)
{
	//� �ϳ��� �ִϸ��̼��� �ؽ���
	//�ִϸ��̼� �ؽ��� ��ü �庸�� ���� ����ǰ� �ִ�
	//�������� �� ũ��, ���� ����ϰ� �ִ� ������ �ܰ踦
	//0���� �ʱ�ȭ.
	
	if (fFrame >= m_iNumTextures)
		fFrame = 0;
	
	HRESULT hr = m_pGraphic_Device->SetTexture(0, m_Textures[fFrame]);
	//���� ����ϰ� �ִ� �������� �ؽ��ĸ� ��ġ�� ���ε�
	return hr;
}


void CTexture::ChangeTextureColor(D3DCOLOR originalColor, D3DCOLOR newColor)
{
	if (m_Textures.empty())
		return;

	IDirect3DTexture9* pTexture = static_cast<IDirect3DTexture9*>(m_Textures[0]);
	if (!pTexture)
		return;

	D3DLOCKED_RECT lockedRect;
	if (FAILED(pTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD)))
		return;

	BYTE* pTexels = (BYTE*)lockedRect.pBits;
	D3DSURFACE_DESC desc;
	pTexture->GetLevelDesc(0, &desc);

	for (UINT y = 0; y < desc.Height; ++y)
	{
		for (UINT x = 0; x < desc.Width; ++x)
		{
			BYTE* pPixel = pTexels + y * lockedRect.Pitch + x * 4;
			D3DCOLOR color = D3DCOLOR_ARGB(pPixel[3], pPixel[2], pPixel[1], pPixel[0]);

			if (color == originalColor)
			{
				pPixel[2] = (newColor >> 16) & 0xFF; // R
				pPixel[1] = (newColor >> 8) & 0xFF;  // G
				pPixel[0] = newColor & 0xFF;         // B
			}
		}
	}

	pTexture->UnlockRect(0);
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _wstring & strTextureFilePath, _uint iNumTextures)
{

	CTexture*		pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType, strTextureFilePath, iNumTextures)))
	{
		MSG_BOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}
