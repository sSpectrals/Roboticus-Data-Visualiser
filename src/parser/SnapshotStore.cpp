#include "parser/SnapshotStore.h"

SnapshotStore::SnapshotStore(QObject *parent) : QObject(parent) {}


void SnapshotStore::clear() { m_snapshots.clear(); }


void SnapshotStore::append(const FrameSnapshot &snapshot) {
  m_snapshots.append(snapshot);
}


int SnapshotStore::count() const { return m_snapshots.size(); }


qint64 SnapshotStore::timestampAt(int index) const {
  if (!isValidIndex(index)) {
    return -1;
  }
  return m_snapshots.at(index).timestamp;
}


QList<qint64> SnapshotStore::availableTimestamps() const {
  QList<qint64> result;
  result.reserve(m_snapshots.size());
  for (const auto &snapshot : m_snapshots) {
    result.append(snapshot.timestamp);
  }
  return result;
}


bool SnapshotStore::isValidIndex(int index) const {
  return index >= 0 && index < m_snapshots.size();
}


FrameSnapshot SnapshotStore::at(int index) const {
  if (!isValidIndex(index)) {
    return {};
  }
  return m_snapshots.at(index);
}


QList<FrameSnapshot> SnapshotStore::all() const { return m_snapshots; }
