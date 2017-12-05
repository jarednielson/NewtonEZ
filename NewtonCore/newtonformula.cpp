#include "newtonformula.h"
#include <QVector>
#include <cfloat>

NewtonFormula::NewtonFormula(QString formula, QObject *parent) : QObject(parent),
    formula(formula)
{
    fun = engine.evaluate(formula);
    validFunc = !engine.hasUncaughtException();
}

float NewtonFormula::evaluate(const QVector<float>& vals)
{
    QScriptValueList args;
    for(size_t i = 0; i < vals.size(); i++){
        args << vals[i];
    }

    QScriptValue res = fun.call(QScriptValue(), args).toNumber();
    if(!res.isNumber()){
        return FLT_MAX;
    }
    return (float) res.toNumber();
}

QString NewtonFormula::getFormula(){
    return formula;
}
