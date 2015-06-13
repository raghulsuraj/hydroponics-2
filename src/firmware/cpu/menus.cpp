/* This file is a part of 'hydroponics' project.
 * Copyright (c) 2012-2015, Artyom Lebedev <artyom.lebedev@gmail.com>
 * All rights reserved.
 * See COPYING file for copyright details.
 */

/** @file menus.cpp */

#include "cpu.h"

using namespace adk;


const Menu::Action MainMenu::actions[] = {
    {Application::GetPageTypeCode<MainPage>(), MainPage::Fabric},
    {Application::GetPageTypeCode<Menu>(), ManualControlMenu::Fabric},
    {Application::GetPageTypeCode<Menu>(), CalibrationMenu::Fabric},
    {Application::GetPageTypeCode<Menu>(), SetupMenu::Fabric},
};

void
MainMenu::Fabric(void *p)
{
    new (p) Menu(strings.MainMenu, Menu::returnPos, actions);
    Menu::returnPos = 0;
}


const Menu::Action ManualControlMenu::actions[] = {
    {Application::GetPageTypeCode<Menu>(), MainMenu::Fabric},
    {0, nullptr},
    {0, nullptr},
    {0, nullptr}
};

void
ManualControlMenu::Fabric(void *p)
{
    new (p) Menu(strings.ManualControlMenu, Menu::returnPos, actions, 0);
    Menu::returnPos = Menu::FindAction(MainMenu::actions,
                                       SIZEOF_ARRAY(MainMenu::actions),
                                       ManualControlMenu::Fabric);
}


const Menu::Action CalibrationMenu::actions[] = {
    {Application::GetPageTypeCode<Menu>(), MainMenu::Fabric},
    {0, nullptr},
    {0, nullptr},
    {0, nullptr}
};

void
CalibrationMenu::Fabric(void *p)
{
    new (p) Menu(strings.CalibrationMenu, Menu::returnPos, actions, 0);
    Menu::returnPos = Menu::FindAction(MainMenu::actions,
                                       SIZEOF_ARRAY(MainMenu::actions),
                                       CalibrationMenu::Fabric);
}


const Menu::Action SetupMenu::actions[] = {
    {Application::GetPageTypeCode<Menu>(), MainMenu::Fabric},
    {0, nullptr},
    {0, nullptr},
    {0, nullptr}
};

void
SetupMenu::Fabric(void *p)
{
    new (p) Menu(strings.SetupMenu, Menu::returnPos, actions, 0);
    Menu::returnPos = Menu::FindAction(MainMenu::actions,
                                       SIZEOF_ARRAY(MainMenu::actions),
                                       SetupMenu::Fabric);
}
