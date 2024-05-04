#include "Graphic_Device.h"

CGraphic_Device::CGraphic_Device() 
	: m_pSDK{ nullptr }
	, m_pDevice{ nullptr }
	, m_pSprite{ nullptr }
	, m_pFont{ nullptr }
{

}

HRESULT CGraphic_Device::Init_Device(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	// 1. 장치를 조사할 객체를 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. 장치를 조사(지원 수준 여부 판단)
	// HAL(Hardware Abstaction Layer) : 하드웨어 추상화 계층

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 기록된 수 많은 지원 수준 중에 앞으로 3D 렌더링 연산에 필요한 기본적인 지원 수준을 조사
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD	vp(0);
	// 버텍스 프로세싱 = 정점 변환 + 조명 연산

	/* 정점 변환 : 장치에 월드, 뷰, 투영행렬을 바인딩하고 로컬정의된 정점을 그리면. */
	/* 조명 연산 : 빛 정보를 장치에 바인딩해주면. 내 정점에 빛 연산한 결과를 포함하여 그려준다. */

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	// 3. 그래픽 제어 객체를 생성
	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp, hWnd, iWinSizeX, iWinSizeY);

	// CreateDevice : 그래픽 제어 객체 생성 함수
	// CreateDevice(어떤 그래픽 카드를 제어할 것인가, 어떤 정보로 장치에 접근 할 것인가, 장치를 사용할 윈도우, 동작 방식, 사용환경, 생성할 객체)

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									vp, 
									&d3dpp, 
									&m_pDevice)))
	{
		MSG_BOX(L"CreateDevice Failed");
		return E_FAIL;
	}

	// 스프라이트 컴 객체 생성

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// 폰트 컴 객체 생성

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSG_BOX(L"CreateFontIndirect");
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);
	
	return S_OK;
}

// 후면 버퍼의 동작 방식

void CGraphic_Device::Render_Begin()
{
	// 지우기
	// 이제 그리기 시작

	m_pDevice->Clear(0,			// rect 배열의 원소 개수
					nullptr,	// rect들을 저장한 배열의 첫 주소(null인 경우 전체 영역을 의미)
					D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,   // 지우고자 하는 그리기 버퍼
					D3DCOLOR_ARGB(255, 0, 0, 255),  // 백버퍼 텍스처 색상
					1.f, // 깊이 버퍼 초기화 값
					0); // 스텐실 버퍼 초기화 값

	m_pDevice->BeginScene();

	// m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

//0.7		SRCBLEND		INVSRCALPHA(1.f - 0.7f)-> 0.3f			DESTBLEND
//A(RGB)	-> AR, AG, AB	+	A`(R`G`B`) ->A`R` A`B` A`B`

void CGraphic_Device::Render_End(HWND hWnd)
{
	// 여기까지 그리기 완료
	// 교환
	m_pDevice->EndScene();


	m_pDevice->Present(NULL, NULL, hWnd, NULL);

	// 1, 2 인자 : 렉트 주소
	// 3인자 : 출력 대상 윈도우 핸들(NULL인 경우 Set_Parameters함수에서 설정한 값이 들어감)
	// 4인자 : 몰라요(1, 2, 4인자 모두 swapeffect가 copy였을 때만 사용 가능)
}

void CGraphic_Device::Set_Parameters(D3DPRESENT_PARAMETERS & d3dpp, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	d3dpp.BackBufferWidth  = iWinSizeX;
	d3dpp.BackBufferHeight = iWinSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	
	// D3DSWAPEFFECT_DISCARD = 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP = 버퍼 하나로 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY = 더블 버퍼링과 유사한 복사 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// 장치를 사용할 윈도우 핸들 지정
	d3dpp.hDeviceWindow = hWnd;
	
	d3dpp.Windowed = TRUE;		// 창 모드(TRUE)로 실행할 것인가 전체 화면 모드(FALSE)로 실행할 것인가
	
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	// 전체화면 시에만 적용
	// 창 모드일 경우 OS가 재생률 관리
	// 전체화면 모드시 그래픽 카드가 관여

	// 전체화면 모드일 모니터 화면 갱신 주기 설정(현재 모니터 재생률을 토대로 알아서 수행)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	
	// 모니터 재생률과 프로그램 시연 간격 설정
	// D3DPRESENT_INTERVAL_DEFAULT : DX가 알아서 결정(모니터 재생률을 따라감)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 
}

CGraphic_Device * CGraphic_Device::Create(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	CGraphic_Device*		pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Init_Device(hWnd, iWinSizeX, iWinSizeY, ppOut)))
	{
		MSG_BOX(TEXT("Failed to Created : CGraphic_Device"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGraphic_Device::Free()
{
	__super::Free();

	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	if (0 < Safe_Release(m_pDevice))
	{
		MSG_BOX(TEXT("Memory leak Detected : IDirect3DDevice9"));
	}
	Safe_Release(m_pSDK);
}
