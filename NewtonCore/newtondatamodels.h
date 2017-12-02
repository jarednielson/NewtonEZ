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
class NewtonConversion {
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
    NewtonObject(QObject *parent = 0) : QObject(parent){}
    virtual QString serialize() const = 0;
};


///
/// \brief The NewtonSceneObject class
/// Base class for objects that can be placed in a newton scene
/// These will hold all data for a simulatable object in box2d
class NewtonSceneObject : public NewtonObject {
    Q_OBJECT
    Q_PROPERTY(QPointF initPos READ getInitPos WRITE setInitPos NOTIFY initPosChanged MEMBER m_initPos)
    Q_PROPERTY(float initOrientation READ getInitOrientation WRITE setInitOrientation NOTIFY initOrientationChanged MEMBER m_initOrientation)

public:
    NewtonSceneObject(QObject *parent = Q_NULLPTR);
    NewtonSceneObject(QPointF initPos, QObject *parent = Q_NULLPTR);

public:
    QPointF getInitPos() const;
    void setInitPos(QPointF pos);
    float getInitOrientation();
    void setInitOrientation(float theta);
signals:
    void initPosChanged(QPointF pos);
    void initOrienationChanged(float theta);

private:
    QPointF m_initPos;
    float m_initOrientation;
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

    virtual QString serialize() const;

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

public:
    explicit NewtonScene(QObject *parent = Q_NULLPTR);
    NewtonScene(float gravity, NewtonConversion* units, QString tutorial, QString brief, QObject *parent = Q_NULLPTR);

    float getGravity() const;
    void setGravity(float gravity);
    NewtonConversion* getUnits() const;
    void addBody(NewtonBody* body);
    const QVector<NewtonBody*>& getBodies() const;

    QString getTutorialText();
    QString getBriefDescription();

    QStringList& getWidgetLabels();
    QList<QPair<float, float>> getWidgetValueRanges();
    QList<bool>& getWidgetEditable();

    void addWidget(QString label, bool editable, QPair<float, float> range);

signals:
    void gravityChanged(float gravity);

private:
    float m_Gravity;
    NewtonConversion* units;
    QVector<NewtonBody*> bodies;

    QString tutorial;
    QString briefDescription;

    QStringList widgetLabels;
    QList<bool> editableWidgets;
    QList<QPair<float, float>> widgetValueRanges;
};

#endif // NEWTONSCENE_H
