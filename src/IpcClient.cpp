#include "nexgen/sys/ipc/IpcClient.h"

#include <QLocalSocket>
#include <QJsonDocument>

namespace nexgen::sys::ipc {

IpcClient::IpcClient(QObject* parent) : QObject(parent) {}

QJsonObject IpcClient::request(const QString& serverName, const QJsonObject& message, int timeoutMs) {
  QLocalSocket sock;
  sock.connectToServer(serverName);
  if (!sock.waitForConnected(timeoutMs)) {
    return QJsonObject{{"ok", false}, {"error", QStringLiteral("connect failed")}};
  }

  const QJsonDocument doc(message);
  sock.write(doc.toJson(QJsonDocument::Compact));
  sock.write("\n");
  sock.flush();
  if (!sock.waitForBytesWritten(timeoutMs)) {
    return QJsonObject{{"ok", false}, {"error", QStringLiteral("write timeout")}};
  }

  if (!sock.waitForReadyRead(timeoutMs)) {
    return QJsonObject{{"ok", false}, {"error", QStringLiteral("read timeout")}};
  }

  const QByteArray line = sock.readLine();
  const auto resp = QJsonDocument::fromJson(line.trimmed());
  if (!resp.isObject()) {
    return QJsonObject{{"ok", false}, {"error", QStringLiteral("bad json reply")}};
  }
  return resp.object();
}

} // namespace nexgen::sys::ipc
