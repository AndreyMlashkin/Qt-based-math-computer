#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "calculationcomputer.h"

namespace parser
{

CalculationComputer::CalculationComputer()
    : m_adaptor(new ParserAdaptor())
{}

QMap<QString, double> CalculationComputer::calculateFromFile(const QString &formulsFileName,
                                                             const QMap<QString, double> &_sourceData)
{
    QMap<QString, double> ans;
    QFile file(formulsFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "no file formuls.txt";
        return ans;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().simplified();
        if(line.isEmpty())
            continue;

        int equalSignPos = line.indexOf("=");
        QString lSide = line.left(equalSignPos).simplified();

        int equalSignPosFromRight = line.size() - equalSignPos - 1;
        line = line.right(equalSignPosFromRight).simplified();
        line = substitute(line, _sourceData);

        QString calculated = m_adaptor->calculate(line);
        bool isOk = false;
        double calculatedMean = calculated.toDouble(&isOk);
        if(!isOk)
            qDebug() << "can not cast to double";

        isOk = std::isnan(calculatedMean) || std::isinf(calculatedMean);
        if(isOk)
            qDebug() << "wrong calculated" << lSide << "=" << line;
        ans[lSide] = calculatedMean;
    }
    return ans;
}

QString CalculationComputer::substitute(const QString &_expression, const QMap<QString, double> &_source)
{
    QString ans = _expression;
    QStringList rSide = _expression.split(QRegExp("[\\s+=*/()]"), QString::SkipEmptyParts);

    for(QString operand : rSide)
    {
        static QRegExp variableMatcher("^[a-zA-Z_][a-zA-Z0-9_]*$");
        bool isVariable = operand.contains(variableMatcher);
        if(!isVariable)
        {
            static QRegExp constantsMatcher("^[-+]?[0-9]*\\.?[0-9]*$");
            if(!operand.contains(constantsMatcher))
            {
                qDebug() << "Wrong argument passed in calculator:" << operand;
                Q_ASSERT(false);
            }
            continue;
        }

        if(!_source.contains(operand))
        {
            qDebug() << "WARNING! There is no tag with name " << operand;
            break; // !!!
        }
        double operandMean = _source[operand];
        ans.replace(operand, QString::number(operandMean));
    }

    return ans;
}

}
