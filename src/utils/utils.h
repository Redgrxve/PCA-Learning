#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QStringList>
#include <QDebug>
#include <QColor>
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

std::pair<Eigen::MatrixXd, Eigen::VectorXd> readCsvByFeatures(
    const QString& filePath,
    const QStringList& selectedColumnNames,
    const QString& targetFeature,
    int readLimit = 100000)
{
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
        QString trimmedName = colName.trimmed();
        if (!nameToIndex.contains(trimmedName))
            throw std::runtime_error("Колонка \"" + trimmedName.toStdString() + "\" не найдена в заголовке.");
        selectedIndices.push_back(nameToIndex[trimmedName]);
    }

    int targetIndex = -1;
    QString trimmedTarget = targetFeature.trimmed();
    if (!trimmedTarget.isEmpty()) {
        if (!nameToIndex.contains(trimmedTarget))
            throw std::runtime_error("Целевая колонка \"" + trimmedTarget.toStdString() + "\" не найдена в заголовке.");
        targetIndex = nameToIndex[trimmedTarget];
    }

    // Шаг 3: читаем данные
    std::vector<std::vector<double>> featureRows;
    std::vector<double> targetValues;

    int readed = 0;
    while (!in.atEnd() && readed < readLimit) {
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

        if (targetIndex != -1) {
            if (targetIndex < tokens.size()) {
                targetValues.push_back(tokens[targetIndex].toDouble());
            } else {
                throw std::runtime_error("Не удалось прочитать целевое значение.");
            }
        }

        featureRows.push_back(std::move(row));
        ++readed;
    }

    int numRows = featureRows.size();
    int numCols = selectedIndices.size();
    Eigen::MatrixXd X(numRows, numCols);
    Eigen::VectorXd y;

    for (int i = 0; i < numRows; ++i)
        for (int j = 0; j < numCols; ++j)
            X(i, j) = featureRows[i][j];

    if (targetIndex != -1) {
        y.resize(numRows);
        for (int i = 0; i < numRows; ++i)
            y(i) = targetValues[i];
    }

    return {X, y};
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
