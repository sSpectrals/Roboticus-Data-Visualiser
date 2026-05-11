#ifndef SNAPSHOTLOADER_H
#define SNAPSHOTLOADER_H

#include "FrameTypes.h"

#include <QUrl>

class SnapshotLoader : public QObject {
public:
  explicit SnapshotLoader(QObject *parent = nullptr);

    /**
     * @brief Serializes a list of snapshots to a compact JSON file.
     * @param filePath   Destination file path as a QUrl.
     * @param snapshots  The snapshots to serialize.
     * @param errorMessage Optional output string populated on failure.
     * @return True on success, false on any file or write error.
     */
    bool save(const QUrl &filePath, const QList<FrameSnapshot> &snapshots,
            QString *errorMessage = nullptr) const;


    /**
     * @brief Deserializes a list of snapshots from a JSON file.
     * @param filePath     Source file path as a QUrl.
     * @param snapshots    Output list populated on success.
     * @param errorMessage Optional output string populated on failure.
     * @return True on success, false on any file, parse, or format error.
     */
    bool load(const QUrl &filePath, QList<FrameSnapshot> *snapshots,
            QString *errorMessage = nullptr) const;
};

#endif // SNAPSHOTLOADER_H
