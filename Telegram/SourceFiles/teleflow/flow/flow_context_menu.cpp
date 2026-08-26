/*
This file is part of TeleFlow, an unofficial Telegram client based on
Telegram Desktop.

TeleFlow-specific modifications:
Copyright (c) 2026 Stanley Lloyd.

For license and copyright information see LEGAL and LICENSE.
*/
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

	if (!result->empty()) {
		result->addSeparator();
	}

	const auto navigation = request.navigation;
	result->addAction(u"Add to Flow as Task"_q, [navigation, itemId] {
		const auto added = TeleFlow::AddItem(
			navigation->session(),
			itemId,
			TeleFlow::FlowType::Task);
		switch (added) {
		case TeleFlow::AddResult::Added:
			navigation->showToast(u"Added to Flow as Task"_q);
			break;
		case TeleFlow::AddResult::AlreadyExists:
			navigation->showToast(
				u"This message is already an active Flow task"_q);
			break;
		case TeleFlow::AddResult::InvalidMessage:
			navigation->showToast(u"This message can't be added to Flow"_q);
			break;
		case TeleFlow::AddResult::StorageLimitReached:
			navigation->showToast(u"Flow storage limit reached"_q);
			break;
		}
	});
	return result;
}

} // namespace HistoryView
