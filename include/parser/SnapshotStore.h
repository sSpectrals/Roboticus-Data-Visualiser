#ifndef SNAPSHOTSTORE_H
#define SNAPSHOTSTORE_H

#include "FrameTypes.h"

class SnapshotStore : public QObject {
public:
  explicit SnapshotStore(QObject *parent = nullptr);

  void clear();
  void append(const FrameSnapshot &snapshot);

  int count() const;
  qint64 timestampAt(int index) const;
  QList<qint64> availableTimestamps() const;
  bool isValidIndex(int index) const;
  FrameSnapshot at(int index) const;
  QList<FrameSnapshot> all() const;

private:
  QList<FrameSnapshot> m_snapshots;
};

#endif // SNAPSHOTSTORE_H
