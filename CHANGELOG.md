# Changelog

All notable TeleFlow-specific changes are documented here. Telegram Desktop upstream changes remain available in the upstream changelog/history.

## [0.1.0] - Foundation

### Added
- TeleFlow product identity for Windows x64.
- Independent Windows installer AppId.
- Independent Windows AppUserModelID.
- Independent toast notification COM activator CLSID.
- Separate `%APPDATA%\\TeleFlow` application data path through the TeleFlow application name.
- TeleFlow product version separate from the underlying Telegram Desktop version.
- English and Russian project documentation.

### Changed
- Windows executable output name to `TeleFlow.exe` while retaining the upstream CMake target name to reduce merge conflicts.
- Windows installer, shortcuts, startup task and version resources branded as TeleFlow.
- About dialog title and GitHub project link changed to TeleFlow.

### Disabled
- Telegram Desktop upstream automatic updates, until TeleFlow has its own release/update channel.
- Telegram Desktop upstream crash reporting, until TeleFlow has its own endpoint.

### Upstream base
- Telegram Desktop `dev` commit `8dc0f52ee5cb17c68ee7b3762edce9a36fcf9955`.
- Telegram Desktop version `7.1.2`.
