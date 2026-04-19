# nexgen_qt_sys

Shared system glue for Nexgen Qt utilities.

Includes:
- QSettings conventions (`Nexgen` / `Utilities`)
- IPC helper (QLocalServer/QLocalSocket) using newline-delimited compact JSON
- Windows global hotkeys (RegisterHotKey)

## IPC names
Utilities should listen on a stable server name, e.g.
- `nexgen.clock`
- `nexgen.finder`

The tray hub connects and sends commands like `{ "cmd": "toggle" }`.
