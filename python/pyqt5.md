## pycharm config

designer:
	program
		E:\script\venvProject\venv\Lib\site-packages\qt5_applications\Qt\bin\designer.exe
	argument
		none
	workingdir
		$ProjectFileDir$

pyuic:
	program
		E:\script\venvProject\venv\Scripts\pyuic5.exe
	argument
		$FileName$ -o $FileNameWithoutExtension$.py
	workingdir
		$ProjectFileDir$
		
pyuic:
	program
		E:\script\venvProject\venv\Scripts\python.exe
	argument
		-m PyQt5.uic.pyuic $FileName$ -o $FileNameWithoutExtension$.py
	workingdir
		$ProjectFileDir$

