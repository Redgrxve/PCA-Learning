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

Eigen::MatrixXd readCsvByFeatures(const QString& filePath, const QStringList& selectedColumnNames) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Не удалось открыть файл: " + filePath.toStdString());
    }

    QTextStream in(&file);

    // Шаг 1: читаем заголовки
    if (in.atEnd()) throw std::runtime_error("Файл пустой.");
    QString headerLine = in.readLine().trimmed();
    QStringList headers = headerLine.split(',');

    // Шаг 2: сопоставляем имена с индексами
    QMap<QString, int> nameToIndex;
    for (int i = 0; i < headers.size(); ++i)
        nameToIndex[headers[i].trimmed()] = i;

    std::vector<int> selectedIndices;
    for (const QString& colName : selectedColumnNames) {
        if (!nameToIndex.contains(colName.trimmed()))
            throw std::runtime_error("Колонка \"" + colName.toStdString() + "\" не найдена в заголовке.");
        selectedIndices.push_back(nameToIndex[colName.trimmed()]);
    }

    // Шаг 3: читаем данные
    std::vector<std::vector<double>> rows;

    int readed{};
    while (!in.atEnd() && readed < 100) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList tokens = line.split(',');

        std::vector<double> row;
        for (int colIndex : selectedIndices) {
            if (colIndex < tokens.size()) {
                row.push_back(tokens[colIndex].toDouble());
            } else {
                throw std::runtime_error("Недостаточно значений в строке CSV.");
            }
        }
        rows.push_back(std::move(row));
        ++readed;
    }

    // Шаг 4: преобразуем в Eigen::MatrixXd
    int numRows = rows.size();
    int numCols = selectedIndices.size();
    Eigen::MatrixXd result(numRows, numCols);

    for (int i = 0; i < numRows; ++i)
        for (int j = 0; j < numCols; ++j)
            result(i, j) = rows[i][j];

    return result;
}

QStringList getCSVHeaders(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Не удалось открыть файл: " + filePath.toStdString());
    }

    QTextStream in(&file);
    return in.readLine().trimmed().split(',');
}

}


#endif // UTILS_H
