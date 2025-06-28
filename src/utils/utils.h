#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QStringList>
#include <QDebug>
#include <qregularexpression.h>

namespace Utils {

Eigen::MatrixXd loadMatrixFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return Eigen::MatrixXd();
    }

    QVector<QVector<double>> rows;
    int expectedCols = -1;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList tokens = line.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
        QVector<double> values;
        for (const QString &token : std::as_const(tokens)) {
            bool ok;
            double val = token.toDouble(&ok);
            if (!ok) {
                qWarning() << "Invalid number in file:" << token;
                return Eigen::MatrixXd(); // Ошибка
            }
            values.append(val);
        }

        if (expectedCols == -1)
            expectedCols = values.size();
        else if (values.size() != expectedCols) {
            qWarning() << "Inconsistent row size:" << values.size() << ", expected:" << expectedCols;
            return Eigen::MatrixXd(); // Ошибка
        }

        rows.append(values);
    }

    Eigen::MatrixXd result(rows.size(), expectedCols);
    for (int i = 0; i < rows.size(); ++i)
        for (int j = 0; j < expectedCols; ++j)
            result(i, j) = rows[i][j];

    return result;
}

}


#endif // UTILS_H
