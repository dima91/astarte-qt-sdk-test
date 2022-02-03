
#include <sdkTester.hpp>

#include <chrono>


SdkTester::SdkTester (QSettings &app_settings) : m_astarte_sdk (nullptr),
                                                    m_app_settings (app_settings),
                                                    m_publish_timer(new QTimer(this)),
                                                    m_counter (0) {
    // Setting up "m_publish_timer"
    if (m_app_settings.value ("DeviceSettings/publishIntervalMs").toInt() <= 0)
        throw std::runtime_error ("You must insert an interval greater than 0");

    m_publish_timer->setInterval(m_app_settings.value ("DeviceSettings/publishIntervalMs").toInt());
    connect(m_publish_timer, &QTimer::timeout, this, &SdkTester::send_values);

    // Building astarte SDK
    m_astarte_sdk = new AstarteDeviceSDK(m_app_settings.fileName(),
                                            m_app_settings.value ("DeviceSettings/interfacesDirectory").toString(),
                                            m_app_settings.value ("DeviceSettings/deviceID").toByteArray(),
                                            this);
    connect(m_astarte_sdk->init(), &Hemera::Operation::finished, this, &SdkTester::check_init_result);
    //connect(m_astarte_sdk, &AstarteDeviceSDK::dataReceived, this, &PeopleCounter::handleIncomingData);
}




void SdkTester::check_init_result (Hemera::Operation *operation) {
    if (operation->isError()) {
        QString error_string    = "Catched initializing error: " + operation->errorName() + "\n\t" +
                                    operation->errorMessage();
        qCritical() << error_string;
        throw std::runtime_error (error_string.toStdString());
    } else {
        m_publish_timer->start();
    }
}




void SdkTester::send_values () {
    QVariantHash payload;

    const auto current_time = std::chrono::system_clock::now();

    // Building data to be sent to Astarte
    payload["/counter"]     = m_counter++;
    payload["/random"]      = (int) m_random_generator.generate();

    // Actually sending data to Astarte
    bool sent_result    = m_astarte_sdk->sendData (m_app_settings.value("DeviceSettings/testerInterfaceName").toByteArray(),
                                                    payload, QDateTime::currentDateTime());
    if (!sent_result) {
        qWarning() << "Send operation reports an error!";
    }
}