/* This file is a part of 'hydroponics' project.
 * Copyright (c) 2015, Artyom Lebedev <artyom.lebedev@gmail.com>
 * All rights reserved.
 * See LICENSE file for copyright details.
 */

/** @file pages.h */

#ifndef PAGES_H_
#define PAGES_H_

namespace ManCtrl_Light {
    using TPage = LinearValueSelector;

    void
    Fabric(void *p);
};

namespace ManCtrl_Pump {
    using TPage = LinearValueSelector;

    void
    Fabric(void *p);
};

namespace Status_LvlGauge {
    using TPage = LinearValueSelector;

    void
    Fabric(void *p);
}

namespace Status_LightSensor {
    using TPage = LinearValueSelector;

    void
    FabricA(void *p);

    void
    FabricB(void *p);
}

namespace ClbLvlGauge_MinValue {
    using TPage = LinearValueSelector;

    void
    Fabric(void *p);
}

namespace ClbLvlGauge_MaxValue {
    using TPage = LinearValueSelector;

    void
    Fabric(void *p);
}

namespace SetupFlooding_PumpThrottle {
    using TPage = LinearValueSelector;

    void
    Fabric(void *p);
};

namespace SetupFlooding_PumpBoostThrottle {
    using TPage = LinearValueSelector;

    void
    Fabric(void *p);
};

namespace SetupFlooding_MinSunriseTime {
    using TPage = TimeSelector;

    void
    Fabric(void *p);
};

namespace SetupFlooding_FirstFloodDelay {
    using TPage = TimeSelector;

    void
    Fabric(void *p);
};

namespace SetupFlooding_FloodDuration {
    using TPage = TimeSelector;

    void
    Fabric(void *p);
};

namespace SetupFlooding_FloodPeriod {
    using TPage = TimeSelector;

    void
    Fabric(void *p);
};

namespace SetupFlooding_MaxSunsetTime {
    using TPage = TimeSelector;

    void
    Fabric(void *p);
};

namespace SetupTime {
    using TPage = TimeSelector;

    void
    Fabric(void *p);
};

#endif /* PAGES_H_ */
