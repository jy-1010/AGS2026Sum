#pragma once

class SkinCanvas;

class SkinRenderer
{
public:

    static constexpr int PIXEL_SIZE = 8;
    static constexpr int GRID_COLOR = 0xC8C8C8;

    void Draw(
        const SkinCanvas& canvas,
        int offsetX,
        int offsetY);

    static void DrawGrid(
        int offsetX,
        int offsetY,
        int pixelSize,
        int gridColor);

};

