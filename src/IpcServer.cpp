#include "nexgen/sys/ipc/IpcServer.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QJsonDocument>

namespace nexgen::sys::ipc {

IpcServer::IpcServer(QObject* parent) : QObject(parent) {
  m_server = new QLocalServer(this);
  connect(m_server, &QLocalServer::newConnection, this, &IpcServer::onNewConnection);
}

IpcServer::~IpcServer() = default;

bool IpcServer::listen(const QString& serverName) {
  // Remove stale socket name (common crash-restart issue on Windows).
  QLocalServer::removeServer(serverName);
  return m_server->listen(serverName);
}

void IpcServer::setHandler(Handler h) {
  m_handler = std::move(h);
}

void IpcServer::onNewConnection() {
  while (auto* sock = m_server->nextPendingConnection()) {
    connect(sock, &QLocalSocket::readyRead, sock, [this, sock]() {
      while (sock->canReadLine()) {
        const QByteArray line = sock->readLine().trimmed();
        const auto req = QJsonDocument::fromJson(line);

        QJsonObject reply{{"ok", false}, {"error", QStringLiteral("bad request")}};
        if (req.isObject()) {
          if (m_handler) reply = m_handler(req.object());
          else reply = QJsonObject{{"ok", false}, {"error", QStringLiteral("no handler")}};
        }

        sock->write(QJsonDocument(reply).toJson(QJsonDocument::Compact));
        sock->write("\n");
        sock->flush();
      }
    });

    connect(sock, &QLocalSocket::disconnected, sock, &QObject::deleteLater);
  }
}

} // namespace nexgen::sys::ipc
