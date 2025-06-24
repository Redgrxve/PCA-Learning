#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Eigen/Dense>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class PCADataModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Eigen::MatrixXd generateData();

private:
    Ui::MainWindow *ui;
    PCADataModel *m_dataModel{};

private slots:
    void onImportTriggered();
};
#endif // MAINWINDOW_H
