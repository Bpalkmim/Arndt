REM Inicia as vari�veis ambiente
REM para compilar e executar Lua
if ""=="%LUA%" goto inicia
goto sai
:inicia
set LUA="set"
set path=%path%;"c:\Program files (x86)\lua"
:sai