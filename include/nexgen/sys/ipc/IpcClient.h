#pragma once

#include <QObject>
#include <QJsonObject>

namespace nexgen::sys::ipc {

class IpcClient final : public QObject {
  Q_OBJECT
public:
  explicit IpcClient(QObject* parent = nullptr);

  // Connect-send-receive with a short timeout. Returns {ok:false, error:"..."} on failure.
  QJsonObject request(const QString& serverName, const QJsonObject& message, int timeoutMs = 250);
};

} // namespace nexgen::sys::ipc
