#include <DxLib.h>
#include "../../Common/Vector.h"
#include "SkinCanvas.h"
#include "SkinRenderer.h"

void SkinRenderer::Draw(
    const SkinCanvas& canvas,
    int offsetX,
    int offsetY)
{
    for (int y = 0; y < SkinCanvas::SIZE; y++)
    {
        for (int x = 0; x < SkinCanvas::SIZE; x++)
        {
            IntVector3 color =
                canvas.GetPixel(x, y);

            DrawBox(
                offsetX + x * PIXEL_SIZE,
                offsetY + y * PIXEL_SIZE,
                offsetX + (x + 1) * PIXEL_SIZE,
                offsetY + (y + 1) * PIXEL_SIZE,
                GetColor(
                    color.r,
                    color.g,
                    color.b),
                TRUE);
        }
    }
}

void SkinRenderer::DrawGrid(
    int offsetX,
    int offsetY,
    int pixelSize,
    int gridColor)
{
    for (int i = 0; i <= SkinCanvas::SIZE; i++)
    {
        DrawLine(
            offsetX + i * pixelSize,
            offsetY,
            offsetX + i * pixelSize,
            offsetY + SkinCanvas::SIZE * pixelSize,
            gridColor
        );

        DrawLine(
            offsetX,
            offsetY + i * pixelSize,
            offsetX + SkinCanvas::SIZE * pixelSize,
            offsetY + i * pixelSize,
            gridColor
        );
    }
}