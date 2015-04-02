# Clean files and start from scratch
&git reset --hard
&git clean -d -x -f

Set-Location "build\VC12"

$solutionFilePath = "MinHookVC12.sln"

# Change the build config

$projectFilePaths = "libMinHook.vcxproj"
$projectFilePaths | %{
    $projectFilePath = $_
    $projectContent = Get-Content -Path $projectFilePath
    $projectContent = $projectContent -replace '(?<=<RuntimeLibrary>)MultiThreadedDebug(?=</RuntimeLibrary>)', "MultiThreadedDebugDLL"
    #$projectContent = $projectContent -replace '(?<=<RuntimeLibrary>)MultiThreaded(?=</RuntimeLibrary>)', "MultiThreadedDLL"
    $projectContent | Set-Content -Path $projectFilePath
}

# Build
&msbuild $solutionFilePath /t:Rebuild /p:Configuration=Debug
&msbuild $solutionFilePath /t:Rebuild /p:Configuration=Release
