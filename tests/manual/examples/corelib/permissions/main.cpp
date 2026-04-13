// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUICore/qmetaobject.h>
#include <BobUIWidgets/qapplication.h>
#include <BobUIWidgets/qwidget.h>
#include <BobUIWidgets/qpushbutton.h>
#include <BobUIWidgets/qlayout.h>
#include <BobUIWidgets/qmessagebox.h>

#if !BOBUI_CONFIG(permissions)
#error "This example requires the permissions feature, which is not enabled on this platform"
#endif

#include <BobUICore/qpermissions.h>

class PermissionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PermissionWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        static const QPermission permissions[] = {
            QCameraPermission{},
            QMicrophonePermission{},
            QBluetoothPermission{},
            QContactsPermission{},
            QCalendarPermission{},
            QLocationPermission{}
        };

        for (auto permission : permissions) {
            auto permissionName = QString::fromLatin1(permission.type().name());
            QPushButton *button = new QPushButton(permissionName.sliced(1, permissionName.length() - 11));
            connect(button, &QPushButton::clicked, this, &PermissionWidget::buttonClicked);
            button->setProperty("permission", QVariant::fromValue(permission));
            layout->addWidget(button);
        }

        QPalette pal = palette();
        pal.setBrush(QPalette::Window, QGradient(QGradient::HappyAcid));
        setPalette(pal);
    }

private:
    void buttonClicked()
    {
        auto *button = static_cast<QPushButton*>(sender());

        auto permission = button->property("permission").value<QPermission>();
        Q_ASSERT(permission.type().isValid());

        switch (qApp->checkPermission(permission)) {
        case BobUI::PermissionStatus::Undetermined:
                qApp->requestPermission(permission, this,
                    [button](const QPermission &permission) {
                        Q_UNUSED(permission);
                        emit button->clicked(); // Try again
                    }
                );
            return;
        case BobUI::PermissionStatus::Denied:
            QMessageBox::warning(this, button->text(),
                tr("Permission is needed to use %1. Please grant permission "\
                "to this application in the system settings.").arg(button->text()));
            return;
        case BobUI::PermissionStatus::Granted:
            break; // Proceed
        }

        // All good, can use the feature
        QMessageBox::information(this, button->text(),
            tr("Accessing %1").arg(button->text()));
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    PermissionWidget widget;
    widget.show();
    return app.exec();
}

#include "main.moc"
