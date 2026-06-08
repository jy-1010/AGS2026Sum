#include "../../Common/Vector.h"
#include "SkinCanvas.h"

SkinCanvas::SkinCanvas()
{
    Clear({ 255, 255, 255 });
}

void SkinCanvas::Clear(IntVector3 color)
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            pixels_[x][y] = color;
        }
    }
}

void SkinCanvas::SetPixel(
    int x,
    int y,
    IntVector3 color)
{
    if (x < 0 || x >= SIZE ||
        y < 0 || y >= SIZE)
    {
        return;
    }

    pixels_[x][y] = color;
}

IntVector3 SkinCanvas::GetPixel(
    int x,
    int y) const
{
    if (x < 0 || x >= SIZE ||
        y < 0 || y >= SIZE)
    {
        return { 0, 0, 0 };
    }

    return pixels_[x][y];
}

IntVector3(&SkinCanvas::GetData())[SIZE][SIZE]
{
    return pixels_;
}