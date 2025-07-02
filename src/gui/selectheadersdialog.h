#ifndef SELECTHEADERSDIALOG_H
#define SELECTHEADERSDIALOG_H

#include <QDialog>

namespace Ui {
class SelectHeadersDialog;
}

class SelectHeadersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectHeadersDialog(QWidget *parent = nullptr);
    ~SelectHeadersDialog();

    void addHeaders(const QStringList &headers);
    QStringList getSelectedHeaders();
    QString getTarget();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::SelectHeadersDialog *ui;

    void deleteSelectedItems();

signals:
    void headersSelected(const QStringList &items);

private slots:
    void onAccepted();
    void onHeaderSelected(int index);
};

#endif // SELECTHEADERSDIALOG_H
