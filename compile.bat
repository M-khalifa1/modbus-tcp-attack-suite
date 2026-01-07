@echo off
REM ============================================================================
REM Modbus/TCP Attack Suite - Compilation Script
REM Authors: Mahmoud A. Khalifa, Ahmad Taher Azar, Walid El-Shafai
REM Institution: Prince Sultan University, Saudi Arabia
REM ============================================================================

echo.
echo ============================================================
echo   Modbus/TCP Attack Suite - Compilation Script
echo   Automated Systems and Computing Lab (ASCL)
echo ============================================================
echo.

REM Check if GCC exists
where gcc >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] GCC compiler not found!
    echo.
    echo Please install MinGW-w64:
    echo   1. Download MSYS2 from: https://www.msys2.org/
    echo   2. Run: pacman -S mingw-w64-x86_64-gcc
    echo   3. Add C:\msys64\mingw64\bin to your PATH
    echo.
    goto :END
)

echo [*] GCC compiler found!
echo [*] Compiling Modbus_attack.c...
echo.

gcc Modbus_attack.c -o modbus_attack.exe -lws2_32

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ============================================================
    echo   [SUCCESS] Compilation complete!
    echo ============================================================
    echo.
    echo   Output file: modbus_attack.exe
    echo.
    echo   Usage:
    echo     modbus_attack.exe [target_ip]
    echo.
    echo   Example:
    echo     modbus_attack.exe 192.168.1.100
    echo.
    echo ============================================================
) else (
    echo.
    echo [ERROR] Compilation failed! Check error messages above.
)

:END
echo.
pause
