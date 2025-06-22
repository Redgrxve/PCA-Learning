#include "mainwindow.h"
#include "pca.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->chartWidget->setData(generateData());
    ui->chartWidget->createInitialChart();
}

MainWindow::~MainWindow()
{
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

    //const auto c = PCA::meanSubtraction(a);
    return a;
}

