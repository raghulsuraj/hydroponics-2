/* This file is a part of 'hydroponics' project.
 * Copyright (c) 2012-2015, Artyom Lebedev <artyom.lebedev@gmail.com>
 * All rights reserved.
 * See COPYING file for copyright details.
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

#endif /* PAGES_H_ */