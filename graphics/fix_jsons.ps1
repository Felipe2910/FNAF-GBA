Add-Type -AssemblyName System.Drawing

$base = Split-Path -Parent $MyInvocation.MyCommand.Path
$input = $base
$output = Join-Path $base "output_frames"

New-Item -ItemType Directory -Force -Path $output | Out-Null

Get-ChildItem -Path $input -Filter "trailer_*.bmp" | ForEach-Object {

    $inPath = $_.FullName
    $outBmp = Join-Path $output $_.Name
    $outJson = [System.IO.Path]::ChangeExtension($outBmp, ".json")

    try {
        $img = [System.Drawing.Image]::FromFile($inPath)

        # Crear lienzo 256x256
        $canvas = New-Object System.Drawing.Bitmap 256,256
        $g = [System.Drawing.Graphics]::FromImage($canvas)
        $g.Clear([System.Drawing.Color]::Black)

        $x = [int]((256 - $img.Width) / 2)
        $y = [int]((256 - $img.Height) / 2)

        $g.DrawImage($img, $x, $y, $img.Width, $img.Height)

        # ⚠️ Convertir a 8bpp (truco: clonar como Indexed)
        $bmp8 = $canvas.Clone(
            [System.Drawing.Rectangle]::FromLTRB(0,0,256,256),
            [System.Drawing.Imaging.PixelFormat]::Format8bppIndexed
        )

        $bmp8.Save($outBmp, [System.Drawing.Imaging.ImageFormat]::Bmp)

        # Crear JSON (UTF-8 sin BOM)
        $json = @"
{
    "type": "regular_bg",
    "bpp_mode": "bpp_8",
    "compression": "lz77"
}
"@

        [System.IO.File]::WriteAllText($outJson, $json, (New-Object System.Text.UTF8Encoding $false))

        $g.Dispose()
        $canvas.Dispose()
        $bmp8.Dispose()
        $img.Dispose()

        Write-Host "OK: $($_.Name)"
    }
    catch {
        Write-Host "ERROR: $($_.Name)"
    }
}

Write-Host "Listo para Butano 🚀"