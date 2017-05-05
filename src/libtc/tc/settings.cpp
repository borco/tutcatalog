#include "settings.h"
#include "config.h"

#include <QDebug>
#include <QSettings>
#include <QUrl>

namespace tc {

class SettingsPrivate
{
    Q_DECLARE_PUBLIC(Settings)
    Settings* q_ptr { nullptr };
    QScopedPointer<QSettings> m_settings { nullptr };

    static QString m_fileName;

    explicit SettingsPrivate(Settings* ptr)
        : q_ptr (ptr)
        , m_settings (!m_fileName.isEmpty()
                      ? new QSettings(m_fileName, QSettings::IniFormat)
                      : new QSettings(QSettings::IniFormat, QSettings::UserScope, MY_COMPANY, MY_APPLICATION))
    {
    }
};

QString SettingsPrivate::m_fileName;

Settings::Settings() : d_ptr(new SettingsPrivate(this))
{
}

Settings::~Settings()
{
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    Q_D(Settings);
    d->m_settings->setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    Q_D(const Settings);
    return d->m_settings->value(key, defaultValue);
}

int Settings::beginReadArray(const QString &prefix)
{
    Q_D(Settings);
    d->m_settings->beginReadArray(prefix);
}

void Settings::beginWriteArray(const QString &prefix, int size)
{
    Q_D(Settings);
    d->m_settings->beginWriteArray(prefix, size);
}

void Settings::endArray()
{
    Q_D(Settings);
    d->m_settings->endArray();
}

void Settings::setArrayIndex(int i)
{
    Q_D(Settings);
    d->m_settings->setArrayIndex(i);
}

void Settings::sync()
{
    Q_D(Settings);
    d->m_settings->sync();
}

void Settings::setFileName(const QString &fileName)
{
    qDebug() << "setting settings file name to:" << fileName;

    SettingsPrivate::m_fileName = fileName.startsWith("file://")
            ? QUrl(fileName).toLocalFile()
            : fileName
              ;
}

QString Settings::fileName() const
{
    Q_D(const Settings);
    return d->m_settings->fileName();
}

}
