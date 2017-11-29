#ifndef NEWTONSCENE_H
#define NEWTONSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>

///
/// \brief The NewtonConversion class
/// Interface for converting from an arbitrary unit system to SI units
/// for mass, distance and time
class NewtonConversion : public QObject{
    Q_OBJECT

public:
    virtual float convertToKg(float unitMass) const = 0;
    virtual float convertToM(float unitDistance) const = 0;
    virtual float convertToS(float unitTime) const = 0;

    virtual QString massSymbol() const = 0;
    virtual QString distanceSymbol() const = 0;
    virtual QString timeSymbol() const = 0;
};

///
/// \brief The NewtonObject class
/// Base class for all objects allowed in the scene
/// This extends QObject so we can get properties
class NewtonObject : public QObject{
    Q_OBJECT

public:
    virtual QString serialize() const = 0;
};

///
/// \brief The NewtonSceneObject class
/// Base class for objects that can be placed in a newton scene
/// These will hold all data for a simulatable object in box2d
class NewtonSceneObject : public NewtonObject {
    Q_OBJECT
    Q_PROPERTY(QPointF initPos READ getInitPos WRITE setInitPos NOTIFY initPosChanged MEMBER m_initPos)

public:
    NewtonSceneObject(QObject *parent = Q_NULLPTR);
    NewtonSceneObject(QPointF initPos, QObject *parent = Q_NULLPTR);

public:
    QPointF getInitPos();
    void setInitPos(QPointF pos);
signals:
    void initPosChanged(QPointF pos);

private:
    QPointF m_initPos;
};

///
/// \brief The NewtonBody class
/// Base class for all objects that are simulated in the scene
class NewtonBody : public NewtonSceneObject {
    Q_OBJECT
    Q_PROPERTY(bool isDynamic READ isDynamic MEMBER m_isDynamic)
    Q_PROPERTY(float mass READ getMass WRITE setMass MEMBER m_Mass NOTIFY massChanged)

    QVector<QPointF> hull;

public:
    ///Constructs a NewtonBody with default property values
    /// isDynamic = false
    /// mass = 1
    /// pos = 0, 0
    /// vertices = {(0,0)}
    NewtonBody(QObject *parent = Q_NULLPTR);

    ///
    /// Constructs a NewtonBody with the passed in properties
    /// The initial position is element 0 of hull.
    /// \brief NewtonBody
    /// \param isDynamic
    /// \param mass
    /// \param hull - Assumes convex set of points given in clockwise order
    /// \param parent
    ///
    NewtonBody(bool isDynamic, float mass, const QVector<QPointF>& hull, QObject *parent = Q_NULLPTR);

    bool isDynamic();
    float getMass();
    void setMass(float mass);

    const QVector<QPointF>& getVertices() const;

signals:
    void massChanged(float mass);

private:
    bool m_isDynamic;
    float m_Mass;
};

///
/// \brief The NewtonScene class
/// Holds global data for drawing and simulating a scene
class NewtonScene : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float gravity READ getGravity WRITE setGravity MEMBER m_Gravity NOTIFY gravityChanged)
    Q_PROPERTY(NewtonConversion units READ getUnits)

    float getGravity();
    void setGravity(float gravity);

    NewtonConversion getUnits();

public:
    explicit NewtonScene(QObject *parent = nullptr);

signals:
    void gravityChanged(float gravity);

private:
    float m_Gravity;

};

#endif // NEWTONSCENE_H
