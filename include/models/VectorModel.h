#ifndef VECTORMODEL_H
#define VECTORMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <QQmlEngine>
#include <QUuid>

/**
 * @brief Represents a single vector with its properties and position.
 *        The id is automatically generated on construction.
 */
struct Vector
{
    QUuid id;
    QString name = "No Name Set";
    double rotation = 0.0;
    double scale = 1.0;
    QColor color = "white";
    QString layer = "Layer 1";
    double x = 0.0;
    double y = 0.0;

    Vector()
        : id(QUuid::createUuid())
    {}
};

/**
 * @brief List model that stores and manages vectors, exposed to QML via QML_ELEMENT.
 *        Emits signals when vectors are added, removed, or updated so other
 *        components (e.g. the graph) can react without polling the model.
 */
class VectorModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * @brief The list of unique layer names currently present in the model.
     *        Updated automatically when a vector with a new layer is added.
     */
    Q_PROPERTY(QList<QString> layers READ layers NOTIFY layersChanged)

public:
    /**
     * @brief Roles used to access vector fields from QML delegates.
     */
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        RotationRole,
        ScaleRole,
        ColorRole,
        LayerRole,
        XRole,
        YRole
    };
    Q_ENUM(Roles)

    explicit VectorModel(QObject *parent = nullptr);

    /** @brief Returns the list of unique layer names across all vectors. */
    QList<QString> layers() const { return m_layers; }

    /** @brief Returns all vectors as a flat list, e.g. for serialization. */
    QList<Vector> getAllVectors() const { return m_vectors; }

    // QAbstractListModel overrides
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Creates a new vector, appends it to the model, and emits vectorAdded.
     *        If name is empty, a default name is generated based on the current count.
     * @return The newly created Vector, including its generated id.
     */
    Q_INVOKABLE Vector addVector(QString name = QString(),
                                 double rotation = 0.0,
                                 double scale = 1.0,
                                 QColor color = "white",
                                 QString layer = "Layer 1",
                                 double x = 0.0,
                                 double y = 0.0);

    /**
     * @brief Removes the vector with the given id from the model and emits vectorRemoved.
     * @return True if the vector was found and removed, false if the id was not found.
     */
    Q_INVOKABLE bool removeVector(const QUuid &id);

    /** @brief Removes all vectors from the model and resets it. */
    Q_INVOKABLE void clear();

    /**
     * @brief Returns the row index of the vector with the given id.
     * @return The zero-based index, or -1 if not found.
     */
    Q_INVOKABLE int getIndexFromId(const QUuid &id) const;

    /**
     * @brief Returns the vector with the given id.
     * @return The matching Vector, or a default-constructed Vector if not found.
     */
    Q_INVOKABLE Vector getVectorById(const QUuid &id) const;

    /**
     * @brief Returns the row index of the first vector with the given name.
     * @return The zero-based index, or -1 if not found.
     */
    Q_INVOKABLE int getIndexByName(const QString &name) const;

signals:
    /** @brief Emitted after a vector is successfully added to the model. */
    void vectorAdded(const QUuid &id,
                     const QString &name,
                     double rotation,
                     double scale,
                     const QColor &color,
                     const QString &layer,
                     double x,
                     double y);

    /** @brief Emitted after a vector is successfully removed from the model. */
    void vectorRemoved(const QUuid &id);

    /**
     * @brief Emitted after any property of an existing vector changes via setData.
     *        Used by the graph to update the visual representation of the vector.
     */
    void vectorUpdated(const QUuid &id,
                       const QString &name,
                       double rotation,
                       double scale,
                       const QColor &color,
                       const QString &layer,
                       double x,
                       double y);

    /** @brief Emitted when a vector with a previously unseen layer name is added. */
    void layersChanged();

private:
    QList<Vector> m_vectors;
    QList<QString> m_layers;
};

#endif // VECTORMODEL_H