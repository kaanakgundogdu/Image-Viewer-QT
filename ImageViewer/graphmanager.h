#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QObject>
#include <QString>
#include "fileexplorer.h"

class QGraphicsScene;

class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QObject *parent = nullptr);

    QGraphicsScene* scene();
    QStringList supported_extensions();
    QStringList get_file_names();

    QString get_current_path();

    unsigned short int get_current_index();

    void open_image(const QString& full_path);
    void next_image();
    void prev_image();
    bool has_next_image();
    bool has_prev_image();

    void open_selected_item(QString path);


    void fit_in_view();
    bool is_file_supported(const QString& full_path);

signals:
    void image_changed(const QString& file_name);

private:
    void show_image(const QString& full_path);

private:
    FileExplorer file_exp;
    QGraphicsScene* image_scene=nullptr;
    const QStringList image_extensions= {"*.png","*.bmp","*.jpg","*.jpeg" };

};

#endif // GRAPHMANAGER_H










