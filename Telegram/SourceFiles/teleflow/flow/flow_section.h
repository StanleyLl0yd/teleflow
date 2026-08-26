/*
This file is part of TeleFlow, an unofficial Telegram client based on
Telegram Desktop.

TeleFlow-specific modifications:
Copyright (c) 2026 Stanley Lloyd.

For license and copyright information see LEGAL and LICENSE.
*/
#pragma once

#include <memory>

namespace Window {
class SectionMemento;
} // namespace Window

namespace TeleFlow {

[[nodiscard]] std::shared_ptr<Window::SectionMemento> MakeFlowSection();

} // namespace TeleFlow
