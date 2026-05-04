#ifndef VECTORMODEL_H
#define VECTORMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <QQmlEngine>
#include <QUuid>

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

class VectorModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<QString> layers READ layers NOTIFY layersChanged)
public:
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

    QList<QString> layers() const { return m_layers; }

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE Vector addVector(QString name = QString(),
                                 double rotation = 0.0,
                                 double scale = 1.0,
                                 QColor color = "white",
                                 QString layer = "Layer 1",
                                 double x = 0.0,
                                 double y = 0.0);
    Q_INVOKABLE bool removeVector(const QUuid &id);
    Q_INVOKABLE void clear();

    Q_INVOKABLE int getIndexFromId(const QUuid &id) const;
    Q_INVOKABLE Vector getVectorById(const QUuid &id) const;
    Q_INVOKABLE int getIndexByName(const QString &name) const;
    QList<Vector> getAllVectors() const { return m_vectors; }

signals:
    void vectorAdded(const QUuid &id,
                     const QString &name,
                     double rotation,
                     double scale,
                     const QColor &color,
                     const QString &layer,
                     double x,
                     double y);
    void vectorRemoved(const QUuid &id);
    void vectorUpdated(const QUuid &id,
                       const QString &name,
                       double rotation,
                       double scale,
                       const QColor &color,
                       const QString &layer,
                       double x,
                       double y);
    void layersChanged();

private:
    QList<Vector> m_vectors;
    QList<QString> m_layers;
};

#endif // VECTORMODEL_H
