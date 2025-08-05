#include "mainwindow.h"
#include "model.h"
#include "utils.h"
//#include "log.h"
#include "selectheadersdialog.h"

#include <QFileDialog>
#include <QMessageBox>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_dataModel = new Model;

    connect(ui->importAction, &QAction::triggered,
            this, &MainWindow::onImportTriggered);
}

MainWindow::~MainWindow()
{
    delete m_dataModel;
    delete ui;
}

Eigen::MatrixXd MainWindow::generateData()
{
    Eigen::MatrixXd a(10, 2);
    a << 2.5, 2.4,
        0.5, 0.7,
        2.2, 2.9,
        1.9, 2.2,
        3.1, 3.0,
        2.3, 2.7,
        2.0, 1.6,
        1.0, 1.1,
        1.5, 1.6,
        1.1, 0.9;

    return a;
}

void MainWindow::onImportTriggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Matrix File", "../../datasets");
    if (filePath.isEmpty()) return;

    QStringList featureNames;
    QString targetName;
    const auto headers = Utils::getCSVHeaders(filePath);

    SelectHeadersDialog dialog(this);
    dialog.addHeaders(headers);

    if (dialog.exec() == QDialog::Accepted) {
        featureNames = dialog.getSelectedHeaders();
        targetName = dialog.getTarget();
    };

    const auto [X, y] = Utils::readCsvByFeatures(filePath, featureNames, targetName);
    if (X.size() == 0 || y.size() == 0) {
        QMessageBox::warning(this, "Error", "Failed to load matrix from file.");
        return;
    }

    m_dataModel->setData(X, y);
    m_dataModel->setFeaturesNames(featureNames);
    m_dataModel->setTargetName(targetName);

    ui->chartWidget->setModel(m_dataModel);

    ui->statusbar->showMessage(filePath, 100000);

    QFile file("./log.txt");
    if (!file.open(QFile::Append)) {
        qWarning() << "Failed to open log file!";
    }

    QTextStream out(&file);
    out << "\n" + filePath + "\n";

    file.close();
}

