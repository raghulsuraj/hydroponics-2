/* This file is a part of 'hydroponics' project.
 * Copyright (c) 2015, Artyom Lebedev <artyom.lebedev@gmail.com>
 * All rights reserved.
 * See LICENSE file for copyright details.
 */

/** @file flooder.h */

#ifndef FLOODER_H_
#define FLOODER_H_

class Flooder {
public:
    enum Status {
        IDLE,
        FLOODING,
        FLOOD_WAIT,
        FLOOD_FINAL,
        DRAINING,
        FAILURE
    };

    enum ErrorCode {
        /** Too low water level for flooding start. */
        LOW_WATER
    };

    Flooder();

    void
    Initialize();

    Status
    GetStatus()
    {
        return static_cast<Status>(status);
    }

    const char *
    GetStatusString();

    ErrorCode
    GetErrorCode()
    {
        return static_cast<ErrorCode>(errorCode);
    }

    const char *
    GetErrorString();

    void
    StartFlooding();

    static u8
    GetPumpThrottle();

    static void
    SetPumpThrottle(u8 value);

    static u8
    GetPumpBoostThrottle();

    static void
    SetPumpBoostThrottle(u8 value);

    /** Get heuristically calculated water level in the top pot. 255 is 100%. */
    u8
    GetTopPotWaterLevel();

    Time
    GetMinSunriseTime();

    void
    SetMinSunriseTime(Time t);

    Time
    GetFirstFloodDelay();

    void
    SetFirstFloodDelay(Time t);

    Time
    GetFloodDuration();

    void
    SetFloodDuration(Time t);

    Time
    GetFloodPeriod();

    void
    SetFloodPeriod(Time t);

    Time
    GetMaxSunsetTime();

    void
    SetMaxSunsetTime(Time t);

    Time
    GetLastSunriseTime()
    {
        return lastSunriseTime;
    }

    Time
    GetLastSunsetTime()
    {
        return lastSunsetTime;
    }

    /** Is currently daylight time according to ambient light sensors. */
    bool
    IsAmbientDaylight()
    {
        return isAmbientDaylight;
    }

    /** Is currently daylight time (possibly extended by lighting). */
    bool
    IsDaylight()
    {
        return isDaylight;
    }

    /** Get next flooding time. Return 00:00 if no more flooding until next
     * sunrise.
     */
    Time
    GetNextFloodTime();

private:
    enum {
        /** Minimal water level to start flooding, in percents. */
        MIN_START_WATER = 95,
        /** Control polling period. */
        POLL_PERIOD = TASK_DELAY_S(2),
        /** Polling period for flooding schedule. */
        SCHEDULE_POLL_PERIOD = TASK_DELAY_S(60)
    };
    u8 status:3,
       errorCode:3,
       isDaylight:1,
       floodWaitDone:1,

       siphonReached:1,
       isAmbientDaylight:1,
       sunsetSeen:1,
       dayOfWeek:3,
       /** Extended delay when changing pump mode. */
       extendedPollDelay:1,
       :1;
    /** Water level when cycle started. */
    u8 startLevel = 0;
    /** Water level on most recent gauge reading. */
    u8 lastWaterLevel;
    /** Water level when siphon reached. */
    u8 siphonLevel = 0;
    /** Last volume counted for top pot flooding. */
    u8 lastTopVolume = 0;
    /** Minimal level seen during various flooding stages. */
    u8 minLevel;
    u8 minLevelUpdated = 0, minLevelStayed = 0;
    /** Maximal level seen during various flooding stages. */
    u8 maxLevel;
    u8 maxLevelUpdated = 0, maxLevelStayed = 0;

    Time lastSunriseTime{0, 0}, lastSunsetTime{0, 0}, lastFloodTime {0, 0},
         floodDelayTime;

    /** Throttle value to use when pump is on. */
    static u8 EEMEM eePumpThrottle,
    /** Throttle value to use when pump is boosted on final run. */
                    eePumpBoostThrottle;
    static Time EEMEM eeMinSunriseTime,
                      eeFirstFloodDelay,
                      eeFloodDuration,
                      eeFloodPeriod,
                      eeMaxSunsetTime;

    static u16
    _FloodPoll();

    u16
    FloodPoll();

    static u16
    _SchedulePoll();

    u16
    SchedulePoll();

} __PACKED;

extern Flooder flooder;

#endif /* FLOODER_H_ */
