@echo off
set "folder=%~1"

if "%folder%"=="" (
    echo Drag a folder onto this bat file.
    pause
    exit /b
)

cd /d "%folder%"

powershell -NoProfile -Command "Get-ChildItem -Filter *.jpg | Sort-Object { [int]($_.BaseName -replace '\D','') } | ForEach-Object { 'file ''' + $_.Name + '''' }" > list.txt

ffmpeg -f concat -safe 0 -r 5 -i list.txt -c:v libx264 -pix_fmt yuv420p output.mp4

del list.txt

pause