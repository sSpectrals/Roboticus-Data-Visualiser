#pragma once

#include <QObject>
#include <QQmlEngine>
#include "../models/VectorModel.h"

/**
 * @brief Facade over VectorModel that provides a QML-friendly API for
 *        adding, removing, and updating vectors.
 *
 *        Owns the VectorModel and forwards its signals outward. Also emits
 *        clearChartSeries and vectorAdded when the active layer changes,
 *        so the graph can repopulate without knowing about the model directly.
 */
class VectorController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    /** @brief The underlying list model, exposed for use in QML views and delegates. */
    Q_PROPERTY(VectorModel *model READ model CONSTANT)

    /** @brief Mirrors VectorModel::layers; updated whenever a new layer appears. */
    Q_PROPERTY(QList<QString> layers READ layers NOTIFY layersChanged)

public:
    explicit VectorController(QObject *parent = nullptr);

    VectorModel *model() const { return m_model; }
    QList<QString> layers() const { return m_model ? m_model->layers() : QList<QString>(); }

    /**
     * @brief Creates and registers a new vector in the model.
     *        If name is empty, the model assigns a default name.
     * @return The newly created Vector, including its generated id.
     */
    Q_INVOKABLE Vector addVector(const QString &name = QString(),
                                 double rotation = 0,
                                 double scale = 1,
                                 QColor color = "white",
                                 QString layer = "Layer 1",
                                 double x = 0.0,
                                 double y = 0.0);

    /**
     * @brief Removes the vector with the given id from the model.
     * @return True if found and removed, false if the id did not exist.
     */
    Q_INVOKABLE bool removeVector(const QUuid &id);

    /**
     * @brief Updates the rotation angle of a vector in degrees.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setVectorRotation(const QUuid &id, double rotation);

    /**
     * @brief Updates the scale factor of a vector.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setVectorScale(const QUuid &id, double scale);

    /**
     * @brief Updates the display color of a vector.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setVectorColor(const QUuid &id, const QColor &color);

    /**
     * @brief Renames a vector.
     * @return True if the name changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setVectorName(const QUuid &id, const QString &name);

    /**
     * @brief Updates both X and Y position of a vector in one call.
     * @return True if either coordinate changed, false if not found or both unchanged.
     */
    Q_INVOKABLE bool setVectorPositionXY(const QUuid &id, double x, double y);

    /**
     * @brief Updates the X position of a vector.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setVectorPositionX(const QUuid &id, double x);

    /**
     * @brief Updates the Y position of a vector.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setVectorPositionY(const QUuid &id, double y);

    /**
     * @brief Switches the active layer, emitting clearChartSeries followed by
     *        vectorAdded for every vector that belongs to the new layer.
     *        This lets the graph repopulate without querying the model directly.
     * @param layerName The layer to switch to.
     */
    Q_INVOKABLE void setActiveLayer(const QString &layerName);

signals:
    /** @brief Forwarded from VectorModel::vectorAdded. Also emitted by setActiveLayer. */
    void vectorAdded(const QUuid &id,
                     const QString &name,
                     double rotation,
                     double scale,
                     const QColor &color,
                     const QString &layer,
                     double x,
                     double y);

    /** @brief Forwarded from VectorModel::vectorRemoved. */
    void vectorRemoved(const QUuid &id);

    /** @brief Forwarded from VectorModel::vectorUpdated. */
    void vectorUpdated(const QUuid &id,
                       const QString &name,
                       double rotation,
                       double scale,
                       const QColor &color,
                       const QString &layer,
                       double x,
                       double y);

    /** @brief Emitted by setActiveLayer before re-emitting vectors for the new layer. */
    void clearChartSeries();

    /** @brief Forwarded from VectorModel::layersChanged. */
    void layersChanged();

    /** @brief Emitted when a model operation fails, e.g. unknown id. */
    void errorOccurred(QString message);

private:
    VectorModel *m_model;
};
