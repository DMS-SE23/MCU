:: cmd /C $PROJ_DIR$\Release-Utility\release_batch.bat
@echo off
:: ============================================================================
:: mnver	=> Main Image Version
:: blimg	=> Bootloader Image Name
:: blver	=> Bootloader Image Version
:: mergout	=> Name of merge of Main image and Bootloader image
::
:: blph		=> Bootloader image built path
:: rlsph	=> Released archive path
:: packerph	=> fw-packer.exe path
:: combph	=> binmerge.exe path
:: %1		=> $PROJ_DIR$
:: %2		=> $PROJ_FNAME$
:: %3		=> $TARGET_DIR$
:: ============================================================================
set mnver=V0.093
set blver=V0.002
set blimg=%2-BL
set mergout=%2-%mnver%-BL-%blver%.bin

set blph=%1\Release\Exe
set rlsph=%1\Release-Utility\released
set packerph=%1\Release-Utility\fw-packer.exe
set combph=%1\Release-Utility\Combine\binmerge.exe
:: ----------------------------------------------------------------------------

cd %1
@echo on
@echo ~
@echo *****************************************************   Copying to "released"   *********************************************
@echo ***   [ %2 ]   :   [ %2-%mnver%.bin ]
@echo ***   [ %blimg% ]   :   [ %blimg%-%blver%.bin ]
@echo *****************************************************************************************************************************
copy /y %3\%2.bin %rlsph%\%2-%mnver%.bin
copy /y %blph%\%blimg%.bin %rlsph%\%blimg%-%blver%.bin
@echo ~

@echo *****************************************************   Building   **********************************************************
@echo ****************************************************************   EBF   ****************************************************
@echo ***   [ %2.bin ]   :   [ %2-%mnver%.EBF ]
@echo *****************************************************************************************************************************
:: %packerph% %rlsph%\%2-%mnver%.bin %rlsph%\%2-%mnver%-USB.EBF -F0 -T2 -Cu
:: %packerph% %rlsph%\%2-%mnver%.bin %rlsph%\%2-%mnver%-RS232.EBF -F0 -T2 -Cr -B115200
:: %packerph% %rlsph%\%2-%mnver%.bin %rlsph%\%2-%mnver%-I2C.EBF -F0 -T2 -Ci
%packerph% %rlsph%\%2-%mnver%.bin %rlsph%\%2-%mnver%.EBF -F0 -T2 -Ci
@echo ~

@echo *****************************************************   Building   **********************************************************
@echo ****************************************************************   Main + Bootloader   **************************************
@echo ***   [ %2 ]   +   [ %blimg% ]   =   [ %mergout% ]
@echo *****************************************************************************************************************************
%combph% %rlsph%\%blimg%-%blver%.bin %rlsph%\%2-%mnver%.bin %rlsph%\%mergout% 2800 3D800 3FFDE A

@echo off
:exit_rls_bat
set mnver=
set blimg=
set blver=
set mergout=

set ldph=
set rlsph=
:: ============================================================================
:: $COMPILER_ARGS$	(X)
:: $CONFIG_NAME$	(V) : Release
::
:: $CUR_DIR$		(X)
:: $CUR_LINE$		(X)
:: $DATE$		    (V) : 2016/1/30
:: $EW_DIR$		    (V) : C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.2
::
:: $EXE_DIR$		(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000\Build\Main-Release\Exe
:: $FILE_BNAME$		(X)
:: $FILE_BPATH$		(X)
:: $FILE_DIR$		(X)
:: $FILE_FNAME$		(X)
:: $FILE_PATH$		(X)
::
:: $LIST_DIR$		(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000\Build\Main-Release\List
:: $OBJ_DIR$		(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000\Build\Main-Release\Obj
:: $PROJ_DIR$		(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000
:: $PROJ_FNAME$		(V) : IPS-1x-MCU
:: $PROJ_PATH$		(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000\IPS-1x-MCU.ewp
::
:: $TARGET_DIR$		(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000\Build\Main-Release\Exe
:: $TARGET_BNAME$	(V) : IPS-1x-MCU
:: $TARGET_BPATH$	(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000\Build\Main-Release\Exe\IPS-1x-MCU
:: $TARGET_FNAME$	(V) : IPS-1x-MCU.out
:: $TARGET_PATH$	(V) : E:\CYH\Advantech\ADV_Prj\IPS-1x\source\000.000\Build\Main-Release\Exe\IPS-1x-MCU.out
:: $TOOLKIT_DIR$	(V) : C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.2\arm
:: $USER_NAME$		(V) : Phoenix.Chang
::
:: $_ENVVAR_$		(X)
:: $MY_CUSTOM_VAR$	(X)
:: ****************************************************************************