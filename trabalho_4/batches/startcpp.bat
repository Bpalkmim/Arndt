REM Inicia as vari�veis ambiente
REM para compilar C++ com Visual Studio
if ""=="%VCENV%" call vcvars32.bat
set VCENV="set"