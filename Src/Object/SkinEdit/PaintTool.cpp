#include "PaintTool.h"

PaintTool::PaintTool()
{
    currentTool_ = PEN;
}

void PaintTool::Toggle()
{
    if (currentTool_ == PEN)
    {
        currentTool_ = BUCKET;
    }
    else
    {
        currentTool_ = PEN;
    }
}

bool PaintTool::IsPen() const
{
    return currentTool_ == PEN;
}

bool PaintTool::IsBucket() const
{
    return currentTool_ == BUCKET;
}