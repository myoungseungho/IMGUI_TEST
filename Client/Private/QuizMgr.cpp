#include "stdafx.h"
#include "QuizMgr.h"


CQuizMgr* CQuizMgr::p_QuizInstance = { nullptr };

CQuizMgr::CQuizMgr()
{
}

CQuizMgr::~CQuizMgr()
{
}

void CQuizMgr::Destroy_QuizInstance()
{
	if (p_QuizInstance)
	{
		delete p_QuizInstance;
		p_QuizInstance = nullptr;
	}
}

CQuizMgr* CQuizMgr::Get_QuizInstance()
{
	if (!p_QuizInstance)
		p_QuizInstance = new CQuizMgr;

	return p_QuizInstance;
}

void CQuizMgr::Add_MonkeyStatue(CMonkey_Statue* pMonkeyStatue)
{
	m_vecMonkeyStatues.push_back(pMonkeyStatue);
}

void CQuizMgr::Add_Block(CBlock* pBlock)
{
	m_vecBlocks.push_back(pBlock);
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

void CQuizMgr::Set_Block_State()
{
	if (m_First_State)
	{
		m_vecBlocks[1]->First_State();
		m_vecBlocks[2]->First_State();
		m_vecBlocks[4]->First_State();
		m_vecBlocks[6]->First_State();
		m_vecBlocks[8]->First_State();

		m_First_State = false;
	}
}

void CQuizMgr::Change_Block_State(_uint iMonkeyIndex)
{
	if (iMonkeyIndex == 0)
	{
		m_vecBlocks[0]->Change_State();
		m_vecBlocks[6]->Change_State();
	}
	else if (iMonkeyIndex == 1)
	{
		m_vecBlocks[4]->Change_State();
		m_vecBlocks[9]->Change_State();
	}
	else if (iMonkeyIndex == 2)
	{
		m_vecBlocks[1]->Change_State();
		m_vecBlocks[5]->Change_State();
	}
	else if (iMonkeyIndex == 4)
	{
		m_vecBlocks[2]->Change_State();
		m_vecBlocks[7]->Change_State();
	}
	else if (iMonkeyIndex == 3)
	{
		m_vecBlocks[3]->Change_State();
		m_vecBlocks[8]->Change_State();
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
