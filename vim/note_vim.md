1.1 
jumping around

    CTRL-]: 超链接跳转;
    CTRL-o: jump back,o stands for older;
    


2 edit file

2.1 first time
2.2 inserting text
2.3 moving around
2.4 deleting characters
2.5 undo and redo
2.6 other editing commands
2.7 getting out
2.8 finding help 

2.1 for the first time

    start vim: gvim file.txt
    
2.2 inserting text

    :set showmode
    i: insert mode

2.3 moving around

    h: left
    l: right
    j: roll up
    k: roll down

2.4 deleting characters

    x: delete the character under the cursor
    dd: to delete a line

2.5 undo and redo

    u: undo
    U: undo line
    CTRL-r: redo

2.6 other editing commands
    i: 在光标前面插入
    a: append,在光标后增加
    o: open a new,empty line below the cursor
    O: above the cursor
    count:3x = xxx

2.7 getting out

    ZZ: write and exit
    :q!: quit and throw things away
    :e!: reload the original version of the file

2.8 finding help

    :help: get the generic help
    :help {subject}
	:hepl x
	:help index
	:help CTRL-a


3

3.1 word movement
3.2 moving to the start or end of a line
3.3 moving to a character
3.4 matching a parenthesis
3.5 moving to a specific line
3.6 telling where you are
3.7 scrolling around
3.8 simple search
3.9 simple search pattern
3.10 using mark

3.1 word movement
    w: moves to the start if the next word
    b: moves backward to the start of the previous word
    e: moves to the end of a word
    ge: moves to the previous end of a word

3.2 moving to the start or end of a line 
    $: moves to the end of a line
    ^: moves to the start of a line
    0: moves to the very first character of the line

3.3 moving to a character
    fx: searches forward in the line for the single character x
    Fx: searches backward
    tx: forward,stops one character before the searched character
    Tx: backward
     
3.4 matching parenthesis
    %: moves to the matching paren

3.5 moving to a specific line
    nG: go to line n
    G: positions you to at the end of the file
    gg: go to start of the file
    90%:
    H: home
    M: middle
    L: last

3.6 telling where you are
    CTRL-g: show line number information
    :set number
    :set nonumber
    :set ruler

3.7 scrolling around
    CTRL-u: scrolls down half a screen
    CTRL-d: scrolls up half a screen
    CTRL-e: scrolls up one line
    CTRL_y: scrolls down one line
    CTRL_F: scrolls up s screen
    CTRL-B: scrools down a screen
    zz: puts the cursor at the middle of the screen
    zt: puts the cursor line at the top
    zb: bottom

3.8 simple searches
    /xx: find the string
        n: for next searches
    ?xx: searches backward
    /N: backward
    ?N: forward
    :set ignorecase
    :set noignorecase 
    Position the cursor on the word and use the "*"command.
    \>:matches at the end
    \<:matches at the beginning
    /\<tht\>: only find the
    












