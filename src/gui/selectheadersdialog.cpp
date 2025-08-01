#include "selectheadersdialog.h"
#include <qevent.h>

#include "ui_selectheadersdialog.h"

SelectHeadersDialog::SelectHeadersDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectHeadersDialog)
{
    ui->setupUi(this);

    connect(ui->comboBox, &QComboBox::activated,
            this, &SelectHeadersDialog::onHeaderSelected);
    connect(ui->selectAllButton, &QPushButton::clicked,
            this, &SelectHeadersDialog::onSelectAll);
}

SelectHeadersDialog::~SelectHeadersDialog()
{
    delete ui;
}

void SelectHeadersDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        deleteSelectedItems();
    } else {
        QDialog::keyPressEvent(event);
    }
}

void SelectHeadersDialog::deleteSelectedItems()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();

    for (QListWidgetItem *item : std::as_const(selectedItems)) {
        int row = ui->listWidget->row(item);
        ui->comboBox->addItem(item->text());
        ui->targetComboBox->addItem(item->text());
        delete ui->listWidget->takeItem(row);
    }
}

void SelectHeadersDialog::addHeaders(const QStringList &headers)
{
    const auto size = headers.size();
    for (int i = 0; i < size; ++i) {
        const auto header = headers[i];
        ui->comboBox->addItem(header);
        ui->targetComboBox->addItem(header);
    }
}

QStringList SelectHeadersDialog::getSelectedHeaders()
{
    const auto size = ui->listWidget->count();
    QStringList headers;
    for (int i = 0; i < size; ++i) {
        const auto item = ui->listWidget->item(i);
        headers.append(item->text());
    }

    return headers;
}

QString SelectHeadersDialog::getTarget()
{
    return ui->targetComboBox->currentText();
}

void SelectHeadersDialog::onAccepted()
{
    const auto size = ui->listWidget->count();
    QStringList headers(size);
    for (int i = 0; i < size; ++i) {
        const auto item = ui->listWidget->item(i);
        headers.append(item->text());
    }

    emit headersSelected(headers);
}

void SelectHeadersDialog::onHeaderSelected(int index)
{
    ui->listWidget->addItem(ui->comboBox->itemText(index));
    ui->comboBox->removeItem(index);
    ui->targetComboBox->removeItem(index);
}

void SelectHeadersDialog::onSelectAll()
{
    const int count = ui->comboBox->count() - 1;
    if (count <= 0) return;

    for (int i = 0; i < count; ++i) {
        ui->listWidget->addItem(ui->comboBox->itemText(i));
    }

    ui->targetComboBox->setCurrentIndex(count);

    ui->comboBox->clear();
    //ui->targetComboBox->clear();
}
