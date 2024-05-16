#pragma once

#define BEGIN(NAME)		namespace NAME {
#define END								}

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)

#else
#define ENGINE_DLL _declspec(dllimport)

#endif

#define MSG_BOX(message)								\
::MessageBox(nullptr, message, L"System Message", MB_OK)

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static unsigned int Destroy_Instance();				\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
unsigned int  ClassName::Destroy_Instance()				\
{														\
	unsigned int iRefCnt = { 0 };						\
	if(nullptr != m_pInstance)							\
	{													\
		iRefCnt = m_pInstance->Release();				\
		if(0 == iRefCnt)								\
			m_pInstance = nullptr;						\
	}													\
	return iRefCnt;										\
}

#define	VK_MAX	0xff

#define   WHITE (D3DCOLOR_XRGB(255, 255, 255));
#define   BLACK (D3DCOLOR_XRGB(0, 0, 0));
#define     RED (D3DCOLOR_XRGB(255, 0, 0));
#define   GREEN (D3DCOLOR_XRGB(0, 255, 0));
#define    BLUE (D3DCOLOR_XRGB(0, 0, 255));
#define  YELLOW (D3DCOLOR_XRGB(255, 255, 0));
#define    CYAN (D3DCOLOR_XRGB(0, 255, 255));
#define MAGENTA (D3DCOLOR_XRGB(255, 0, 255));

