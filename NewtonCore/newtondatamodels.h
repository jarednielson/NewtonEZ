#ifndef NEWTONSCENE_H
#define NEWTONSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include "newtonformula.h"

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
/// \brief The NewtonCircle struct
/// Holds data for a circle
///
struct NewtonCircle {
    float r;
};

///
/// \brief The NewtonRect struct
/// Holds data for a rect who's cX and cY are the center coordinates of the rect
///
struct NewtonRect {
    float width;
    float height;
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
    NewtonSceneObject(QPointF initPos, float orientation, QObject *parent = Q_NULLPTR);

public:
    QPointF getInitPos() const;
    void setInitPos(QPointF pos);
    float getInitOrientation();
    void setInitOrientation(float theta);
signals:
    void initPosChanged(QPointF pos);
    void initOrientationChanged(float theta);

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
    Q_PROPERTY(Shape shape READ getShapeType MEMBER m_shape)
    Q_PROPERTY(QPointF initVelocity READ getInitVelocity WRITE setInitVelocity MEMBER m_InitVelocity)
    Q_PROPERTY(QPointF initForce READ getInitForce WRITE setInitForce MEMBER m_InitForce)


public:

    enum Shape {Circle, Rect};
    Q_ENUM(Shape)

    union NewtonShape{
        NewtonCircle circle;
        NewtonRect rect;
    };

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
    NewtonBody(bool isDynamic, float mass, float centerX, float centerY, float radius, QObject *parent = Q_NULLPTR);
    NewtonBody(bool isDynamic, float mass, float centerX, float centerY, float width, float height, QObject *parent = Q_NULLPTR);

    bool isDynamic();
    float getMass();
    void setMass(float mass);
    Shape getShapeType();
    NewtonShape getShapeValue();
    void setShape(float centerX, float centerY, float radius);
    void setShape(float centerX, float centerY, float width, float height);
    void setInitVelocity(QPointF velocity);
    QPointF getInitVelocity();
    void setInitForce(QPointF force);
    QPointF getInitVelocity();

    virtual QString serialize() const;

signals:
    void massChanged(float mass);

private:
    bool m_isDynamic;
    float m_Mass;
    Shape m_shape;
    NewtonShape shapeVal;
    QPointF m_InitVelocity;
    QPointF m_InitForce;
};

///
/// \brief The NewtonScene class
/// Holds global data for drawing and simulating a scene
class NewtonScene : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float gravity READ getGravity WRITE setGravity MEMBER m_Gravity NOTIFY gravityChanged)

public:
    ///
    /// \brief NewtonScene constructs an empty NewtonScene with an optional parent.
    /// The gravity is set to -9.8 m/sec
    /// The scene can then be populated with class methods
    /// \param parent
    ///
    explicit NewtonScene(QObject *parent = Q_NULLPTR);
    ///
    /// \brief NewtonScene constructs an empty NewtonScene with the passed in arguments
    /// \param gravity
    /// \param units
    /// \param tutorial
    /// \param brief
    /// \param parent
    ///
    NewtonScene(float gravity, NewtonConversion* units, QString tutorial, QString brief, QObject *parent = Q_NULLPTR);

    ///
    /// \brief getGravity
    /// \return gravity for the scene in the units set up for the scene.
    ///
    float getGravity() const;
    ///
    /// \brief setGravity - Sets the gravity for the scene in scene units.
    /// Gravity can be either positive or negative, with negative being down
    /// \param gravity
    ///
    void setGravity(float gravity);
    ///
    /// \brief getUnits returns a NewtonConversion object for converting scene units to M/k/S
    /// if null scene units are in M/K/S
    /// \return
    ///
    NewtonConversion* getUnits() const;

    ///
    /// \brief addBody - adds the newton body to the scene.
    /// \param body
    ///
    void addBody(NewtonBody* body);
    ///
    /// \brief getBodies all bodies in the scene
    /// \return
    ///
    const QVector<NewtonBody*>& getBodies() const;

    ///
    /// \brief getTutorialText instructions on how to complete the problem
    /// \return
    ///
    QString getTutorialText();
    ///
    /// \brief getBriefDescription - brief description of the problem
    /// \return
    ///
    QString getBriefDescription();

    ///
    /// \brief getWidgetLabels - Labels for each of the input/output widgets
    /// \return
    ///
    QStringList& getDisplayWidgetLabels();
    ///
    /// \brief getWidgetValueRanges - maximum and minimum values for a given parameter for the problem
    /// \return
    ///
    QList<float>& getDisplayWidgetValues();
    ///
    /// \brief getEditableWidgets - Parallel to WidgetLabels, true if the widget can be used
    /// as an input param
    /// \return
    ///
    QStringList& getEditableWidgetLabels();
    ///
    /// \brief getFormulas - Parallell to WidgetLabels, formula for generating value for the widget
    /// given values for other widgets.
    /// \return
    ///
    QList<NewtonFormula*>& getEditableFunctions();


    void addDisplayWidget(QString label, float value);

    void addEditableWidget(QString label, QString function);

signals:
    ///
    /// \brief gravityChanged emitted if the gravity value for the scene has changed
    /// \param gravity
    ///
    void gravityChanged(float gravity);

private:
    float m_Gravity;

    NewtonConversion* units;
    QVector<NewtonBody*> bodies;

    QString tutorial;
    QString briefDescription;

    //Tightly coupled parallel lists
    QStringList displayWidgetLabels;
    QList<float> displayWidgetValues;

    QStringList editableWidgetLabels;
    QList<NewtonFormula*> functions;
};

#endif // NEWTONSCENE_H
