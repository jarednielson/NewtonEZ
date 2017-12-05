#ifndef NEWTONFORMULA_H
#define NEWTONFORMULA_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptValue>

class NewtonFormula : public QObject
{
    Q_OBJECT
public:
    explicit NewtonFormula(QString formula, QObject *parent = 0);

    float evaluate(const QVector<float>& args);
    QString getFormula();

private:
    QScriptEngine engine;
    QString formula;
    QScriptValue fun;
    bool validFunc;
};

#endif // NEWTONFORMULA_H
