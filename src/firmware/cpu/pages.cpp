/* This file is a part of 'hydroponics' project.
 * Copyright (c) 2012-2015, Artyom Lebedev <artyom.lebedev@gmail.com>
 * All rights reserved.
 * See COPYING file for copyright details.
 */

/** @file pages.cpp */

#include "cpu.h"

using namespace adk;

namespace ManCtrl_Light {

void
OnClosed(u16)
{
    app.SetNextPage(Application::GetPageTypeCode<Menu>(),
                    ManualControlMenu::Fabric);
}

void
OnChanged(u16 value)
{
    light.SetLevel(value);
}

void
Fabric(void *p)
{
    TPage *sel = new (p) TPage(strings.LightControl, light.GetLevel(), 0, 255);
    Menu::returnPos = Menu::FindAction(ManualControlMenu::actions, Fabric);
    sel->onClosed = OnClosed;
    sel->onChanged = OnChanged;
}

} /* namespace ManCtrl_Light */


namespace ManCtrl_Pump {

void
OnClosed(u16)
{
    app.SetNextPage(Application::GetPageTypeCode<Menu>(),
                    ManualControlMenu::Fabric);
}

void
OnChanged(u16 value)
{
    pump.SetLevel(value);
}

void
Fabric(void *p)
{
    TPage *sel = new (p) TPage(strings.PumpControl, pump.GetLevel(), 0, 255);
    Menu::returnPos = Menu::FindAction(ManualControlMenu::actions, Fabric);
    sel->onClosed = OnClosed;
    sel->onChanged = OnChanged;
}

} /* namespace ManCtrl_Pump */


namespace Status_LvlGauge {

void
OnClosed(u16)
{
    app.SetNextPage(Application::GetPageTypeCode<Menu>(),
                    StatusMenu::Fabric);
}

void
Poll()
{
    static_cast<TPage *>(app.CurPage())->SetValue(lvlGauge.GetValue());
}

void
Fabric(void *p)
{
    TPage *sel = new (p) TPage(strings.LvlGaugeStatus, lvlGauge.GetValue(),
                               0, 0xff, true);
    Menu::returnPos = Menu::FindAction(StatusMenu::actions, Fabric);
    sel->onClosed = OnClosed;
    sel->poll = Poll;
}

} /* namespace Status_LvlGauge */


namespace ClbLvlGauge_MinValue {

void
Poll()
{
    static_cast<TPage *>(app.CurPage())->SetHint(lvlGauge.GetRawValue());
}

void
OnClosed(u16)
{
    lvlGauge.SetMinValue(static_cast<TPage *>(app.CurPage())->GetValue());
    app.SetNextPage(Application::GetPageTypeCode<Menu>(),
                    LvlGaugeCalibrationMenu::Fabric);
}

void
Fabric(void *p)
{
    TPage *sel = new (p) TPage(strings.LvlGaugeClbMin, lvlGauge.GetMinValue(),
                               0, 0xffff);
    Menu::returnPos = Menu::FindAction(LvlGaugeCalibrationMenu::actions, Fabric);
    sel->onClosed = OnClosed;
    sel->poll = Poll;
}

} /* namespace ClbLvlGauge_MinValue */


namespace ClbLvlGauge_MaxValue {

void
OnClosed(u16)
{
    lvlGauge.SetMaxValue(static_cast<TPage *>(app.CurPage())->GetValue());
    app.SetNextPage(Application::GetPageTypeCode<Menu>(),
                    LvlGaugeCalibrationMenu::Fabric);
}

void
Fabric(void *p)
{
    TPage *sel = new (p) TPage(strings.LvlGaugeClbMax, lvlGauge.GetMaxValue(),
                               0, 0xffff);
    Menu::returnPos = Menu::FindAction(LvlGaugeCalibrationMenu::actions, Fabric);
    sel->onClosed = OnClosed;
    sel->poll = ClbLvlGauge_MinValue::Poll;
}

} /* namespace ClbLvlGauge_MaxValue */