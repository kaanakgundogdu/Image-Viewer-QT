#include "graphmanager.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QImageReader>

GraphManager::GraphManager(QObject *parent)
    : QObject{parent}
{
    image_scene = new QGraphicsScene(this);
    file_exp.set_extensions(image_extensions);
}

QGraphicsScene *GraphManager::scene()
{
    return image_scene;
}

QStringList GraphManager::supported_extensions()
{
    return image_extensions;
}

void GraphManager::open_image(const QString &full_path)
{
    file_exp.open_file(full_path);
    show_image(full_path);
}

void GraphManager::next_image()
{
    show_image(file_exp.next_file());
}

void GraphManager::prev_image()
{
    show_image(file_exp.prev_file());
}

bool GraphManager::has_next_image()
{
    return file_exp.has_next_file();
}

bool GraphManager::has_prev_image()
{
    return file_exp.has_prev_file();
}

void GraphManager::fit_in_view()
{
    if (image_scene->items().size() && image_scene->views().size()) {
        auto view=image_scene->views().at(0);
        view->fitInView(image_scene->items().at(0),Qt::KeepAspectRatio);
        view->setSceneRect(image_scene->items().at(0)->boundingRect());
    }
}

bool GraphManager::is_file_supported(const QString &full_path)
{
    for (auto extension : image_extensions) {
        extension=extension.remove(0,1);

        if (full_path.endsWith(extension,Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void GraphManager::show_image(const QString &full_path)
{
    image_scene->clear();
    QImageReader image_reader(full_path);
    image_reader.setAutoTransform(true);
    QImage image=image_reader.read();

    if (!image.isNull()) {
        QPixmap pix_map=QPixmap::fromImage(image);

        image_scene->addPixmap(pix_map);
        image_scene->update();

        emit image_changed(file_exp.get_current_file_name());
    }

}











