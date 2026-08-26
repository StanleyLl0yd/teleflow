#!/usr/bin/env python3
from pathlib import Path
import sys

ROOT = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path.cwd().resolve()

replacements = {
    'Telegram/SourceFiles/core/version.h': [
        (
'''constexpr auto AppId = "{53F49750-6209-4FBF-9CA8-7A333C87D1ED}"_cs;
constexpr auto AppNameOld = "Telegram Win (Unofficial)"_cs;
constexpr auto AppName = "Telegram Desktop"_cs;
constexpr auto AppFile = "Telegram"_cs;
constexpr auto AppVersion = 7001002;
constexpr auto AppVersionStr = "7.1.2";
constexpr auto AppBetaVersion = false;
constexpr auto AppAlphaVersion = TDESKTOP_ALPHA_VERSION;
''',
'''constexpr auto AppId = "{1934CBAF-F72A-43EC-B0CD-94ECEED6FA86}"_cs;
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
'''
        ),
    ],
    'Telegram/CMakeLists.txt': [
        (
'''add_executable(Telegram WIN32 MACOSX_BUNDLE)
init_non_host_target(Telegram)
''',
'''add_executable(Telegram WIN32 MACOSX_BUNDLE)
init_non_host_target(Telegram)

if (WIN32)
    # Keep the upstream CMake target name to minimize merge conflicts, but
    # ship an independent Windows executable.
    set_target_properties(Telegram PROPERTIES OUTPUT_NAME "TeleFlow")
endif()
'''
        ),
        (
'''include(cmake/telegram_options.cmake)
include(cmake/lib_ffmpeg.cmake)
''',
'''include(cmake/telegram_options.cmake)

if (WIN32)
    # Never let an upstream Telegram Desktop update replace TeleFlow.
    # Crash reporting is also disabled until TeleFlow has its own endpoint.
    target_compile_definitions(Telegram PRIVATE
        TDESKTOP_DISABLE_AUTOUPDATE
        TDESKTOP_DISABLE_CRASH_REPORTS
    )
endif()

include(cmake/lib_ffmpeg.cmake)
'''
        ),
    ],
    'Telegram/build/setup.iss': [
        (
'''#define MyAppShortName "Telegram"
#define MyAppName "Telegram Desktop"
#define MyAppPublisher "Telegram FZ-LLC"
#define MyAppURL "https://desktop.telegram.org"
#define MyAppExeName "Telegram.exe"
#define MyAppId "53F49750-6209-4FBF-9CA8-7A333C87D1ED"
''',
'''#define MyAppShortName "TeleFlow"
#define MyAppName "TeleFlow"
#define MyAppPublisher "Stanley Lloyd"
#define MyAppURL "https://github.com/StanleyLl0yd/teleflow"
#define MyAppExeName "TeleFlow.exe"
#define MyAppId "1934CBAF-F72A-43EC-B0CD-94ECEED6FA86"
'''
        ),
        ('UninstallDisplayIcon={app}\\Telegram.exe', 'UninstallDisplayIcon={app}\\TeleFlow.exe'),
        ('OutputBaseFilename=tsetup-x64.{#MyAppVersionFull}', 'OutputBaseFilename=teleflow-setup-x64.{#MyAppVersionFull}'),
        ('OutputBaseFilename=tsetup.{#MyAppVersionFull}', 'OutputBaseFilename=teleflow-setup.{#MyAppVersionFull}'),
        ('Source: "{#ReleasePath}\\Telegram.exe"; DestDir: "{app}"; Flags: ignoreversion', 'Source: "{#ReleasePath}\\TeleFlow.exe"; DestDir: "{app}"; Flags: ignoreversion'),
    ],
    'Telegram/SourceFiles/_other/startup_task_win.cpp': [
        ('(directory + L"\\\\Telegram.exe").c_str()', '(directory + L"\\\\TeleFlow.exe").c_str()'),
    ],
    'Telegram/SourceFiles/_other/updater_win.cpp': [
        ('equal(tofname, updateTo + L"Telegram.exe") && exeName != L"Telegram.exe"', 'equal(tofname, updateTo + L"TeleFlow.exe") && exeName != L"TeleFlow.exe"'),
    ],
    'Telegram/SourceFiles/platform/win/windows_app_user_model_id.cpp': [
        ('const WCHAR AppUserModelIdBase[] = L"Telegram.TelegramDesktop.Store";', 'const WCHAR AppUserModelIdBase[] = L"TeleFlow.TeleFlowDesktop.Store";'),
        ('const WCHAR AppUserModelIdBase[] = L"Telegram.TelegramDesktop";', 'const WCHAR AppUserModelIdBase[] = L"TeleFlow.TeleFlowDesktop";'),
        ('systemShortcutPath() + u"Telegram.lnk"_q', 'systemShortcutPath() + u"TeleFlow.lnk"_q'),
        (
'''\tconst auto installed = u"Telegram Desktop/Telegram.lnk"_q;
\tconst auto old = u"Telegram Win (Unofficial)/Telegram.lnk"_q;
\treturn validateShortcutAt(path + installed)
\t\t|| validateShortcutAt(path + old);
''',
'''\tconst auto installed = u"TeleFlow/TeleFlow.lnk"_q;
\treturn validateShortcutAt(path + installed);
'''
        ),
        ('path += u"TelegramAlpha.lnk"_q;', 'path += u"TeleFlowAlpha.lnk"_q;'),
        ('path += u"Telegram.lnk"_q;', 'path += u"TeleFlow.lnk"_q;'),
    ],
    'Telegram/SourceFiles/platform/win/windows_toast_activator.h': [
        (
'''// {F11932D3-6110-4BBC-9B02-B2EC07A1BD19}
class DECLSPEC_UUID("F11932D3-6110-4BBC-9B02-B2EC07A1BD19") ToastActivator
''',
'''// TeleFlow-specific COM class id. Must not collide with Telegram Desktop.
// {A71A18F3-931C-4986-AF79-000B3AC9C98F}
class DECLSPEC_UUID("A71A18F3-931C-4986-AF79-000B3AC9C98F") ToastActivator
'''
        ),
    ],
    'Telegram/SourceFiles/platform/win/specific_win.cpp': [
        ('QString::fromWCharArray(userDesktopFolder) + "\\\\Telegram.lnk"', 'QString::fromWCharArray(userDesktopFolder) + "\\\\TeleFlow.lnk"'),
        ('QString::fromWCharArray(commonDesktopFolder) + "\\\\Telegram.lnk"', 'QString::fromWCharArray(commonDesktopFolder) + "\\\\TeleFlow.lnk"'),
    ],
    'Telegram/Resources/winrc/Telegram.rc': [
        (' FILEVERSION 7,1,2,0\n PRODUCTVERSION 7,1,2,0', ' FILEVERSION 0,1,0,0\n PRODUCTVERSION 0,1,0,0'),
        (
'''            VALUE "CompanyName", "Telegram FZ-LLC"
            VALUE "FileDescription", "Telegram Desktop"
            VALUE "FileVersion", "7.1.2.0"
            VALUE "LegalCopyright", "Copyright (C) 2014-2026"
            VALUE "ProductName", "Telegram Desktop"
            VALUE "ProductVersion", "7.1.2.0"
''',
'''            VALUE "CompanyName", "Stanley Lloyd"
            VALUE "FileDescription", "TeleFlow - unofficial Telegram client with a smart inbox"
            VALUE "FileVersion", "0.1.0.0"
            VALUE "LegalCopyright", "TeleFlow modifications Copyright (C) 2026 Stanley Lloyd"
            VALUE "ProductName", "TeleFlow"
            VALUE "ProductVersion", "0.1.0.0"
'''
        ),
    ],
    'Telegram/Resources/winrc/Updater.rc': [
        (' FILEVERSION 7,1,2,0\n PRODUCTVERSION 7,1,2,0', ' FILEVERSION 0,1,0,0\n PRODUCTVERSION 0,1,0,0'),
        (
'''            VALUE "CompanyName", "Telegram FZ-LLC"
            VALUE "FileDescription", "Telegram Desktop Updater"
            VALUE "FileVersion", "7.1.2.0"
            VALUE "LegalCopyright", "Copyright (C) 2014-2026"
            VALUE "ProductName", "Telegram Desktop"
            VALUE "ProductVersion", "7.1.2.0"
''',
'''            VALUE "CompanyName", "Stanley Lloyd"
            VALUE "FileDescription", "TeleFlow Updater"
            VALUE "FileVersion", "0.1.0.0"
            VALUE "LegalCopyright", "TeleFlow modifications Copyright (C) 2026 Stanley Lloyd"
            VALUE "ProductName", "TeleFlow"
            VALUE "ProductVersion", "0.1.0.0"
'''
        ),
    ],
    'Telegram/SourceFiles/boxes/about_box.cpp': [
        ('"https://github.com/telegramdesktop/tdesktop")),', '"https://github.com/StanleyLl0yd/teleflow")),'),
        ('box->setTitle(u"Telegram Desktop"_q);', 'box->setTitle(u"TeleFlow"_q);'),
        (
'''QString currentVersionText() {
\tauto result = QString::fromLatin1(AppVersionStr);
''',
'''QString currentVersionText() {
\tauto result = u"%1 (Telegram %2)"_q
\t\t.arg(QString::fromLatin1(TeleFlowVersionStr))
\t\t.arg(QString::fromLatin1(AppVersionStr));
'''
        ),
    ],
}

changed = []
for rel, ops in replacements.items():
    path = ROOT / rel
    if not path.exists():
        raise SystemExit(f'Missing expected upstream file: {rel}')
    text = path.read_text(encoding='utf-8')
    original = text
    for old, new in ops:
        count = text.count(old)
        if count != 1:
            raise SystemExit(f'Expected exactly one match in {rel}, found {count}: {old[:100]!r}')
        text = text.replace(old, new, 1)
    if text != original:
        path.write_text(text, encoding='utf-8', newline='\n')
        changed.append(rel)

print('TeleFlow Foundation changes applied:')
for rel in changed:
    print(f'  - {rel}')
