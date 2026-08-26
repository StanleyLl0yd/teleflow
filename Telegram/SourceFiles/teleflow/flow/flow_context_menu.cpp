/*
This file is part of TeleFlow, an unofficial Telegram client based on
Telegram Desktop.

TeleFlow-specific modifications:
Copyright (c) 2026 Stanley Lloyd.

For license and copyright information see LEGAL and LICENSE.
*/
#include "teleflow/flow/flow_section.h"
#include "teleflow/flow/flow_store.h"

#include "history/history_item.h"
#include "history/view/history_view_context_menu.h"
#include "ui/widgets/popup_menu.h"
#include "window/window_session_controller.h"

namespace HistoryView {

base::unique_qptr<Ui::PopupMenu> TeleFlowFillContextMenu(
		not_null<ListWidget*> list,
		const ContextMenuRequest &request) {
	auto result = FillContextMenu(list, request);
	const auto item = request.item;
	if (!item
		|| !item->isRegular()
		|| item->isService()
		|| !request.selectedItems.empty()) {
		return result;
	}

	const auto itemId = item->fullId();
	if (!IsServerMsgId(itemId.msg)) {
		return result;
	}

	const auto navigation = request.navigation;
	const auto addFlow = [navigation, itemId](
			TeleFlow::FlowType type,
			QString label) {
		return [navigation, itemId, type, label = std::move(label)] {
			const auto added = TeleFlow::AddItem(
				navigation->session(),
				itemId,
				type);
			switch (added) {
			case TeleFlow::AddResult::Added:
				navigation->showToast(
					u"Added to Flow as %1"_q.arg(label));
				break;
			case TeleFlow::AddResult::AlreadyExists:
				navigation->showToast(
					u"This message is already active in Flow as %1"_q.arg(label));
				break;
			case TeleFlow::AddResult::InvalidMessage:
				navigation->showToast(u"This message can't be added to Flow"_q);
				break;
			case TeleFlow::AddResult::StorageLimitReached:
				navigation->showToast(u"Flow storage limit reached"_q);
				break;
			}
		};
	};

	auto flowMenu = std::make_unique<Ui::PopupMenu>(result.get(), result->st());
	flowMenu->addAction(
		u"Reply later"_q,
		addFlow(TeleFlow::FlowType::ReplyLater, u"Reply later"_q));
	flowMenu->addAction(
		u"Task"_q,
		addFlow(TeleFlow::FlowType::Task, u"Task"_q));
	if (item->out()) {
		flowMenu->addAction(
			u"Waiting"_q,
			addFlow(TeleFlow::FlowType::Waiting, u"Waiting"_q));
	}
	flowMenu->addAction(
		u"Later"_q,
		addFlow(TeleFlow::FlowType::Later, u"Later"_q));
	flowMenu->addSeparator();
	flowMenu->addAction(u"Open Flow"_q, [navigation] {
		navigation->showSection(TeleFlow::MakeFlowSection());
	});

	if (!result->empty()) {
		result->addSeparator();
	}
	result->addAction(u"Add to Flow"_q, std::move(flowMenu));
	return result;
}

} // namespace HistoryView
