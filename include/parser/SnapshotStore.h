#ifndef SNAPSHOTSTORE_H
#define SNAPSHOTSTORE_H

#include "FrameTypes.h"

class SnapshotStore : public QObject {
public:
  explicit SnapshotStore(QObject *parent = nullptr);

    /**
     * @brief Removes all stored snapshots.
     */
    void clear();

    /**
     * @brief Appends a new snapshot to the end of the store.
     * @param snapshot The frame snapshot to store.
     */
    void append(const FrameSnapshot &snapshot);

    /**
     * @brief Returns the total number of stored snapshots.
     */
    int count() const;

    /**
     * @brief Returns the timestamp of the snapshot at the given index.
     * @param index Zero-based index into the snapshot list.
     * @return Timestamp in milliseconds, or -1 if the index is out of range.
     */
    qint64 timestampAt(int index) const;

    /**
     * @brief Returns a list of all stored timestamps in order of arrival.
     */
    QList<qint64> availableTimestamps() const;

    /**
     * @brief Checks whether the given index refers to a valid snapshot.
     * @param index Zero-based index to validate.
     * @return True if the index is within bounds, false otherwise.
     */
    bool isValidIndex(int index) const;

    /**
     * @brief Returns the snapshot at the given index.
     * @param index Zero-based index into the snapshot list.
     * @return The snapshot at that index, or a default-constructed snapshot if out of range.
     */
    FrameSnapshot at(int index) const;

    /**
     * @brief Returns a copy of all stored snapshots.
     */
    QList<FrameSnapshot> all() const;

private:
  QList<FrameSnapshot> m_snapshots;
};

#endif // SNAPSHOTSTORE_H
