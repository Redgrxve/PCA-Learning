#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Eigen/Dense>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Model;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Eigen::MatrixXd generateData();

private:
    Ui::MainWindow *ui;
    Model *m_dataModel{};

private slots:
    void onImportTriggered();
};
#endif // MAINWINDOW_H
