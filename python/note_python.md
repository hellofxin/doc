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