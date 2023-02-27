// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "BaseMessage.hpp"

#include <module-gui/gui/SwitchData.hpp>

namespace app::manager
{
    class SwitchBackRequest : public BaseMessage
    {
        std::unique_ptr<gui::SwitchData> data;

      public:
        SwitchBackRequest(const ApplicationName &name,
                          std::unique_ptr<gui::SwitchData> data              = nullptr,
                          bool dontSwitchBackWhenRequestedAppNameDoesntMatch = false);

        [[nodiscard]] auto getData() noexcept -> std::unique_ptr<gui::SwitchData> &;
        bool dontSwitchBackWhenRequestedAppNameDoesntMatch;
    };

    struct GetPreviousApplicationName : BaseMessage
    {
        GetPreviousApplicationName(ApplicationName const &sender, ApplicationName const &appNameToCheck)
            : BaseMessage(MessageType::APMCheckAppIsPrevious, sender), appNameToCheck_{appNameToCheck}
        {}

        ApplicationName appNameToCheck_;
    };

    struct GetPreviousApplicationNameResponse : sys::ResponseMessage
    {
        GetPreviousApplicationNameResponse(bool response) : result_{response}
        {}

        bool result_;
    };
} // namespace app::manager
