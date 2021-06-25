// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <OptionWindow.hpp>

namespace gui
{
    class AppsAndToolsWindow : public OptionWindow
    {
      public:
        AppsAndToolsWindow(app::Application *app);
        ~AppsAndToolsWindow() override = default;

      private:
        std::list<Option> appsAndToolsOptionsList();
    };
} // namespace gui