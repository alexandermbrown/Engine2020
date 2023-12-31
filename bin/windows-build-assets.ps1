param (
	[switch]$copy = $false
)

Push-Location "${PSScriptRoot}\..\AssetBase"

try {
	Get-Command "..\out\build\x64-Release\AssetBase\AssetBase.exe" -ErrorAction Stop | Out-Null
}
catch {
	Write-Host "AssetBase.exe not found. Please build AssetBase in Release with Visual Studio." -ForegroundColor Red
	Write-Host "Script failed. Press any key to finish...";
	$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown");
	EXIT
}

..\out\build\x64-Release\AssetBase\AssetBase.exe

function Copy-Assets {
	Copy-Item ".\output\*" -Destination "..\Article\data\" -Recurse -Force
	Write-Host "Copied asset bundles."
}

if ($copy) {
	Copy-Assets
}
else {
	$confirmation = Read-Host "Copy asset bundles to Article/data? (y/N)"
	$confirmation = $confirmation.ToLower()
	if ($confirmation -eq "y" -Or $confirmation -eq "yes") {
		Copy-Assets
	}
}

Pop-Location
