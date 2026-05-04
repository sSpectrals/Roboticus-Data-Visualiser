#ifndef SNAPSHOTLOADER_H
#define SNAPSHOTLOADER_H

#include "FrameTypes.h"

#include <QUrl>

class SnapshotLoader : public QObject {
public:
  explicit SnapshotLoader(QObject *parent = nullptr);

  bool save(const QUrl &filePath, const QList<FrameSnapshot> &snapshots,
            QString *errorMessage = nullptr) const;
  bool load(const QUrl &filePath, QList<FrameSnapshot> *snapshots,
            QString *errorMessage = nullptr) const;
};

#endif // SNAPSHOTLOADER_H
