/*
This file is part of TeleFlow, an unofficial Telegram client based on
Telegram Desktop.

TeleFlow-specific modifications:
Copyright (c) 2026 Stanley Lloyd.

For license and copyright information see LEGAL and LICENSE.
*/
#pragma once

#include "data/data_msg_id.h"

#include <vector>

namespace Main {
class Session;
} // namespace Main

namespace TeleFlow {

enum class FlowType : uchar {
	ReplyLater = 0,
	Task = 1,
	Waiting = 2,
	Later = 3,
};

enum class FlowState : uchar {
	Active = 0,
	Done = 1,
	Cancelled = 2,
};

struct FlowItem {
	FullMsgId messageId;
	FlowType type = FlowType::Task;
	FlowState state = FlowState::Active;
	qint64 createdAt = 0;
	qint64 dueAt = 0;

	friend inline bool operator==(const FlowItem &, const FlowItem &) = default;
};

enum class AddResult : uchar {
	Added,
	AlreadyExists,
	InvalidMessage,
	StorageLimitReached,
};

// Flow data is stored per Telegram account in the existing local account
// preferences. No Telegram server state is changed by these operations.
[[nodiscard]] std::vector<FlowItem> ReadItems(Main::Session &session);
[[nodiscard]] AddResult AddItem(
	Main::Session &session,
	FullMsgId messageId,
	FlowType type,
	qint64 dueAt = 0);

} // namespace TeleFlow
