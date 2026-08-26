/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/const_string.h"

#define TDESKTOP_REQUESTED_ALPHA_VERSION (0ULL)

#ifdef TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION TDESKTOP_REQUESTED_ALPHA_VERSION
#else // TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION (0ULL)
#endif // TDESKTOP_ALLOW_CLOSED_ALPHA

// used in Updater.cpp and Setup.iss for Windows
constexpr auto AppId = "{1934CBAF-F72A-43EC-B0CD-94ECEED6FA86}"_cs;
constexpr auto AppNameOld = "TeleFlow"_cs;
constexpr auto AppName = "TeleFlow"_cs;
constexpr auto AppFile = "TeleFlow"_cs;
constexpr auto AppVersion = 7001002;
constexpr auto AppVersionStr = "7.1.2";
constexpr auto AppBetaVersion = false;
constexpr auto AppAlphaVersion = TDESKTOP_ALPHA_VERSION;

// TeleFlow has its own product version while retaining the upstream
// Telegram Desktop version for protocol/build compatibility.
constexpr auto TeleFlowVersion = 100;
constexpr auto TeleFlowVersionStr = "0.1.0";
