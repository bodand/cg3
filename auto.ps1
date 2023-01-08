$src = @()
$src += Get-ChildItem .\src\ -Recurse -Filter "main.cxx" # -Exclude "*.in.*", "data\*"
#$src += Get-ChildItem .\checks\ -Recurse -Filter "main.cxx" # -Exclude "*.in.*", "data\*"

foreach ($src in $src)
{
    $ran = @(Get-Job | ? { $_.State -eq 'Running' })
    if ($ran.Count -ge 8)
    {
        $ran | Wait-Job -Any | Out-Null
    }

    Write-Host -NoNewline "Starting analysis for: " | Out-Default
    Write-Host -ForegroundColor Green "$src" | Out-Default
    Start-Job {
        Get-Location
        Write-Host "clang-tidy -p `"D:\.projects\cg3-clone\_build-release-x64-msvc\`" $using:src"
        clang-tidy -p "D:\.projects\cg3-clone\_build-release-x64-msvc\" $using:src
    } | Out-Null
}

Wait-Job *

foreach ($job in Get-Job)
{
    $result = Receive-Job $job
    Write-Host $result
    Write-Host ""
}

Remove-Job -State Completed
