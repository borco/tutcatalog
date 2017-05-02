#include "folderinfo.h"

namespace tc {
namespace folders {

FolderInfo::FolderInfo(QObject *parent)
    : QObject(parent)
    , m_withCopies(false)
{
}

void FolderInfo::clear()
{
    *this = FolderInfo();
}

FolderInfo &FolderInfo::operator=(const FolderInfo &other)
{
    if (&other != this) {
        set_name(other.m_name);
        set_path(other.m_path);
        set_cachePath(other.m_cachePath);
        set_skipBackupPath(other.m_skipBackupPath);
        set_trashPath(other.m_trashPath);
        set_withCopies(other.m_withCopies);
    }
    return *this;
}

bool FolderInfo::operator==(const FolderInfo &other) const
{
    return m_name == other.m_name
            && m_path == other.m_path
            && m_cachePath == other.m_cachePath
            && m_skipBackupPath == other.m_skipBackupPath
            && m_trashPath == other.m_trashPath
            && m_withCopies == other.m_withCopies;
}

} // namespace folders
} // namespace tc
