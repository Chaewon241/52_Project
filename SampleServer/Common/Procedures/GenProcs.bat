pushd %~dp0

GenProcs.exe --path=../../SampleServer/GameDB.xml --output=GenProcedures.h

IF ERRORLEVEL 1 PAUSE

XCOPY /Y GenProcedures.h "../../SampleServer"

DEL /Q /F *.h

PAUSE