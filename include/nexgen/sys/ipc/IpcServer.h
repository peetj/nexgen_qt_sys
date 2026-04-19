#pragma once

#include <QObject>
#include <QJsonObject>
#include <functional>

class QLocalServer;

namespace nexgen::sys::ipc {

class IpcServer final : public QObject {
  Q_OBJECT
public:
  using Handler = std::function<QJsonObject(const QJsonObject&)>;

  explicit IpcServer(QObject* parent = nullptr);
  ~IpcServer() override;

  bool listen(const QString& serverName);
  void setHandler(Handler h);

private:
  QLocalServer* m_server = nullptr;
  Handler m_handler;

  void onNewConnection();
};

} // namespace nexgen::sys::ipc
