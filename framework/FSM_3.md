## 问题背景
为了更好地描述状态机的应用，这里用一个地铁站的闸机为背景，简单叙述一下闸机的工作流程： 通常闸机默认是关闭的，当闸机检测到有效的卡片信息后，打开闸机，当乘客通过后，关闭闸机；如果有人非法通过，那么闸机就会产生报警，如果闸机已经打开，而乘客仍然在刷卡，那么闸机将会显示票价和余额，并在屏幕输出“请通过，谢谢”。 在了解了闸机的工作流程之后，我们就可以画出闸机的状态图

## 代码实现
### 嵌套的 switch 语句
使用嵌套的 switch 语句是最为直接的办法，也是最容易想的方法，第一层 switch 用于状态管理，第二层 switch 用于管理各个状态下的各个事件。代码实现可以用下述伪代码来实现：

```
switch(当前状态)
{
case LOCKED 状态：
    switch(事件)：
    {
    case card 事件：
         切换至 UNLOCKED 状态;
         执行 unlock 动作;
         break;
    case pass 事件：
        执行 alarm 动作;
        break;
    }
    break;
case UNLOCKED 状态：
    switch(事件):
    {
        case card 事件:
            执行 thankyou 动作;
            break;
        case pass 事件:
            切换至 LOCKED 状态;
            执行 lock 动作;
            break;
    }
    break;
}
```

上述代码虽然很直观，但是状态和事件都出现在一个处理函数中，对于一个大型的 FSM 中，可能存在大量的状态和事件，那么代码量将是非常冗长的。为了解决这个问题，可以采用状态转移表的方法来处理。

### 状态转移表
为了减少代码的长度，可以使用查表法，将各个信息存放于一个表中，根据事件和状态查找表项，找到需要执行的动作以及即将转换的状态。


```
typedef struct _transition_t
{
    状态;
    事件;
    转换为新的状态;
    执行的动作;
}transition_t;

transition_t transitions[] = {
    {LOCKED 状态,card 事件,状态转换为UNLOCKED,unlock动作},
    {LOCKED 状态,pass 事件,状态保持为LOCKED,alarm 动作},
    {UNLOCKED 状态,card 事件,状态转换为 UNLOCKED，thankyou动作},
    {UNLOCKED 状态,pass 事件,状态转换为 LOCKED,lock 动作}
};

for (int i = 0;i < sizeof(transition)/sizeof(transition[0]);i++)
{
    if (当前状态 == transition[i].状态 && 事件 == transition[i].事件)
    {
        切换状态为:transition[i].转换为新的状态;
        执行动作:transition[i].执行的动作;
        break;
    }
}
```

从上述我们可以看到如果要往状态机中添加新的流程，那么只需要往状态表中添加东西就可以了，也就是说整个状态机的维护及管理只需要把重心放到状态转移表的维护中就可以了，从代码量也可以看出来，采用状态转移表的方法相比于第一种方法也大大地缩减了代码量，而且也更容易维护。 **但是对于状态转移表来说，缺点也是显而易见的，对于大型的 FSM 来说，遍历状态转移表需要花费大量的时间，从而影响代码的执行效率。** 那要怎样设计代码量少，又不需要以遍历状态转移表的形式从而花费大量时间的状态机呢？这个时候就需要以面向对象的思想来设计有限状态机。

### 面向对象法设计状态机
上述所提到了两个设计方法都是基于面向过程的一种设计思想，面向过程编程(POP)是一种以过程为中心的编程思想，以正在发生的事件为主要目标，指导开发者利用算法作为基本构建块构建复杂系统。 
即将所要介绍的面向对象编程(OOP)是利用类和对象作为基本构建块，因此分解系统时，可以从算法开始，也可以从对象开始，然后利用所得到的结构作为框架构建系统。 
提到面向对象编程，那自然绕不开面向对象的三个基本特征：

封装：隐藏对象的属性和实现细节，仅仅对外公开接口
继承：使用现有类的所有功能，并在无需重新编写原来的类的情况下对这些功能进行扩展，C 语言使用 struct 的特性实现继承
多态性：使用相同的方法，根据对象的类型调用不同的处理函数。

上述对于面向对象的三个基本特征做了一个简单的介绍，封装和继承的概念都都比较清晰，多态性这个特点可能会有所迷惑，在这里笔者用在书中看到一个例子来解释多态性，
例子是这样的： 
要求画一个形状，这个形状是可能是圆形，矩形，星形，无论是什么图形，其共性都是需要调用一个画的方法来进行绘制，绘制的形状可以通过函数指针调用各自的绘图代码绘制，这就是多态的意义，根据对象的类型调用不同的处理函数。 在介绍了上述很基本的概念之后，我们来看状态机的设计。

#### 实现细节
我们由浅入深地来思考这个问题，首先我们可以想到把闸机当做一个对象，那么这个这个对象的职责就是处理 card 事件(刷卡)和 pass 事件(通过闸机)，闸机会根据当前的状态执行不同的动作，也就有了如下的代码
```
enum {LOCKED,UNLOCKED};/*枚举各个状态*/

/*定义闸机类*/
typedef struct _turnstile
{
    int state;
    void (*card)(struct _turnstile *p_this);
    void (*pass)(struct _turnstile *p_this);
}turnstile_t;

/* 闸机 card 事件 */
void turnstile_card(turnstile_t *p_this)
{
    if (p_this->state == LOCKED)
    {
        /* 切换至解锁状态 */
        /* 执行unlock动作,调用 unlock 函数 */
    }
    else
    {
        /* 执行 thank you 动作,调用 thank you 函数 */
    }
}

/* 闸机 pass 事件*/
void turnstile_pass(turnstile_t *p_this)
{
    if (p_this->state == LOCKED)
    {
        /* 执行 alarm 动作,调用 alarm 函数*/
    }
    else
    {
        /* 状态切换至锁闭状态 */
        /* 执行 lock 动作,调用 lock 函数 */
    }
}
```

上述代码的思想实现的有限状态机相比于前两种不需要进行大量的遍历，也不会导致代码量的冗长，看似已经比较完美了，但是我们再仔细想想，**如果此时状态更改了，那 turnstile_card 函数和 turnstile_pass 函数都要更改，也就是说事件和状态存在着耦合，这与“高内聚，低耦合”的思想所违背**，也就是说如果我们要继续优化代码，那需要对事件和状态进行解耦。

#### 状态和事件解耦
将事件与状态相分离，从而使得各个状态的事件处理函数非常的单一，因此在这里需要定义一个状态类：
```
typedef struct _turnstile_state_t
{
    void (*card)(void);  /* card 事件处理函数 */
    void (*pass)(void);  /* pass 事件处理函数 */
}turnstile_state_t;
```

在定义了状态类之后，我们就可以使用状态类创建 lock 和 unlock 的实例并初始化。
```
turnstile_state_t locked_state = {locked_card,locked_pass};
turnstile_state_t unlocked_state = {unlocked_card,unlocked_pass};
```

在这里需要补充一下上述初始化项里函数里的具体实现。
```
void locked_card(void)
{
    /* 状态切换至解锁状态 */
    /* 执行 unlock 动作 ,调用 unlock 函数 */
}

void locked_pass(void)
{
    /* 执行 alarm 动作，调用 alarm 函数 */
}

void unlocked_card(void)
{
    /* 执行 thank you 动作，调用 thank you 函数 */ 
}

void unlocked_pass(void)
{
    /* 状态切换至锁闭状态 */
    /* 执行 lock 动作，调用 lock 函数 */
}
```

这样，也就实现了状态与事件的解耦，闸机不再需要判断当前的状态，而是直接调用不同状态提供的 card() 和 pass() 方法。定义了状态类之后，由于闸机是整个系统的中心，我们还需要定义闸机类，由于 turnstile_state_t 中只存在方法，并不存在属性，那么我们可以这样来定义闸机类：
```
typedef struct _turnstile_t
{
    turnstile_state_t *p_state;
}turnstile_t;
```

到这里，我们已经定义了闸机类，闸机状态类，以及闸机状态类实例，他们之间的关系如下图所示：

通过图中我们也可以看到闸机类是继承于闸机状态类的，locked_state 和 unlocked_state 实例是由闸机状态类派生而来的，那最底下的那个箭头是为什么呢？这是在后面需要讲到的对于闸机状态转换的处理，在获取输入事件调用具体的方法进行处理后，我们需要修改闸机类的p_state,所以也就有了这个箭头。 相比于最开始定义的闸机类，这个显得更加简洁了，同时 p_state 可以指向相应的状态对象，从而调用相应的事件处理函数。 在定义了一个闸机类之后，就可以通过闸机类定义一个闸机实例:
```
turnstile_t turnstile;
```
然后通过函数进行初始化：
```
void turnstile_init(turnstile_t *p_this)
{
    p_this->p_state = &locked_state;
}
```
整个系统闸机作为中心，进而需要定义闸机类的事件处理方法，定义方法如下：
```
/* 闸机 card 事件*/
void turnstile_card(turnstile_t *p_this)
{
    p_this->p_state->card();
}

/* 闸机 pass 事件 */
void turnstile_pass(turnstile_t *p_this)
{
    p_this->p_state->pass();
}
```
到这里，我们回顾前文所述，我们已经能够对闸机进行初始化并使得闸机根据不同的状态执行不同的处理函数了，再回顾整个闸机的工作流程，我们发现闸机在工作的时候会涉及到从 locked 状态到 unlocked 状态的相互变化，也就是状态的转移,因此状态转移函数可以这样实现：
```
void turnstile_state_set(turnstile_t *p_this,turnstile_state_t *p_new_state)
{
    p_this->p_state = p_new_state;
}
```
而状态的转移是在事件处理之后进行变化的。那么我们可以这样修改处理函数，这里用输出语句替代闸机动作执行函数：
```
void locked_card(turnstile_t *p_turnstile)
{
    turnstile_state_set(p_turnstile,&unlocked_state);
    printf("unlock\n");   /* 执行 unlock 动作 */
}

void locked_pass(turnstile_t *p_turnstile)
{
    printf("alarm\n");   /* 执行 alarm 动作*/
}

void unlocked_card(turnstile_t *p_turnstile)
{
    printf("thankyou\n"); /* 执行 thank you 动作*/
}

void unlocked_pass(turnstile_t *p_turnstile)
{
    turnstile_state_set(p_turnstile,&locked_state);
    printf("lock\n");     /* 执行 lock 动作 */
}
```
既然处理函数都发生了变化，那么闸机状态类也应该发生更改，更改如下：
```
typedef struct _turnstile_state_t
{
    void (*card)(turnstile_t *p_turnstile);
    void (*pass)(turnstile_t *p_turnstile);
}turnstile_state_t;
```
但是回顾之前我们给出的闸机类和闸机状态类的关系，闸机类是继承于闸机状态类的，也就是说先有的闸机状态类后有的闸机类，但是这里却在闸机状态类的方法中使用了闸机类的参数，其实这样也是可行的，需要提前对闸机类进行处理，总的闸机类状态类定义如下：
```
#ifndef __TURNSTILE_H__
#define __TURNSTILE_H__

struct _turnstile_t;
typedef struct _turnstile_t turnstile_t;

typedef struct _turnstile_state_t
{
    void (*card)(turnstile_t *p_turnstile);
    void (*pass)(turnstile_t *p_turnstile);
}turnstile_state_t;

typedef struct _turnstile_t
{
    turnstile_state_t *p_state;
}turnstile_t;

void turnstile_init(turnstile_t *p_this);    /* 闸机初始化 */
void turnstile_card(turnstile_t *p_this);    /* 闸机 card 事件处理 */
void turnstile_pass(turnstile_t *p_this);    /* 闸机 pass 事件处理 */

#endif
```
上述就是所有的关于状态机的相关定义了，下面通过上述的定义实现状态机的实现：
```
#include <stdio.h>
#include <turnstile.h>

int main(void)
{
    int event;
    turnstile_t turnstile;          /* 闸机实例 */
    turnstile_init(&turnstile);     /* 初始化闸机为锁闭状态 */

    while(1)
    {
        scanf("%d",&event);
        switch(event)
        {
        case 0:
            turnstile_card(&turnstile);
            break;
        case 1:
            turnstile_pass(&turnstile);
            break;
        default:
            exit(0);
        }
    }
}
```

#### 较完整OPP FSM
```
struct _turnstile_t;
typedef struct _turnstile_t turnstile_t;

typedef struct _turnstile_state_t
{
    void (*card)(turnstile_t *p_turnstile);
    void (*pass)(turnstile_t *p_turnstile);
}turnstile_state_t;

typedef struct _turnstile_t
{
    turnstile_state_t *p_state;
}turnstile_t;


void locked_card(turnstile_t *p_turnstile)
{
    turnstile_state_set(p_turnstile,&unlocked_state);
    printf("unlock\n");   /* 执行 unlock 动作 */
}

void locked_pass(turnstile_t *p_turnstile)
{
    printf("alarm\n");   /* 执行 alarm 动作*/
}

void unlocked_card(turnstile_t *p_turnstile)
{
    printf("thankyou\n"); /* 执行 thank you 动作*/
}

void unlocked_pass(turnstile_t *p_turnstile)
{
    turnstile_state_set(p_turnstile,&locked_state);
    printf("lock\n");     /* 执行 lock 动作 */
}

turnstile_state_t locked_state = {locked_card,locked_pass};
turnstile_state_t unlocked_state = {unlocked_card,unlocked_pass};


void turnstile_init(turnstile_t *p_this);    /* 闸机初始化 */
void turnstile_card(turnstile_t *p_this);    /* 闸机 card 事件处理 */
void turnstile_pass(turnstile_t *p_this);    /* 闸机 pass 事件处理 */
void turnstile_state_set(turnstile_t *p_this,turnstile_state_t *p_new_state);

void turnstile_init(turnstile_t *p_this)
{
    p_this->p_state = &locked_state;
}

/* 闸机 card 事件*/
void turnstile_card(turnstile_t *p_this)
{
    p_this->p_state->card();
}

/* 闸机 pass 事件 */
void turnstile_pass(turnstile_t *p_this)
{
    p_this->p_state->pass();
}

void turnstile_state_set(turnstile_t *p_this,turnstile_state_t *p_new_state)
{
    p_this->p_state = p_new_state;
}


int main(void)
{
    int event;
    turnstile_t turnstile;          /* 闸机实例 */
    turnstile_init(&turnstile);     /* 初始化闸机为锁闭状态 */

    while(1)
    {
        scanf("%d",&event);
        switch(event)
        {
        case 0:
            turnstile_card(&turnstile);
            break;
        case 1:
            turnstile_pass(&turnstile);
            break;
        default:
            exit(0);
        }
    }
}
```

### FSM template 1
```
#include <stdio.h>

// Define events
typedef enum {
    EVENT_ON_CARD,
    EVENT_ON_PASS
} Event;

// Forward declaration of StateMachine
typedef struct StateMachine StateMachine;

// Define state structure
typedef struct State {
    void (*onCard)(StateMachine* sm);
    void (*onPass)(StateMachine* sm);
} State;

// Define StateMachine structure
struct StateMachine {
    State* currentState;
};


// Define state functions
void lock_onCard(StateMachine* sm) {
    printf("Card accepted, unlocking turnstile\n");
    sm->currentState = &unlockState;
}

void lock_onPass(StateMachine* sm) {
    printf("Invalid event in Lock State\n");
}

void unlock_onCard(StateMachine* sm) {
    printf("Invalid event in Unlock State\n");
}

void unlock_onPass(StateMachine* sm) {
    printf("Passing through turnstile\n");
    sm->currentState = &lockState;
}

// Define state instances
State lockState = {lock_onCard, lock_onPass};
State unlockState = {unlock_onCard, unlock_onPass};


// Initialize the state machine
void StateMachine_init(StateMachine* sm, State* initialState) {
    sm->currentState = initialState;
    sm->currentState->onCard(sm);
}

// Handle events in the state machine
void StateMachine_handleEvent(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_ON_CARD:
            sm->currentState->onCard(sm);
            break;
        case EVENT_ON_PASS:
            sm->currentState->onPass(sm);
            break;
        default:
            printf("Unknown event\n");
    }
}


int main() {
    StateMachine sm;
    StateMachine_init(&sm, &lockState);

    StateMachine_handleEvent(&sm, EVENT_ON_CARD);
    StateMachine_handleEvent(&sm, EVENT_ON_PASS);

    return 0;
}

```

### FSM template 2
```
#include <stdio.h>

// Define events
typedef enum {
    EVENT_ON_CARD,
    EVENT_ON_PASS
} Event;

// Forward declaration of StateMachine
typedef struct StateMachine StateMachine;

// Define state structure
typedef struct State {
    void (*onEnter)(StateMachine* sm);
    void (*onExit)(StateMachine* sm);
    void (*handleEvent)(StateMachine* sm, Event event);
} State;

// Define StateMachine structure
struct StateMachine {
    State* currentState;
};


// Define state functions
void lock_onEnter(StateMachine* sm) {
    printf("Entering Lock State\n");
}

void lock_onExit(StateMachine* sm) {
    printf("Exiting Lock State\n");
}

void lock_handleEvent(StateMachine* sm, Event event) {
    if (event == EVENT_ON_CARD) {
        printf("Card accepted, unlocking turnstile\n");
        sm->currentState = &unlockState;
    } else {
        printf("Invalid event in Lock State\n");
    }
}

void unlock_onEnter(StateMachine* sm) {
    printf("Entering Unlock State\n");
}

void unlock_onExit(StateMachine* sm) {
    printf("Exiting Unlock State\n");
}

void unlock_handleEvent(StateMachine* sm, Event event) {
    if (event == EVENT_ON_PASS) {
        printf("Passing through turnstile\n");
        sm->currentState = &lockState;
    } else {
        printf("Invalid event in Unlock State\n");
    }
}

// Define state instances
State lockState = {lock_onEnter, lock_onExit, lock_handleEvent};
State unlockState = {unlock_onEnter, unlock_onExit, unlock_handleEvent};


// Initialize the state machine
void StateMachine_init(StateMachine* sm, State* initialState) {
    sm->currentState = initialState;
    sm->currentState->onEnter(sm);
}

// Handle events in the state machine
void StateMachine_handleEvent(StateMachine* sm, Event event) {
    sm->currentState->handleEvent(sm, event);
}


int main() {
    StateMachine sm;
    StateMachine_init(&sm, &lockState);

    StateMachine_handleEvent(&sm, EVENT_ON_CARD);
    StateMachine_handleEvent(&sm, EVENT_ON_PASS);

    return 0;
}

```





