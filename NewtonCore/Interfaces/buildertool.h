#ifndef BUILDERTOOL_H
#define BUILDERTOOL_H

#include<QStringList>
#include <QGraphicsItem>

class BuilderToolInterface {
public:
    virtual ~BuilderToolInterface() {}

    virtual QStringList tools() const = 0;
    virtual QMap<QString, QMetaType> properties() const = 0;

    virtual QGraphicsItem addItem(int x, int y, QGraphicsScene * scene) = 0;
    virtual QGraphicsItem addItem(QPoint pos, QGraphicsScene * scene) = 0;

    virtual bool setProperty(QGraphicsItem * item, QString property, void* value);
}

#endif // BUILDERTOOL_H
