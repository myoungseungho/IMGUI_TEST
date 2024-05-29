#pragma once

#include "Client_Defines.h"
#include "Monkey_Statue.h"
#include "Block.h"

class CQuizMgr
{
public:
    CQuizMgr();
    virtual ~CQuizMgr() = default;

public:
    static CQuizMgr* Get_Instance();

    void Add_MonkeyStatue(CMonkey_Statue* pMonkeyStatue);
    void Add_Block(CBlock* pBlock);

    const vector<CMonkey_Statue*>& Get_vecMonkeyStatues();
    const vector<CBlock*>& Get_vecBlocks();

    _uint Find_Monkey_Index(CMonkey_Statue* pMonkey);
    _uint Find_Block_Index(CBlock* pBlock);

    void    Change_Block_State(_uint iMonkeyIndex);

public:
    vector<CMonkey_Statue*> m_vecMonkeyStatues;
    vector<CBlock*> m_vecBlocks;

public:
    static CQuizMgr* p_Instance;
};

