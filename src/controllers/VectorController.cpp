#include "VectorController.h"
#include "controllers/ControllerUtils.h"

VectorController::VectorController(QObject *parent)
    : QObject(parent)
    , m_model(new VectorModel(this))
{
    connect(m_model, &VectorModel::vectorAdded, this, &VectorController::vectorAdded);
    connect(m_model, &VectorModel::vectorRemoved, this, &VectorController::vectorRemoved);
    connect(m_model, &VectorModel::vectorUpdated, this, &VectorController::vectorUpdated);
    connect(m_model, &VectorModel::layersChanged, this, &VectorController::layersChanged);
}

Q_INVOKABLE Vector VectorController::addVector(const QString &name,
                                               double rotation,
                                               double scale,
                                               QColor color,
                                               QString layer,
                                               double x,
                                               double y)
{
    if (!m_model)
        return Vector();

    Vector newVector = m_model->addVector(name, rotation, scale, color, layer, x, y);

    return newVector;
}

Q_INVOKABLE bool VectorController::removeVector(const QUuid &id)
{
    if (!m_model)
        return false;

    Vector removedVector = m_model->getVectorById(id);

    return m_model->removeVector(id);
}

Q_INVOKABLE bool VectorController::setVectorRotation(const QUuid &id, double rotation)
{
    return setModelValueById(
        m_model, id, rotation, VectorModel::RotationRole,
        [this](const QString &message) { emit errorOccurred(message); }, "rotation", "vector");
}

Q_INVOKABLE bool VectorController::setVectorScale(const QUuid &id, double scale)
{
    return setModelValueById(
        m_model, id, scale, VectorModel::ScaleRole,
        [this](const QString &message) { emit errorOccurred(message); }, "scale", "vector");
}

Q_INVOKABLE bool VectorController::setVectorColor(const QUuid &id, const QColor &color)
{
    return setModelValueById(
        m_model, id, color, VectorModel::ColorRole,
        [this](const QString &message) { emit errorOccurred(message); }, "color", "vector");
}

Q_INVOKABLE bool VectorController::setVectorName(const QUuid &id, const QString &name)
{
    return setModelValueById(
        m_model, id, name, VectorModel::NameRole,
        [this](const QString &message) { emit errorOccurred(message); }, "name", "vector");
}

Q_INVOKABLE bool VectorController::setVectorPositionXY(const QUuid &id, double x, double y)
{
    if (!m_model)
        return false;

    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(QString("Attempted to set position for non-existent vector with id: %1")
                               .arg(id.toString()));
        return false;
    }

    bool xChanged = m_model->setData(m_model->index(index), x, VectorModel::XRole);
    bool yChanged = m_model->setData(m_model->index(index), y, VectorModel::YRole);

    return xChanged || yChanged;
}

Q_INVOKABLE bool VectorController::setVectorPositionX(const QUuid &id, double x)
{
    return setModelValueById(
        m_model, id, x, VectorModel::XRole,
        [this](const QString &message) { emit errorOccurred(message); }, "position X", "vector");
}

Q_INVOKABLE bool VectorController::setVectorPositionY(const QUuid &id, double y)
{
    return setModelValueById(
        m_model, id, y, VectorModel::YRole,
        [this](const QString &message) { emit errorOccurred(message); }, "position Y", "vector");
}

Q_INVOKABLE void VectorController::setActiveLayer(const QString &layerName)
{
    if (!m_model)
        return;

    emit clearChartSeries();

    const auto &vectors = m_model->getAllVectors();
    for (const auto &vector : vectors) {
        if (vector.layer == layerName) {
            emit vectorAdded(vector.id,
                             vector.name,
                             vector.rotation,
                             vector.scale,
                             vector.color,
                             vector.layer,
                             vector.x,
                             vector.y);
        }
    }
}
