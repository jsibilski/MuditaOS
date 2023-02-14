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
    if (detectionOngoing) {
        return;
    }
    LOG_DEBUG("Battery brownout detection window start");
    detectionOngoing = true;
    measurementCount = 0;
    check();
}

bool BatteryBrownoutDetector::isBrownout()
{
    if (charger.getBatteryVoltage() < voltage.shutdown) {
        LOG_DEBUG("Battery brownout detected");
        sendMessage();
        return true;
    }
    return false;
}

void BatteryBrownoutDetector::check()
{
    if (isBrownout()) {
        return;
    }

    measurementCount++;
    if (measurementCount <= voltage.measurementMaxCount) {
        measurementTick.start();
    }
    else {
        LOG_DEBUG("Battery brownout detection window finish with negative result");
        detectionOngoing = false;
    }
}
