# Clean files and start from scratch
&git reset --hard
&git clean -d -x -f

Set-Location "build\VC12"

$solutionFilePath = "MinHookVC12.sln"

# Change the build config

$projectFilePaths = "libMinHook.vcxproj"

# Build
&msbuild $solutionFilePath /t:Rebuild /p:Configuration=Debug
&msbuild $solutionFilePath /t:Rebuild /p:Configuration=Release
