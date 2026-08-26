# История изменений

Здесь фиксируются изменения, относящиеся именно к TeleFlow. История изменений исходного Telegram Desktop сохраняется в upstream-репозитории.

## [0.1.0] - Foundation

### Добавлено
- Отдельная идентичность TeleFlow для Windows x64.
- Собственный AppId Windows-установщика.
- Собственный Windows AppUserModelID.
- Собственный CLSID COM-обработчика toast-уведомлений.
- Отдельный путь данных `%APPDATA%\\TeleFlow`, формируемый через имя приложения TeleFlow.
- Отдельная версия продукта TeleFlow при сохранении версии Telegram Desktop как версии upstream-базы.
- Документация проекта на английском и русском языках.

### Изменено
- Имя Windows executable на `TeleFlow.exe`; внутреннее имя CMake target оставлено upstream-совместимым для упрощения будущих merge.
- Установщик Windows, ярлыки, startup task и version resources переведены на branding TeleFlow.
- Заголовок About и ссылка GitHub изменены на TeleFlow.

### Отключено
- Автоматическое обновление с инфраструктуры Telegram Desktop до появления собственного канала обновлений TeleFlow.
- Upstream crash reporting до появления собственной инфраструктуры TeleFlow.

### Upstream-база
- Telegram Desktop `dev`, коммит `8dc0f52ee5cb17c68ee7b3762edce9a36fcf9955`.
- Telegram Desktop `7.1.2`.
