#include <DxLib.h>
#include "../../Common/Vector.h"
#include "SkinSave.h"

void SkinSave::Save(
    IntVector3 skin[64][64],
    const char* fileName)
{
    int screen = MakeScreen(64, 64, TRUE);

    SetDrawScreen(screen);

    ClearDrawScreen();

    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            auto color = skin[x][y];

            DrawPixel(
                x,
                y,
                GetColor(
                    color.r,
                    color.g,
                    color.b));
        }
    }

    SaveDrawScreenToPNG(
        0,
        0,
        64,
        64,
        fileName);

    SetDrawScreen(DX_SCREEN_BACK);

    DeleteGraph(screen);
}