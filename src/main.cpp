
#include <iostream>
#include <fstream>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QSettings>
#include <QtCore/QDir>

#include <sdkTester.hpp>


int main (int argn, char **argv) {
    
    QCoreApplication app (argn, argv);
    app.setApplicationName(QObject::tr("Astarte Qt SDK Test"));
    
    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("Astarte Qt SDK test application"));
    parser.addHelpOption();
    parser.addOption({
        QStringList{QStringLiteral("f"), QStringLiteral("settings-file")},
        QObject::tr("Path to INI file that contains settings to connect to Astarte"),
        QStringLiteral("path"),
        QString("settings.ini")
    });

    parser.process(app);

    QString settings_file_path  = parser.value(QString("settings-file"));
    std::ifstream settings_file (settings_file_path.toStdString());
    if (!settings_file) {
        throw std::runtime_error (QString("File at path   " + settings_file_path + "   does not exists").toStdString());
    }
    
    QSettings app_settings (settings_file_path, QSettings::IniFormat, nullptr);

    // Checking persistence directory existance
    QString persistence_directory_path  = app_settings.value("AstarteTransport/persistencyDir").toString();
    QDir directory (persistence_directory_path);
    if (!directory.exists()) {
        qInfo() << "Creating persistency directory : " << directory.dirName() << " at " << persistence_directory_path;
        directory.mkpath(".");
    }

    SdkTester tester (app_settings);
    return app.exec();
}