/*
This file is part of TeleFlow, an unofficial Telegram client based on
Telegram Desktop.

TeleFlow-specific modifications:
Copyright (c) 2026 Stanley Lloyd.

For license and copyright information see LEGAL and LICENSE.
*/
#include "teleflow/flow/flow_store.h"

#include "base/unixtime.h"
#include "main/main_session.h"
#include "storage/storage_account.h"

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

#include <cstddef>

namespace TeleFlow {
namespace {

constexpr auto kPrefKey = std::string_view("teleflow.flow.items");
constexpr auto kFormatVersion = 1;
constexpr auto kMaxItems = 10000;

[[nodiscard]] bool ValidType(qint32 value) {
	return value >= qint32(FlowType::ReplyLater)
		&& value <= qint32(FlowType::Later);
}

[[nodiscard]] bool ValidState(qint32 value) {
	return value >= qint32(FlowState::Active)
		&& value <= qint32(FlowState::Cancelled);
}

[[nodiscard]] QByteArray Serialize(const std::vector<FlowItem> &items) {
	auto result = QByteArray();
	auto stream = QDataStream(&result, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_5_1);
	stream << qint32(kFormatVersion) << qint32(items.size());
	for (const auto &item : items) {
		stream << quint64(item.messageId.peer.value)
			<< qint64(item.messageId.msg.bare)
			<< qint32(item.type)
			<< qint32(item.state)
			<< qint64(item.createdAt)
			<< qint64(item.dueAt);
	}
	return result;
}

[[nodiscard]] std::vector<FlowItem> Deserialize(const QByteArray &serialized) {
	if (serialized.isEmpty()) {
		return {};
	}

	auto stream = QDataStream(serialized);
	stream.setVersion(QDataStream::Qt_5_1);

	auto version = qint32();
	auto count = qint32();
	stream >> version >> count;
	if (stream.status() != QDataStream::Ok
		|| version != kFormatVersion
		|| count < 0
		|| count > kMaxItems) {
		return {};
	}

	auto result = std::vector<FlowItem>();
	result.reserve(count);
	for (auto i = 0; i != count; ++i) {
		auto peer = quint64();
		auto message = qint64();
		auto type = qint32();
		auto state = qint32();
		auto createdAt = qint64();
		auto dueAt = qint64();
		stream >> peer >> message >> type >> state >> createdAt >> dueAt;
		const auto messageId = MsgId(message);
		if (stream.status() != QDataStream::Ok
			|| !peer
			|| !IsServerMsgId(messageId)
			|| !ValidType(type)
			|| !ValidState(state)) {
			return {};
		}
		result.push_back(FlowItem{
			.messageId = FullMsgId(
				PeerId(BareId(peer)),
				messageId),
			.type = FlowType(type),
			.state = FlowState(state),
			.createdAt = createdAt,
			.dueAt = dueAt,
		});
	}
	return result;
}

} // namespace

std::vector<FlowItem> ReadItems(Main::Session &session) {
	return Deserialize(
		session.local().readPref<QByteArray>(kPrefKey));
}

AddResult AddItem(
		Main::Session &session,
		FullMsgId messageId,
		FlowType type,
		qint64 dueAt) {
	if (!messageId.peer || !IsServerMsgId(messageId.msg)) {
		return AddResult::InvalidMessage;
	}

	auto items = ReadItems(session);
	for (const auto &item : items) {
		if (item.messageId == messageId
			&& item.type == type
			&& item.state == FlowState::Active) {
			return AddResult::AlreadyExists;
		}
	}
	if (items.size() >= std::size_t(kMaxItems)) {
		return AddResult::StorageLimitReached;
	}

	items.push_back(FlowItem{
		.messageId = messageId,
		.type = type,
		.state = FlowState::Active,
		.createdAt = qint64(base::unixtime::now()),
		.dueAt = dueAt,
	});
	session.local().writePref<QByteArray>(kPrefKey, Serialize(items));
	return AddResult::Added;
}

StateChangeResult ChangeActiveItemState(
		Main::Session &session,
		FullMsgId messageId,
		FlowType type,
		FlowState state) {
	if (!messageId.peer || !IsServerMsgId(messageId.msg)) {
		return StateChangeResult::InvalidMessage;
	} else if (state == FlowState::Active) {
		return StateChangeResult::InvalidState;
	}

	auto items = ReadItems(session);
	for (auto &item : items) {
		if (item.messageId == messageId
			&& item.type == type
			&& item.state == FlowState::Active) {
			item.state = state;
			session.local().writePref<QByteArray>(kPrefKey, Serialize(items));
			return StateChangeResult::Changed;
		}
	}
	return StateChangeResult::NotFound;
}

} // namespace TeleFlow
