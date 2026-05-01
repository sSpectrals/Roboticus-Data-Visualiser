#ifndef SNAPSHOTLOADER_H
#define SNAPSHOTLOADER_H

#include <QUrl>

#include "parser/FrameTypes.h"

class SnapshotLoader {
public:
  bool save(const QUrl &filePath, const QList<FrameSnapshot> &snapshots,
            QString *errorMessage = nullptr) const;
  bool load(const QUrl &filePath, QList<FrameSnapshot> *snapshots,
            QString *errorMessage = nullptr) const;
};

#endif // SNAPSHOTLOADER_H
