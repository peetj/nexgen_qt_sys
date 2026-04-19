#pragma once

#include <QSettings>

namespace nexgen::sys {

inline QSettings makeSettings() {
  return QSettings(QStringLiteral("Nexgen"), QStringLiteral("Utilities"));
}

} // namespace nexgen::sys
