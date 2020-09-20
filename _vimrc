"设置编码"
set fileencodings=utf-8,ucs-bom,gb18030,gbk,gb2312,cp936
set termencoding=utf-8
set encoding=utf-8


colorscheme industry
set guifont=Courier:h11:cANSI

"显示行号"
set nu
set number

"突出显示当前行"
set cursorline
set cc=0

"启用鼠标"
set mouse=a
set mouse=v
set selection=exclusive
set selectmode=mouse,key

"显示括号匹配"
set showmatch

"设置Tab长度为4空格"
set tabstop=4
"设置自动缩进长度为4空格"
set shiftwidth=4
"继承前一行的缩进方式，适用于多行注释"
set autoindent

set paste
set listchars=tab:>-,trail:-

"总是显示状态栏"
set laststatus=2
"显示光标当前位置"
set ruler


"-- Cscope setting --
if has("cscope")
set csprg=/usr/bin/cscope " 指定用来执行cscope的命令
set csto=0 " 设置cstag命令查找次序：0先找cscope数据库再找标签文件；1先找标签文件再找cscope数据库
set cst " 同时搜索cscope数据库和标签文件
set cscopequickfix=s-,c-,d-,i-,t-,e- " 使用QuickFix窗口来显示cscope查找结果
set nocsverb
if filereadable("cscope.out") " 若当前目录下存在cscope数据库，添加该数据库到vim
cs add cscope.out
elseif $CSCOPE_DB != "" " 否则只要环境变量CSCOPE_DB不为空，则添加其指定的数据库到vim
cs add $CSCOPE_DB
endif
set csverb
endif
map <F4> :cs add ./cscope.out .<CR><CR><CR> :cs reset<CR>
imap <F4> <ESC>:cs add ./cscope.out .<CR><CR><CR> :cs reset<CR>


"filetype on　　　　　　　　        "打开文件类型检测功能
let Tlist_Ctags_Cmd = "/usr/bin/ctags"         "设定系统中ctags程序的位置
let Tlist_Show_One_File = 1                    "不同时显示多个文件的tag，只显示当前文件的
let Tlist_Exit_OnlyWindow = 1                  "如果taglist窗口是最后一个窗口，则退出vim
let Tlist_Use_Right_Window = 1
map <silent> <F7> :TlistToggle<cr>             "按F7等同于在命令行模式输入:TlistToggle

nmap <C-H> <C-W>h 
nmap <C-J> <C-W>j 
nmap <C-K> <C-W>k 
nmap <C-L> <C-W>l 
 
" // The switch of the Source Explorer 
nmap <F8> :SrcExplToggle<CR>  

" // Set the height of Source Explorer window 
let g:SrcExpl_winHeight = 8 

" // Set 100 ms for refreshing the Source Explorer 
let g:SrcExpl_refreshTime = 100 

" // Set "Enter" key to jump into the exact definition context 
let g:SrcExpl_jumpKey = "<ENTER>" 

" // Set "Space" key for back from the definition context 
let g:SrcExpl_gobackKey = "<SPACE>" 

nmap <F9> :NERDTreeToggle<CR> 
let NERDTreeWinPos = "left"
