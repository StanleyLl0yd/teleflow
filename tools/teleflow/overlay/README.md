# TeleFlow

**TeleFlow** is an unofficial Telegram desktop client for Windows built around a smart inbox.

> **Message → Action → Done.**

TeleFlow is based on the official [Telegram Desktop](https://github.com/telegramdesktop/tdesktop) source code and keeps normal Telegram messaging behavior while adding a local productivity layer for messages that require attention.

## Status

TeleFlow is currently in early development. Version **0.1.0 Foundation** establishes an independent Windows application identity and prepares the codebase for the Flow feature set.

## Planned core features

- **Flow Inbox** — one place for messages that require action.
- **Reply Later** — defer a reply without losing the original context.
- **Tasks** — turn a Telegram message into a lightweight local task.
- **Waiting for Reply** — track outgoing messages that still need a response.
- **Reminders** — return to a specific message at a chosen time.
- **Done** — close the loop without altering Telegram read state.
- **Snooze** — temporarily hide a conversation from the TeleFlow view.
- **Unified Flow** — one action inbox across multiple Telegram accounts.

All Flow metadata is intended to stay local to the TeleFlow client unless a future optional synchronization mechanism is explicitly introduced.

## Version 0.1.0 — Foundation

The Foundation milestone separates TeleFlow from Telegram Desktop on Windows:

- application name: `TeleFlow`;
- executable: `TeleFlow.exe`;
- application data: `%APPDATA%\\TeleFlow`;
- independent installer AppId;
- independent Windows AppUserModelID;
- independent notification COM activator CLSID;
- independent shortcuts and autostart target;
- upstream auto-update disabled until TeleFlow has its own release channel;
- upstream crash reporting disabled until TeleFlow has its own crash-reporting endpoint;
- TeleFlow version separated from the underlying Telegram Desktop version.

The initial Foundation work is pinned to Telegram Desktop upstream commit:

`8dc0f52ee5cb17c68ee7b3762edce9a36fcf9955`

Telegram Desktop upstream version at that commit: **7.1.2**.

## Build target

Initial supported target:

- Windows 10 22H2 or newer;
- Windows 11;
- x64.

Other platforms are not a goal of the first TeleFlow release.

## Telegram API credentials

A distributable third-party Telegram client must use its own Telegram API credentials. Do **not** ship Telegram Desktop test credentials.

Obtain `api_id` and `api_hash` from Telegram and provide them at configure time. Never commit private credentials to this repository.

See `docs/TELEFLOW_BUILDING_WIN.md`.

## Relationship to Telegram

TeleFlow is an **unofficial** Telegram client. It is not affiliated with, endorsed by, or published by Telegram FZ-LLC.

Telegram and Telegram Desktop are trademarks/products of their respective owners.

## License

Telegram Desktop is licensed under the **GNU General Public License v3 or later with the OpenSSL linking exception**. TeleFlow is a derivative work and is distributed under the same applicable terms.

See `LICENSE` and `LEGAL`.

## Author

**Stanley Lloyd**

GitHub: https://github.com/StanleyLl0yd/teleflow
