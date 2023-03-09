// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <hal/battery_charger/AbstractBatteryCharger.hpp>
#include <Service/Service.hpp>
#include <Timers/TimerHandle.hpp>

#include <memory>

namespace sys
{
    class Service;
} // namespace sys

class BatteryBrownoutDetector
{
  public:
    struct Thresholds
    {
        units::Voltage shutdown;
        std::uint8_t measurementMaxCount;
    };

    using BrownoutCallback = std::function<void()>;

    BatteryBrownoutDetector(sys::Service *service,
                            hal::battery::AbstractBatteryCharger &charger,
                            Thresholds voltage,
                            BrownoutCallback messageCallback);
    void startDetection();
    void check();

  private:
    hal::battery::AbstractBatteryCharger &charger;

    bool eventDetectionOngoing        = false;
    unsigned measurementBrownoutCount = 0;
    Thresholds voltage;
    BrownoutCallback sendMessage;
    sys::TimerHandle measurementTick;
};
