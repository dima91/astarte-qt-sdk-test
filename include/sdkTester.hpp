
#pragma once

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtCore/QRandomGenerator>

#include <AstarteDeviceSDK.h>


class SdkTester : public QObject {
    Q_OBJECT

public:
    SdkTester (QSettings &settings);
    ~SdkTester () = default;

private slots:
    void check_init_result (Hemera::Operation *operation);
    void send_values ();

    //void handleIncomingData(const QByteArray &interface, const QByteArray &path, const QVariant &value);

private:
    AstarteDeviceSDK *m_astarte_sdk;
    QSettings &m_app_settings;
    QTimer *m_publish_timer;
    QRandomGenerator m_random_generator;
    int m_counter;

};