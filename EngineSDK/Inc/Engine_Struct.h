#pragma once
#include "Engine_Typedef.h"

namespace Engine
{
	/*D3DDECLUSAGE*/
	typedef struct
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord;
	}VTXPOSTEX;

	typedef struct
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexcoord;
	}VTXCUBE;

	//오브젝트 당 넘겨줄 정보 구조체
	typedef struct 
	{
		wstring prototypeTag;
		wstring layerName;
		_uint levelIndex;
		_float3 position;
		_float3 scale;
		bool isParsing;
	}FILEDATA;

	typedef struct
	{
		_float2 position;
		_float2 scale;
		_float alpha;
		_uint index;
	}UIDATA;

	typedef struct
	{
		_float2 position;
		_float2 scale;
		_float alpha;
		_uint index;
		_uint price;
	}PRICEUIDATA;

}