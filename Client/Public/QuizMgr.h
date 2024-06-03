#pragma once

#include "Client_Defines.h"
#include "Monkey_Statue.h"
#include "Block.h"



class CQuizMgr : public CBase
{
public:
    CQuizMgr();
    virtual ~CQuizMgr();
    void Destroy_QuizInstance();

public:
    static CQuizMgr* Get_QuizInstance();

    void Add_MonkeyStatue(CMonkey_Statue* pMonkeyStatue);
    void Add_Block(CBlock* pBlock);

    _uint Find_Monkey_Index(CMonkey_Statue* pMonkey);
    _uint Find_Block_Index(CBlock* pBlock);

    void    Set_Block_State();
    void    Change_Block_State(_uint iMonkeyIndex);

public:
    vector<CMonkey_Statue*> m_vecMonkeyStatues;
    vector<CBlock*> m_vecBlocks;

    _bool m_First_State = { true };
public:
    void Free() override;
    static CQuizMgr* p_QuizInstance;
};

