#include "fileexplorer.h"
#include <QFileInfo>
#include <QDir>

FileExplorer::FileExplorer()
{

}

void FileExplorer::set_extensions(const QStringList &extensions)
{
    supported_file_extensions=extensions;
}

void FileExplorer::open_file(const QString &file_path)
{
    QFileInfo current(file_path);
    QDir dir=current.absoluteDir();
    current_file_path=file_path;
    current_file_name=current.fileName();

    file_names_in_current_path=dir.entryList(supported_file_extensions,QDir::Files,QDir::Name);
    int index=file_names_in_current_path.indexOf(current_file_name);
    has_prev=index!=0 ? true:false;
    has_next=index!=supported_file_extensions.size()-1 ? true:false;

}

QStringList FileExplorer::get_file_names()
{
    return file_names_in_current_path;
}

QString FileExplorer::next_file()
{
    QFileInfo current(current_file_path);
    QDir dir = current.absoluteDir();

    int index=file_names_in_current_path.indexOf(current_file_name);

    if (index<file_names_in_current_path.size()-1) {
        has_prev=true;
        if (index==file_names_in_current_path.size()-2)
            has_next=false;
        current_file_path=dir.absoluteFilePath(file_names_in_current_path.at(index+1));
        current=QFileInfo(current_file_path);
        current_file_name=current.fileName();
    }

    return current_file_path;
}

QString FileExplorer::prev_file()
{
    QFileInfo current(current_file_path);
    QDir dir = current.absoluteDir();

    int index=file_names_in_current_path.indexOf(current_file_name);

    if (index>0) {
        has_next=true;
        if (index==1)
            has_prev=false;
        current_file_path=dir.absoluteFilePath(file_names_in_current_path.at(index-1));
        current=QFileInfo(current_file_path);
        current_file_name=current.fileName();
    }
    return current_file_path;
}

bool FileExplorer::has_next_file()
{
    return has_next;
}

bool FileExplorer::has_prev_file()
{
    return has_prev;
}

QString FileExplorer::get_current_file_name()
{
    return current_file_name;
}
