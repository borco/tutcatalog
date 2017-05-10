#include "project.h"
#include "folders/folderinfo.h"

#include <QDir>
#include <QFile>
#include <QUrl>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/iterator.h>

namespace tc {

class ProjectPrivate
{
    Q_DECLARE_PUBLIC(Project)
    Project* const q_ptr { nullptr };

    QString getString(const YAML::Node& node,
                      const std::string& key,
                      const QString& defaultValue = QString()) const {
        return node[key]
                ? QString::fromStdString(node[key].as<std::string>())
                : defaultValue;
    }

    QString absolutePath(const QString& path) const {
        return path.isEmpty() ? path : QDir(path).absolutePath();
    }

    folders::FolderInfo* getFolderInfo(const YAML::Node& node) const {
        auto info = new folders::FolderInfo;
        info->set_name(getString(node, "name"));
        info->set_withCopies(getString(node, "with copies", "no").compare("yes", Qt::CaseInsensitive) == 0);

        info->set_path(absolutePath(getString(node, "path")));
        info->set_cachePath(absolutePath(getString(node, "cache")));
        info->set_noBackupPath(absolutePath(getString(node, "no backup")));
        info->set_trashPath(absolutePath(getString(node, "trash")));

        return info;
    }

    void readTutorialFolders(const YAML::Node& node) {
        Q_Q(Project);
        qDeleteAll(q->m_folderInfos);
        q->m_folderInfos.clear();
        for(auto n: node) {
            q->m_folderInfos.append(getFolderInfo(n));
        }
    }

    void readVideoExtensions(const YAML::Node& node) {
        Q_Q(Project);
        QStringList videoExtensions;
        for(auto n: node) {
            videoExtensions.append(QString::fromStdString(n.as<std::string>()));
        }
        q->set_videoExtensions(videoExtensions);
    }

    void readStringProperty(const YAML::Node& node, bool (Project::*setter)(QString)) {
        Q_Q(Project);
        (q->*setter)(node ? QString::fromStdString(node.as<std::string>()) : "");
    }

public:
    ProjectPrivate(Project* ptr) : q_ptr(ptr) {}

    bool readFromFile(const QString& fileName) {
        QString fn = fileName;
        if (!QFile::exists(fn))
            fn = QUrl(fileName).toLocalFile();
        if (!QFile::exists(fn))
            return false;

        QFile f(fn);
        if (!f.open(QIODevice::ReadOnly))
            return false;

        return readFromData(f.readAll());
    }

    bool readFromData(const QByteArray& data) {
        auto node = YAML::Load(data);
        readStringProperty(node["ffprobe"], Project::set_ffprobePath);
        readStringProperty(node["scrap script"], Project::set_scrapScriptPath);
        readTutorialFolders(node["tutorial folders"]);
        readVideoExtensions(node["video extensions"]);
        return true;
    }
};

Project::Project(QObject *parent)
    : QObject(parent)
    , d_ptr(new ProjectPrivate(this))
{
}

Project::~Project()
{
}

bool Project::readFromFile(const QString &fileName)
{
    Q_D(Project);
    return d->readFromFile(fileName);
}

bool Project::readFromData(const QByteArray &data)
{
    Q_D(Project);
    return d->readFromData(data);
}

} // namespace tc
