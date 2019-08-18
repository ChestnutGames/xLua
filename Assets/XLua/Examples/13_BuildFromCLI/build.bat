@echo off
echo =======================================================
echo ˵������ʾ��ʾ���������е���Unity�Զ���������
echo       C#Դ��ο�BuildFromCLI.cs
echo.
echo ע��1���޸ı��ļ������·����
echo        UNITY_PATH��unity.exe������·��
echo        PROJECT_PATH�����̸�Ŀ¼������·��
echo        LOG_PATH��������־����·��
echo ע��2��ִ�����ȹر�Unity
echo =======================================================

set UNITY_PATH="D:\Program Files (x86)\Unity 2017.4.3f1\Editor\Unity.exe"
set PROJECT_PATH="D:\work\xLua_forsakenyang"
set LOG_PATH="D:\work\xLua_forsakenyang\output\log.txt"

echo start...

rem ȷ����־Ŀ¼����
for %%a in (%LOG_PATH%) do (
    set log_root=%%~dpa
)    
if not exist %log_root% mkdir %log_root%

%UNITY_PATH% -batchmode -quit -projectPath %PROJECT_PATH% -logFile %LOG_PATH% -executeMethod XLuaTest.BuildFromCLI.Build

echo done.
pause