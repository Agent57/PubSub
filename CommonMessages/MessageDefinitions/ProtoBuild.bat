.echo off

REM Set up environment

  set PROTOC="..\..\Thirdparty\protobuf\Debug\bin\protoc"
  set SRC_DIR="%1\MessageDefinitions"
  set OUT_DIR=%SRC_DIR%


REM Remove old protoc generated files

  echo Removing old CC files from %OUT_DIR%...
  IF EXIST %OUT_DIR%\*.cc (
    DEL /q %OUT_DIR%\*.cc
  )
  echo Done.

  echo Removing old H files from %OUT_DIR%...
  IF EXIST %OUT_DIR%\*.h (
    DEL /q %OUT_DIR%\*.h
  )
  echo Done.


REM Rebuild protobuf message class files

  echo Rebuilding C++ protobuf message class files from %SRC_DIR%...
  FOR /R %SRC_DIR% %%i IN (*.proto) DO (
    echo Generating message class %%i
    %PROTOC% -I=%SRC_DIR% --cpp_out=%OUT_DIR% %%i
  )
  echo Done.

echo on
