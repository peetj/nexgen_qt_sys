#pragma once

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <functional>

namespace nexgen::sys::hotkeys {

class HotkeyManager final : public QObject, public QAbstractNativeEventFilter {
  Q_OBJECT
public:
  explicit HotkeyManager(QObject* parent = nullptr);
  ~HotkeyManager() override;

  // Windows-only for now. Returns false if registration fails.
  bool registerHotkey(int id, bool ctrl, bool alt, bool shift, int vk);
  void unregisterHotkey(int id);

  void setCallback(std::function<void(int id)> cb);

  // QAbstractNativeEventFilter
  bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

private:
  std::function<void(int)> m_cb;
};

} // namespace nexgen::sys::hotkeys
