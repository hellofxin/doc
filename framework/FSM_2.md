### 方式1
```
//比如我们定义了小明一天的状态如下
enum
{
  GET_UP,
  GO_TO_SCHOOL,
  HAVE_LUNCH,
  GO_HOME,
  DO_HOMEWORK,
  SLEEP,
};


int main()
{
  int state = GET_UP;
  //小明的一天
  while (1)
  {
    if (state == GET_UP)
    {
      GetUp(); //具体调用的函数
      state = GO_TO_SCHOOL; //状态的转移
    }
    else if (state == GO_TO_SCHOOL)
    {
      Go2School();
      state = HAVE_LUNCH;
    }
    else if (state == HAVE_LUNCH)
    {
      HaveLunch();
    }
    ...
    else if (state == SLEEP)
    {
      Go2Bed();
      state = GET_UP;
    }
  }

  return 0;
}
```

### 方式2
```
int main()
{
  int state = GET_UP;
  //小明的一天
  while (1)
  {

    switch(state)
    {
    case GET_UP:
      GetUp(); //具体调用的函数
      state = GO_TO_SCHOOL; //状态的转移
      break;
    case GO_TO_SCHOOL:
      Go2School();
      state = HAVE_LUNCH;
      break;
    case HAVE_LUNCH:
      HaveLunch();
      state = GO_HOME;
      break;
      ...
    default:
      break;
    }
  }

  return 0;
}
```

### 方式3
```
//比如我们定义了小明一天的状态如下
enum
{
  GET_UP,
  GO_TO_SCHOOL,
  HAVE_LUNCH,
  DO_HOMEWORK,
  SLEEP,
};

enum
{
  EVENT1 = 1,
  EVENT2,
  EVENT3,
};

typedef struct FsmTable_s
{
  int event;  //事件
  int CurState; //当前状态
  void (*eventActFun)(); //函数指针
  int NextState; //下一个状态
}FsmTable_t;

FsmTable_t XiaoMingTable[] =
{
  //{到来的事件，当前的状态，将要要执行的函数，下一个状态}
  { EVENT1, SLEEP,      GetUp,    GET_UP },
  { EVENT2, GET_UP,     Go2School,  GO_TO_SCHOOL },
  { EVENT3, GO_TO_SCHOOL,  HaveLunch,  HAVE_LUNCH },
  { EVENT1, HAVE_LUNCH,   DoHomework,  DO_HOMEWORK },
  { EVENT2, DO_HOMEWORK,   Go2Bed,    SLEEP },

  //add your codes here
};

/*状态机注册*/
void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable)
{
  pFsm->FsmTable = pTable;
}

/*状态迁移*/
void FSM_StateTransfer(FSM_t* pFsm, int state)
{
  pFsm->curState = state;
}

/*事件处理*/
void FSM_EventHandle(FSM_t* pFsm, int event)
{
  FsmTable_t* pActTable = pFsm->FsmTable;
  void (*eventActFun)() = NULL; //函数指针初始化为空
  int NextState;
  int CurState = pFsm->curState;
  int flag = 0; //标识是否满足条件
  int i;

  /*获取当前动作函数*/
  for (i = 0; i<g_max_num; i++)
  {
    //当且仅当当前状态下来个指定的事件，我才执行它
    if (event == pActTable[i].event && CurState == pActTable[i].CurState)
    {
      flag = 1;
      eventActFun = pActTable[i].eventActFun;
      NextState = pActTable[i].NextState;
      break;
    }
  }


  if (flag) //如果满足条件了
  {
    /*动作执行*/
    if (eventActFun)
    {
      eventActFun();
    }

    //跳转到下一个状态
    FSM_StateTransfer(pFsm, NextState);
  }
  else
  {
    // do nothing
  }
}

int main()
{
  FSM_t fsm;
  InitFsm(&fsm);
  int event = EVENT1; 
  //小明的一天,周而复始的一天又一天，进行着相同的活动
  while (1)
  {
    printf("event %d is coming...\n", event);
    FSM_EventHandle(&fsm, event);
    printf("fsm current state %d\n", fsm.curState);
    test(&event); 
    sleep(1); //休眠1秒，方便观察
  }

  return 0;
}
```