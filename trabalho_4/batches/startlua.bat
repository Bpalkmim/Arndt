REM Inicia as variáveis ambiente
REM para compilar e executar Lua
if ""=="%LUA%" goto inicia
goto sai
:inicia
set LUA="set"
set path=%path%;"C:\Program Files (x86)\Lua\5.1"
:sai