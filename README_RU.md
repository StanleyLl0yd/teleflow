# TeleFlow

**TeleFlow** — неофициальный клиент Telegram для Windows, построенный вокруг концепции умного Inbox.

> **Message → Action → Done.**

TeleFlow основан на исходном коде официального [Telegram Desktop](https://github.com/telegramdesktop/tdesktop). Обычная логика работы Telegram сохраняется, а поверх неё добавляется локальный слой для сообщений, которые требуют внимания или действия.

## Статус

Проект находится на ранней стадии разработки. Версия **0.1.0 Foundation** создаёт независимую идентичность Windows-приложения и подготавливает кодовую базу к реализации Flow.

## Планируемые основные функции

- **Flow Inbox** — единое место для сообщений, требующих действия.
- **Reply Later** — отложить ответ, не теряя исходный контекст.
- **Tasks** — превратить сообщение Telegram в простую локальную задачу.
- **Waiting for Reply** — отслеживать собственные сообщения, на которые ещё ждём ответа.
- **Reminders** — вернуться к конкретному сообщению в заданное время.
- **Done** — закрыть действие, не меняя Telegram read state.
- **Snooze** — временно скрыть диалог только в представлении TeleFlow.
- **Unified Flow** — единый Inbox действий для нескольких аккаунтов Telegram.

Метаданные Flow планируется хранить локально в TeleFlow, если в будущем явно не будет добавлена отдельная опциональная синхронизация.

## Версия 0.1.0 — Foundation

На этапе Foundation TeleFlow отделяется от Telegram Desktop в Windows:

- имя приложения: `TeleFlow`;
- исполняемый файл: `TeleFlow.exe`;
- данные приложения: `%APPDATA%\\TeleFlow`;
- собственный AppId установщика;
- собственный Windows AppUserModelID;
- собственный CLSID обработчика Windows-уведомлений;
- отдельные ярлыки и цель автозапуска;
- upstream-автообновление отключено до появления собственного канала обновлений TeleFlow;
- upstream crash reporting отключён до появления собственной инфраструктуры;
- версия TeleFlow отделена от версии Telegram Desktop, лежащей в основе форка.

Первоначальная Foundation-база привязана к коммиту Telegram Desktop:

`8dc0f52ee5cb17c68ee7b3762edce9a36fcf9955`

Версия Telegram Desktop в этом коммите: **7.1.2**.

## Целевая платформа

Первый релиз ориентирован на:

- Windows 10 22H2 и новее;
- Windows 11;
- x64.

Другие платформы не входят в цели первой версии TeleFlow.

## Telegram API credentials

Для распространяемого стороннего клиента Telegram необходимы собственные `api_id` и `api_hash`. Тестовые credentials Telegram Desktop нельзя использовать в публичной сборке.

Credentials передаются при конфигурации и не должны попадать в Git.

См. `docs/TELEFLOW_BUILDING_WIN.md`.

## Связь с Telegram

TeleFlow — **неофициальный** клиент Telegram. Проект не связан с Telegram FZ-LLC, не одобрен и не публикуется Telegram FZ-LLC.

## Лицензия

Telegram Desktop распространяется по **GNU General Public License v3 или более поздней версии с исключением для линковки OpenSSL**. TeleFlow является производной работой и распространяется на тех же применимых условиях.

См. `LICENSE` и `LEGAL`.

## Автор

**Stanley Lloyd**

GitHub: https://github.com/StanleyLl0yd/teleflow
