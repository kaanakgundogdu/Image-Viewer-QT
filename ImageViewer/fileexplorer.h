#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QString>
#include <QStringList>

class FileExplorer
{
public:
    FileExplorer();

    void set_extensions(const QStringList& extensions);
    void open_file(const QString& file_path);
    unsigned short int get_current_index();

    QStringList get_file_names();
    QString next_file();
    QString prev_file();
    QString selected_file(const QString selected_item_path);

    bool has_next_file();
    bool has_prev_file();

    QString get_current_file_name();
    QString get_current_file_path();

private:
    QStringList file_names_in_current_path;
    QString current_file_name;
    QString current_file_path;

    unsigned short int current_index;

    bool has_next=false;
    bool has_prev=false;

    QStringList supported_file_extensions;

};

#endif // FILEEXPLORER_H
