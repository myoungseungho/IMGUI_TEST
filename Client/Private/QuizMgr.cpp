#include "QuizMgr.h"

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

CMonkey_Statue* CQuizMgr::Find_Monkey(CMonkey_Statue* pMonkey)
{

}

CBlock* CQuizMgr::Find_Block(CBlock* pBlock)
{

}
