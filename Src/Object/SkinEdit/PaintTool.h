#pragma once

class PaintTool
{
public:

    enum TOOL
    {
        PEN,
        BUCKET
    };

public:

    PaintTool();

    void Toggle();

    bool IsPen() const;
    bool IsBucket() const;

private:

    TOOL currentTool_;
};