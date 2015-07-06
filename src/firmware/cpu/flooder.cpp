/* This file is a part of 'hydroponics' project.
 * Copyright (c) 2012-2015, Artyom Lebedev <artyom.lebedev@gmail.com>
 * All rights reserved.
 * See COPYING file for copyright details.
 */

/** @file flooder.cpp */

#include "cpu.h"

using namespace adk;

Flooder flooder;

u8 EEMEM Flooder::eePumpThrottle = 70,
         Flooder::eePumpBoostThrottle = 120;
Time EEMEM Flooder::eeMinSunriseTime {7, 0},
           Flooder::eeFirstFloodDelay{0, 30},
           Flooder::eeFloodDuration{0, 1},
           Flooder::eeFloodPeriod{24, 0},
           Flooder::eeMaxSunsetTime{22, 0};

Flooder::Flooder()
{
    status = Status::IDLE;
}

Time
Flooder::GetMinSunriseTime()
{
    Time t;
    eeprom_read_block(&t, &eeMinSunriseTime, sizeof(t));
    return t;
}

void
Flooder::SetMinSunriseTime(Time t)
{
    eeprom_update_block(&t, &eeMinSunriseTime, sizeof(t));
}

Time
Flooder::GetFirstFloodDelay()
{
    Time t;
    eeprom_read_block(&t, &eeFirstFloodDelay, sizeof(t));
    return t;
}

void
Flooder::SetFirstFloodDelay(Time t)
{
    eeprom_update_block(&t, &eeFirstFloodDelay, sizeof(t));
}

Time
Flooder::GetFloodDuration()
{
    Time t;
    eeprom_read_block(&t, &eeFloodDuration, sizeof(t));
    return t;
}

void
Flooder::SetFloodDuration(Time t)
{
    eeprom_update_block(&t, &eeFloodDuration, sizeof(t));
}

Time
Flooder::GetFloodPeriod()
{
    Time t;
    eeprom_read_block(&t, &eeFloodPeriod, sizeof(t));
    return t;
}

void
Flooder::SetFloodPeriod(Time t)
{
    eeprom_update_block(&t, &eeFloodPeriod, sizeof(t));
}

Time
Flooder::GetMaxSunsetTime()
{
    Time t;
    eeprom_read_block(&t, &eeMaxSunsetTime, sizeof(t));
    return t;
}

void
Flooder::SetMaxSunsetTime(Time t)
{
    eeprom_update_block(&t, &eeMaxSunsetTime, sizeof(t));
}

const char *
Flooder::GetStatusString()
{
    switch (status) {
    case Status::IDLE:
        return strings.FlooderStatus_Idle;
    case Status::FLOODING:
        return strings.FlooderStatus_Flooding;
    case Status::FLOOD_FINAL:
        return strings.FlooderStatus_FloodFinal;
    case Status::DRAINING:
        return strings.FlooderStatus_Draining;
    case Status::FAILURE:
        return strings.FlooderStatus_Failure;
    }
    return strings.NoValue;
}

const char *
Flooder::GetErrorString()
{
    switch (errorCode) {
    case ErrorCode::LOW_WATER:
        return strings.FlooderError_LowWater;
    }
    return strings.NoValue;
}

void
Flooder::StartFlooding()
{
    AtomicSection as;
    if (status != Status::IDLE) {
        return;
    }
    lastWaterLevel = lvlGauge.GetValue();
    if (lastWaterLevel < static_cast<u16>(MIN_START_WATER) * 255 / 100) {
        status = Status::FAILURE;
        errorCode = ErrorCode::LOW_WATER;
        return;
    }
    status = Status::FLOODING;
    startLevel = lastWaterLevel;
    pump.SetLevel(eeprom_read_byte(&eePumpThrottle));
    scheduler.ScheduleTask(_FloodPoll, POLL_PERIOD);
}

u8
Flooder::GetPumpThrottle()
{
    return eeprom_read_byte(&eePumpThrottle);
}

void
Flooder::SetPumpThrottle(u8 value)
{
    eeprom_update_byte(&eePumpThrottle, value);
}

u8
Flooder::GetPumpBoostThrottle()
{
    return eeprom_read_byte(&eePumpBoostThrottle);
}

void
Flooder::SetPumpBoostThrottle(u8 value)
{
    eeprom_update_byte(&eePumpBoostThrottle, value);
}

u16
Flooder::FloodPoll()
{
    u8 newLevel = lvlGauge.GetValue();
    if (status == Status::FLOODING) {
        if (newLevel < 0x80 && newLevel >= lastWaterLevel) {
            siphonLevel = lastWaterLevel;
            status = Status::FLOOD_FINAL;
            pump.SetLevel(eeprom_read_byte(&eePumpBoostThrottle));
        }
    } else if (status == Status::FLOOD_FINAL) {
        if (newLevel > siphonLevel + 0x18) {
            status = Status::DRAINING;
            pump.SetLevel(0);
        }
    } else if (status == Status::DRAINING) {
        if (newLevel > startLevel - 0x20) {
            status = Status::IDLE;
            lastWaterLevel = newLevel;
            return 0;
        }
    } else {
        return 0;
    }
    lastWaterLevel = newLevel;
    return POLL_PERIOD;
}

u16
Flooder::_FloodPoll()
{
    return flooder.FloodPoll();
}

u8
Flooder::GetTopPotWaterLevel()
{
    if (startLevel == 0) {
        return 0;
    }
    if (siphonLevel == 0) {
        return 0xff - lastWaterLevel;
    }
    return 0xff -
        static_cast<u16>(lastWaterLevel - siphonLevel) * 0xff /
        (startLevel - siphonLevel);
}
