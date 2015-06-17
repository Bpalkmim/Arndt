REM Inicia as variáveis ambiente
REM para compilar C++ com Visual Studio
if ""=="%VCENV%" call vcvars32.bat
set VCENV="set"