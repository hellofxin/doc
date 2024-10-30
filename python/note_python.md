## 升级pip
python -m pip install --upgrade pip

## 安装升级指定本版pip
python -m pip install --upgrade pip==23.3.3

## 查看已安装的包
python -m pip list

## 查看可升级的包
python -m pip list --outdated

## 批量安装
python -m pip install pip-review
pip-review --interactive

## 虚拟环境
### 创建虚拟环境
python -m venv myenv

### 激活虚拟环境
workspace$ source venv/Scripts/activate

### 验证虚拟环境
which python

```
fangx@CV119074 MINGW64 /e/script/venvProject/venv/Scripts
$ . activate
(venv)
fangx@CV119074 MINGW64 /e/script/venvProject/venv/Scripts
$ which python
/e/script/venvProject/venv/Scripts/python
(venv)
fangx@CV119074 MINGW64 /e/script/venvProject/venv/Scripts
$ deactivate

fangx@CV119074 MINGW64 /e/script/venvProject/venv/Scripts
$ which python
/d/Python39/python

fangx@CV119074 MINGW64 /e/script/venvProject/venv/Scripts

```

### 在IDE中配置虚拟环境
* VS Code：
1 打开VS Code，按Ctrl+Shift+P打开命令面板。
2 输入“Python: Select Interpreter”并选择。
3 在列表中选择你创建的虚拟环境中的Python解释器。

### 删除虚拟环境
如果某个虚拟环境不再需要，可以简单地删除其所在的文件夹。例如，删除myenv文件夹即可删除虚拟环境。
