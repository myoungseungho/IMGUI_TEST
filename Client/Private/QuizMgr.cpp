#include "stdafx.h"
#include "QuizMgr.h"


CQuizMgr* CQuizMgr::p_Instance = { nullptr };

CQuizMgr::CQuizMgr()
{
}

CQuizMgr::~CQuizMgr()
{
	Free();
}

void CQuizMgr::Destroy_Instance()
{
	if (p_Instance)
	{
		delete p_Instance;
		p_Instance = nullptr;
	}
}

CQuizMgr* CQuizMgr::Get_Instance()
{
	if (!p_Instance)
		p_Instance = new CQuizMgr;

	return p_Instance;
}

void CQuizMgr::Add_MonkeyStatue(CMonkey_Statue* pMonkeyStatue)
{
	m_vecMonkeyStatues.push_back(pMonkeyStatue);
}

void CQuizMgr::Add_Block(CBlock* pBlock)
{
	m_vecBlocks.push_back(pBlock);
}

const vector<CMonkey_Statue*>& CQuizMgr::Get_vecMonkeyStatues()
{
	return m_vecMonkeyStatues;
}

const vector<CBlock*>& CQuizMgr::Get_vecBlocks()
{
	return m_vecBlocks;
}

_uint CQuizMgr::Find_Monkey_Index(CMonkey_Statue* pMonkey)
{
	auto it = find(m_vecMonkeyStatues.begin(), m_vecMonkeyStatues.end(), pMonkey);

	if (it != m_vecMonkeyStatues.end())
	{
		return distance(m_vecMonkeyStatues.begin(), it);
	}
	else
		return -1;
}
_uint CQuizMgr::Find_Block_Index(CBlock* pBlock)
{
	auto it = find(m_vecBlocks.begin(), m_vecBlocks.end(), pBlock);

	if (it != m_vecBlocks.end())
	{
		return distance(m_vecBlocks.begin(), it);
	}
	else
		return -1;
}

void CQuizMgr::Change_Block_State(_uint iMonkeyIndex)
{
	if (iMonkeyIndex == 3)
	{
		m_vecBlocks[7]->Change_State();
	}
}

void CQuizMgr::Free()
{
	for (auto block : m_vecBlocks)
	{
		Safe_Release(block);
	}
	m_vecBlocks.clear();
	m_vecBlocks.shrink_to_fit();

	for (auto monkey : m_vecMonkeyStatues)
	{
		Safe_Release(monkey);
	}
	m_vecMonkeyStatues.clear();
	m_vecMonkeyStatues.shrink_to_fit();
}
