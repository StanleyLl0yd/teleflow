/*
This file is part of TeleFlow, an unofficial Telegram client based on
Telegram Desktop.

TeleFlow-specific modifications:
Copyright (c) 2026 Stanley Lloyd.

For license and copyright information see LEGAL and LICENSE.
*/
#include "teleflow/flow/flow_section.h"

#include "teleflow/flow/flow_store.h"

#include "data/data_peer.h"
#include "data/data_session.h"
#include "history/history_item.h"
#include "main/main_session.h"
#include "ui/painter.h"
#include "ui/widgets/popup_menu.h"
#include "ui/widgets/scroll_area.h"
#include "window/section_memento.h"
#include "window/section_widget.h"
#include "window/window_session_controller.h"
#include "styles/style_window.h"

#include <QtGui/QCursor>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace TeleFlow {
namespace {

constexpr auto kHeaderHeight = 56;
constexpr auto kHeaderBackWidth = 56;
constexpr auto kRowHeight = 82;
constexpr auto kSidePadding = 20;
constexpr auto kEmptyHeight = 180;

[[nodiscard]] QString TypeTitle(FlowType type) {
	switch (type) {
	case FlowType::ReplyLater:
		return u"Reply later"_q;
	case FlowType::Task:
		return u"Task"_q;
	case FlowType::Waiting:
		return u"Waiting"_q;
	case FlowType::Later:
		return u"Later"_q;
	}
	return u"Flow"_q;
}

struct FlowRow {
	FlowItem item;
	QString title;
	QString preview;
};

[[nodiscard]] std::vector<FlowRow> BuildRows(Main::Session &session) {
	auto result = std::vector<FlowRow>();
	for (const auto &item : ReadItems(session)) {
		if (item.state != FlowState::Active) {
			continue;
		}

		const auto peer = session.data().peerLoaded(item.messageId.peer);
		const auto peerName = peer
			? peer->name()
			: u"Chat %1"_q.arg(qulonglong(item.messageId.peer.value));
		const auto message = session.data().message(
			item.messageId.peer,
			item.messageId.msg);
		auto preview = message
			? message->originalText().text.simplified()
			: QString();
		if (preview.isEmpty()) {
			preview = message
				? u"Message without text"_q
				: u"Message #%1"_q.arg(qint64(item.messageId.msg.bare));
		}

		result.push_back(FlowRow{
			.item = item,
			.title = TypeTitle(item.type) + u"  ·  "_q + peerName,
			.preview = std::move(preview),
		});
	}
	return result;
}

class FlowListWidget final : public Ui::RpWidget {
public:
	FlowListWidget(
		QWidget *parent,
		not_null<Window::SessionController*> controller,
		std::function<void(int)> activeCountChanged)
	: RpWidget(parent)
	, _controller(controller)
	, _activeCountChanged(std::move(activeCountChanged))
	, _rows(BuildRows(controller->session())) {
		setAttribute(Qt::WA_OpaquePaintEvent);
		updateCursor();
	}

	[[nodiscard]] int contentHeight() const {
		return _rows.empty()
			? kEmptyHeight
			: int(_rows.size()) * kRowHeight;
	}

	[[nodiscard]] int activeCount() const {
		return int(_rows.size());
	}

protected:
	void paintEvent(QPaintEvent *e) override {
		auto p = Painter(this);
		p.fillRect(e->rect(), st::windowBg);

		if (_rows.empty()) {
			const auto textWidth = std::max(0, width() - 2 * kSidePadding);
			p.setFont(st::semiboldFont);
			p.setPen(st::windowFg);
			p.drawText(
				QRect(kSidePadding, 48, textWidth, 28),
				Qt::AlignHCenter | Qt::AlignVCenter,
				u"Nothing in Flow yet"_q);
			p.setFont(st::normalFont);
			p.setPen(st::windowSubTextFg);
			p.drawText(
				QRect(kSidePadding, 82, textWidth, 48),
				Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap,
				u"Right-click a message and choose Add to Flow → Task."_q);
			return;
		}

		const auto first = std::max(0, e->rect().top() / kRowHeight);
		const auto last = std::min(
			int(_rows.size()),
			(e->rect().bottom() / kRowHeight) + 1);
		const auto textWidth = std::max(0, width() - 2 * kSidePadding);

		for (auto i = first; i != last; ++i) {
			const auto top = i * kRowHeight;
			const auto &row = _rows[i];

			p.setFont(st::semiboldFont);
			p.setPen(st::windowFg);
			const auto title = p.fontMetrics().elidedText(
				row.title,
				Qt::ElideRight,
				textWidth);
			p.drawText(
				QRect(kSidePadding, top + 14, textWidth, 22),
				Qt::AlignLeft | Qt::AlignVCenter,
				title);

			p.setFont(st::normalFont);
			p.setPen(st::windowSubTextFg);
			const auto preview = p.fontMetrics().elidedText(
				row.preview,
				Qt::ElideRight,
				textWidth);
			p.drawText(
				QRect(kSidePadding, top + 43, textWidth, 22),
				Qt::AlignLeft | Qt::AlignVCenter,
				preview);
		}
	}

	void mouseReleaseEvent(QMouseEvent *e) override {
		if (_rows.empty()) {
			return;
		}
		const auto index = e->pos().y() / kRowHeight;
		if (index < 0 || index >= int(_rows.size())) {
			return;
		}
		const auto item = _rows[index].item;
		if (e->button() == Qt::LeftButton) {
			openMessage(item.messageId);
		} else if (e->button() == Qt::RightButton) {
			showContextMenu(item);
		}
	}

private:
	void openMessage(FullMsgId id) {
		_controller->showPeerHistory(
			id.peer,
			Window::SectionShow(Window::SectionShow::Way::Forward),
			id.msg);
	}

	void showContextMenu(FlowItem item) {
		_contextMenu = base::make_unique_q<Ui::PopupMenu>(this);
		_contextMenu->addAction(u"Open message"_q, [=] {
			openMessage(item.messageId);
		});
		_contextMenu->addAction(u"Mark as done"_q, [=] {
			const auto changed = ChangeActiveItemState(
				_controller->session(),
				item.messageId,
				item.type,
				FlowState::Done);
			switch (changed) {
			case StateChangeResult::Changed:
				_controller->showToast(u"Flow item completed"_q);
				reloadRows();
				break;
			case StateChangeResult::NotFound:
				_controller->showToast(u"Flow item is no longer active"_q);
				reloadRows();
				break;
			case StateChangeResult::InvalidMessage:
				_controller->showToast(u"Flow item has an invalid message"_q);
				break;
			case StateChangeResult::InvalidState:
				_controller->showToast(u"Flow item state can't be changed"_q);
				break;
			}
		});
		_contextMenu->popup(QCursor::pos());
	}

	void reloadRows() {
		_rows = BuildRows(_controller->session());
		resize(width(), contentHeight());
		updateCursor();
		if (_activeCountChanged) {
			_activeCountChanged(activeCount());
		}
		update();
	}

	void updateCursor() {
		if (_rows.empty()) {
			unsetCursor();
		} else {
			setCursor(Qt::PointingHandCursor);
		}
	}

	const not_null<Window::SessionController*> _controller;
	const std::function<void(int)> _activeCountChanged;
	std::vector<FlowRow> _rows;
	base::unique_qptr<Ui::PopupMenu> _contextMenu;
};

class FlowWidget;

class FlowMemento final : public Window::SectionMemento {
public:
	object_ptr<Window::SectionWidget> createWidget(
		QWidget *parent,
		not_null<Window::SessionController*> controller,
		Window::Column,
		const QRect &geometry) override;
};

class FlowWidget final : public Window::SectionWidget {
public:
	FlowWidget(
		QWidget *parent,
		not_null<Window::SessionController*> controller)
	: SectionWidget(parent, controller)
	, _scroll(this, st::defaultSolidScroll) {
		setAttribute(Qt::WA_OpaquePaintEvent);
		auto inner = object_ptr<FlowListWidget>(
			_scroll.data(),
			controller,
			[this](int count) {
				_activeCount = count;
				update(QRect(0, 0, width(), kHeaderHeight));
			});
		_inner = inner.data();
		_activeCount = _inner->activeCount();
		_scroll->setOwnedWidget(std::move(inner));
		_scroll->show();
	}

	bool showInternal(
			not_null<Window::SectionMemento*> memento,
			const Window::SectionShow &) override {
		return (dynamic_cast<FlowMemento*>(memento.get()) != nullptr);
	}

	bool sameTypeAs(not_null<Window::SectionMemento*> memento) override {
		return (dynamic_cast<FlowMemento*>(memento.get()) != nullptr);
	}

	std::shared_ptr<Window::SectionMemento> createMemento() override {
		return std::make_shared<FlowMemento>();
	}

protected:
	void resizeEvent(QResizeEvent *) override {
		const auto bodyHeight = std::max(0, height() - kHeaderHeight);
		_scroll->setGeometry(0, kHeaderHeight, width(), bodyHeight);
		if (_inner) {
			_inner->resize(_scroll->width(), _inner->contentHeight());
		}
	}

	void paintEvent(QPaintEvent *e) override {
		if (animatingShow()) {
			Window::SectionWidget::paintEvent(e);
			return;
		}

		auto p = Painter(this);
		p.fillRect(e->rect(), st::windowBg);
		if (e->rect().top() >= kHeaderHeight) {
			return;
		}

		p.setFont(st::semiboldFont);
		p.setPen(st::windowFg);
		p.drawText(
			QRect(0, 0, kHeaderBackWidth, kHeaderHeight),
			Qt::AlignCenter,
			u"‹"_q);
		p.drawText(
			QRect(
				kHeaderBackWidth,
				7,
				std::max(0, width() - kHeaderBackWidth - 16),
				24),
			Qt::AlignLeft | Qt::AlignVCenter,
			u"Flow"_q);

		p.setFont(st::normalFont);
		p.setPen(st::windowSubTextFg);
		p.drawText(
			QRect(
				kHeaderBackWidth,
				30,
				std::max(0, width() - kHeaderBackWidth - 16),
				18),
			Qt::AlignLeft | Qt::AlignVCenter,
			u"%1 active"_q.arg(_activeCount));
	}

	void mouseReleaseEvent(QMouseEvent *e) override {
		if (e->button() == Qt::LeftButton
			&& e->pos().x() < kHeaderBackWidth
			&& e->pos().y() < kHeaderHeight) {
			controller()->showBackFromStack();
		}
	}

private:
	object_ptr<Ui::ScrollArea> _scroll;
	FlowListWidget *_inner = nullptr;
	int _activeCount = 0;
};

object_ptr<Window::SectionWidget> FlowMemento::createWidget(
		QWidget *parent,
		not_null<Window::SessionController*> controller,
		Window::Column,
		const QRect &geometry) {
	auto result = object_ptr<FlowWidget>(parent, controller);
	result->setGeometry(geometry);
	return result;
}

} // namespace

std::shared_ptr<Window::SectionMemento> MakeFlowSection() {
	return std::make_shared<FlowMemento>();
}

} // namespace TeleFlow
