# Building TeleFlow on Windows

TeleFlow 0.1.0 Foundation is based on Telegram Desktop commit:

`8dc0f52ee5cb17c68ee7b3762edce9a36fcf9955`

The upstream Windows build documentation should still be treated as the source of truth for toolchain prerequisites. TeleFlow intentionally keeps the upstream project structure and CMake target name to simplify future merges.

## Target

- Windows 10 22H2 or later
- Windows 11
- x64

## API credentials

A public third-party Telegram client must use its own Telegram API credentials.

Obtain:

- `api_id`
- `api_hash`

from Telegram's API development page.

Never commit real credentials.

Configure using the upstream parameters:

```powershell
python Telegram/configure.py -D TDESKTOP_API_ID=<API_ID> -D TDESKTOP_API_HASH=<API_HASH>
```

Use the exact configure/build invocation required by the current upstream Windows documentation if it differs; the important part is that TeleFlow supplies its own credentials through `TDESKTOP_API_ID` and `TDESKTOP_API_HASH`.

## Foundation safety choices

The TeleFlow patch disables:

- upstream Telegram Desktop automatic updates;
- upstream Telegram Desktop crash reporting.

This prevents a Telegram Desktop release from replacing TeleFlow and avoids sending TeleFlow crash data to an upstream endpoint.

## Expected output

The Windows application target is emitted as:

`TeleFlow.exe`

The application stores installed-user data under:

`%APPDATA%\\TeleFlow`

This allows TeleFlow and the official Telegram Desktop client to coexist with separate local profiles.

## Before any public build

1. Replace Telegram's icon/artwork with original TeleFlow artwork.
2. Use production TeleFlow `api_id` / `api_hash`.
3. Code-sign the executable and installer with a TeleFlow-controlled certificate if available.
4. Confirm the About page clearly says TeleFlow is an unofficial Telegram client.
5. Run side-by-side tests with official Telegram Desktop installed.
6. Verify toast notifications, Start menu shortcuts, autostart and uninstall behavior are isolated.
7. Verify no upstream Telegram Desktop auto-update can overwrite TeleFlow.
