#pragma once

#include "Client_Defines.h"
#include "Monkey_Statue.h"
#include "Block.h"

class CQuizMgr
{
private:
    CQuizMgr();
    virtual ~CQuizMgr() = default;

public:
    static CQuizMgr* Get_Instance();

    void Add_MonkeyStatue(CMonkey_Statue* pMonkeyStatue);
    void Add_Block(CBlock* pBlock);

    const vector<CMonkey_Statue*>& Get_vecMonkeyStatues();
    const vector<CBlock*>& Get_vecBlocks();

    CMonkey_Statue* Find_Monkey(CMonkey_Statue* pMonkey);
    CBlock* Find_Block(CBlock* pBlock);

public:
    vector<CMonkey_Statue*> m_vecMonkeyStatues;
    vector<CBlock*> m_vecBlocks;

public:
    static CQuizMgr* p_Instance;
};

