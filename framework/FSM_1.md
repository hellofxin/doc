## FSM设计与实现

有限状态机（FSM）是表示有限个状态及在这些状态之间的转移和动作等行为的数学模型，在计算机领域有着广泛的应用。通常FSM包含几个要素：状态的管理、状态的监控、状态的触发、状态触发后引发的动作(state, event, guard, transition, action)。本文主要阐述一下状态机的几种设计方法。

### 简单 if else
这种设计方法最简单，通过一大堆判断来处理，适合小规模的状态切换流程，但如果规模扩大难以扩展和维护。

```
curEvent = getEvent();
curState = getCurState();
switch(curState)
{
    case state1:
    {
        switch(curEvent )
        {
             TODO...
             setCurState();
             break; 
        }
        break;
    }
    ...
}
```

### 基于表结构的状态机设计方法
建立相应的状态表和动作查询表，根据状态表、事件、动作表定位相应的动作处理函数，执行完成后再进行状态的切换。
一个通用的状态机处理模块的设计如下：

```
/*状态表注册*/void FSM_Regist(FSM_T* pFsm,STATE_TABLE_S* pStateTable)
{
   pFsm->FsmTable =  pStateTable;
   return;
}
/*状态迁移*/
void FSM_MoveState(FSM_T* pFsm,int state)
{
   pFsm->curState = state;
   return;
}/*事件处理*/
void FSM_EventHandle(FSM_T* pFsm,int event)
{
    ACT_TABLE_T* pActTable = NULL;
    ActFun eventActFun = NULL;
    /*获取当前状态动作表*/
    pActTable = FSM_getActTable(pFsm);
    /*获取当前动作函数*/
    for(int i=0;i<MAX_ACT_NUM;i++)
    {
        if(event == pActTable[i].event)
        {
            eventActFun = pActTable[i].eventActFun;
            break;
        }
    }
    /*动作执行*/
    if(eventActFun)
    {
        eventActFun(pFsm);
    }
}
```

实例
```
/*状态1的动作表*/
ACT_TABLE_T state1ActTable[] = {
    {EVENT1,state1Event1Fun},
    {EVENT3,state1Event3Fun},
};
/*状态2的动作表*/
ACT_TABLE_T state2ActTable[] = {
    {EVENT2,state2Event2Fun},
};
/*状态表*/
STATE_TABLE_T FsmTable[] = {
    {STATE1,state1ActTable},
    {STATE2,state2ActTable},
};

int main(int argc, _TCHAR* argv[])
{
    FSM_T fsm;
    /*状态表注册*/
    FSM_Regist(&fsm,FsmTable);
    FSM_MoveState(&fsm,STATE1);
    FSM_EventHandle(&fsm,EVENT1);
    FSM_EventHandle(&fsm,EVENT2);
    return 0;
}/*客户端提供的状态处理函数*/

void state1Event1Fun(void* pFsm)
{
    FSM_MoveState((FSM_T*)pFsm,STATE2);    return;
}
void state1Event3Fun(void* pFsm)
{
    FSM_MoveState((FSM_T*)pFsm,STATE3);    return;
}
void state2Event2Fun(void* pFsm)
{
    FSM_MoveState((FSM_T*)pFsm,STATE3);    return;
}
```

通过设计一个通用的基于表结构的状态机模块，针对不同的状态图，我们只需要根据状态图得到其状态表结构，然后通过FSM_Regist注册，就可以方便的使用了状态机的功能了。这种机制便于我们添加新的状态流程，并且可以很好的进行分层状态机的设计。