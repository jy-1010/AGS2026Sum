#pragma once

#include <DxLib.h>
#include "HSVRing.h"

HSVRing::HSVRing()
{
    hue_ = 0.0f;

    centerX_ = 0;
    centerY_ = 0;

    ringScreen_ = -1;
}

void HSVRing::Update()
{
}

void HSVRing::Draw()
{
    DrawCircle(
        centerX_,
        centerY_,
        OUTER_RADIUS,
        GetColor(255, 255, 255),
        FALSE);

    DrawCircle(
        centerX_,
        centerY_,
        INNER_RADIUS,
        GetColor(255, 255, 255),
        FALSE);
}

void HSVRing::SetPosition(
    int centerX,
    int centerY)
{
    centerX_ = centerX;
    centerY_ = centerY;
}

float HSVRing::GetHue() const
{
    return hue_;
}