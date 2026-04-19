#include "nexgen/sys/hotkeys/HotkeyManager.h"

#include <QAbstractNativeEventFilter>
#include <QCoreApplication>

#ifdef _WIN32
  #include <windows.h>
#endif

namespace nexgen::sys::hotkeys {

HotkeyManager::HotkeyManager(QObject* parent) : QObject(parent) {
  // Install ourselves as native event filter.
  QCoreApplication::instance()->installNativeEventFilter(reinterpret_cast<QAbstractNativeEventFilter*>(this));
}

HotkeyManager::~HotkeyManager() {
  // Best-effort: unregister all is the caller's job for now.
  QCoreApplication::instance()->removeNativeEventFilter(reinterpret_cast<QAbstractNativeEventFilter*>(this));
}

void HotkeyManager::setCallback(std::function<void(int)> cb) { m_cb = std::move(cb); }

bool HotkeyManager::registerHotkey(int id, bool ctrl, bool alt, bool shift, int vk) {
#ifdef _WIN32
  UINT mod = 0;
  if (ctrl) mod |= MOD_CONTROL;
  if (alt) mod |= MOD_ALT;
  if (shift) mod |= MOD_SHIFT;
  return RegisterHotKey(nullptr, id, mod, static_cast<UINT>(vk));
#else
  Q_UNUSED(id); Q_UNUSED(ctrl); Q_UNUSED(alt); Q_UNUSED(shift); Q_UNUSED(vk);
  return false;
#endif
}

void HotkeyManager::unregisterHotkey(int id) {
#ifdef _WIN32
  UnregisterHotKey(nullptr, id);
#else
  Q_UNUSED(id);
#endif
}

bool HotkeyManager::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
#ifdef _WIN32
  Q_UNUSED(eventType);
  MSG* msg = reinterpret_cast<MSG*>(message);
  if (msg->message == WM_HOTKEY) {
    const int id = static_cast<int>(msg->wParam);
    if (m_cb) m_cb(id);
    if (result) *result = 0;
    return true;
  }
#else
  Q_UNUSED(eventType); Q_UNUSED(message); Q_UNUSED(result);
#endif
  return false;
}

} // namespace nexgen::sys::hotkeys
