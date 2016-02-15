#include "pindialog.h"

PinDialog::PinDialog(QWidget* parent, const QString& title, const QString& pin)
    : QDialog(parent)
{
    ui.reset(new Ui::PinDialog);

    ui->setupUi(this);
    ui->label->setText(title);
    ui->pin->setText(pin);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &PinDialog::accepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &PinDialog::rejected);
}

void PinDialog::initializeComponent() {
    ui->buttonBox->addButton(QDialogButtonBox::Cancel);
}
