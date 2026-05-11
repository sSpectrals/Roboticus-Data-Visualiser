#ifndef CONTROLLERUTILS_H
#define CONTROLLERUTILS_H

#include <QVariant>
#include <QUuid>


template <typename ModelT, typename ErrorEmitterT>
bool setModelValueById(ModelT *model, const QUuid &id, const QVariant &value,
                       int role, ErrorEmitterT emitError, const QString &fieldName,
                       const QString &entityName)
{
    if (!model) {
        return false;
    }

    const int index = model->getIndexFromId(id);
    if (index < 0) {
        emitError(QString("Attempted to set %1 for non-existent %2 with id: %3")
                      .arg(fieldName, entityName, id.toString()));
        return false;
    }

    return model->setData(model->index(index), value, role);
}

#endif // CONTROLLERUTILS_H
