// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "BatteryBrownoutDetector.hpp"

#include <system/Constants.hpp>
#include <Timers/TimerFactory.hpp>
#include <log/log.hpp>

namespace
{
    constexpr std::chrono::milliseconds measurementTickTime{1000};
    constexpr auto measurementTickName = "BrownoutDetectorTick";
} // namespace

BatteryBrownoutDetector::BatteryBrownoutDetector(sys::Service *service,
                                                 hal::battery::AbstractBatteryCharger &charger,
                                                 Thresholds voltage,
                                                 BrownoutCallback messageCallback)
    : charger{charger}, voltage{voltage}, sendMessage{messageCallback},
      measurementTick{sys::TimerFactory::createSingleShotTimer(
          service, measurementTickName, measurementTickTime, [this](sys::Timer &) { check(); })}
{}

void BatteryBrownoutDetector::startDetection()
{
    if (eventDetectionOngoing) {
        return;
    }
    LOG_INFO("Battery brownout detection window start");
    eventDetectionOngoing    = true;
    measurementBrownoutCount = 0;
    check();
}

void BatteryBrownoutDetector::check()
{
    if (charger.getBatteryVoltage() < voltage.shutdown) {
        measurementBrownoutCount++;
        if (measurementBrownoutCount < voltage.measurementMaxCount) {
            measurementTick.start();
        }
        else {
            sendMessage();
            eventDetectionOngoing = false;
            LOG_INFO("Battery brownout detected");
        }
    }
    else {
        if (measurementBrownoutCount > 0) {
            LOG_INFO("Battery brownout detection window finish with negative result");
        }
        // If the voltage level is correct reset counter and event flag
        measurementBrownoutCount = 0;
        eventDetectionOngoing    = false;
    }
}
