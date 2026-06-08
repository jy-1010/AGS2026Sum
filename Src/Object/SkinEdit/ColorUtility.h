#pragma once

class ColorUtility
{
public:

    //定数
    // RGBの最大値（0～255）
    static constexpr int COLOR_MAX = 255;

    // RGB計算用のfloat版最大値
    static constexpr float COLOR_MAX_F = 255.0f;

    // HSVの1セクションの角度（360° ÷ 6）
    static constexpr float HUE_SECTION = 60.0f;

    // 色相(Hue)の最大値
    static constexpr float HSV_MAX_HUE = 360.0f;

    // RGB→HSV変換時の色相補正値
    // Red領域 : 0～60°
    static constexpr float HUE_RED_OFFSET = 6.0f;

    // Green領域 : 120～180°
    static constexpr float HUE_GREEN_OFFSET = 2.0f;

    // Blue領域 : 240～300°
    static constexpr float HUE_BLUE_OFFSET = 4.0f;

    static IntVector3 HSVtoRGB(float h, float s, float v);
    static void RGBtoHSV(IntVector3 color, float& h, float& s, float& v);
};
