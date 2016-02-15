#pragma once

#include "ui_pindialog.h"

#include <QDialog>

class PinDialog
    : public QDialog
{
    Q_OBJECT

public:
    explicit PinDialog(QWidget*, const QString& title, const QString& pin);
    void initializeComponent();

private:
    QScopedPointer<Ui::PinDialog> ui;
};
