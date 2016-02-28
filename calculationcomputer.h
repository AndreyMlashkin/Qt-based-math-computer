#ifndef CALCULATIONCOMPUTER_H
#define CALCULATIONCOMPUTER_H

#include <QMap>
#include <QString>
#include "parseradaptor.h"
#include "export.h"

namespace parser
{

class EXPRESSIONCALCULATOR_EXPORT CalculationComputer
{
public:
    CalculationComputer();
    QMap<QString, double> calculateFromFile(const QString& formulsFileName,
                                            const QMap<QString, double>& _sourceData);
private:
    static QString substitute(const QString& _expression, const QMap<QString, double>& _source);

private:
    parser::ParserAdaptor* m_adaptor;
};

}
#endif // CALCULATIONCOMPUTER_H
