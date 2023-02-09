#include "widget.h"
#include "./ui_widget.h"
#include "graphmanager.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMenuBar>

Widget::Widget(GraphManager& gm,QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::Widget)
    , graph_manager(gm)
{
    ui->setupUi(this);

    setWindowTitle(default_window_title);
    ui->graph_view->setScene(graph_manager.scene());
    //ui->graph_view->hide();
    connect_buttons();
    create_menu_bar();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::connect_buttons()
{
    connect(ui->next_button,&QPushButton::clicked,
            this,&Widget::on_next_button_clicked);
    connect(ui->prev_button,&QPushButton::clicked,
            this,&Widget::on_prev_button_clicked);
    connect(&graph_manager,&GraphManager::image_changed,
            this,&Widget::on_image_changed);
}

void Widget::create_menu_bar()
{
    QMenuBar* menu_bar = new QMenuBar(this);
    QMenu *file_menu = new QMenu("File");
    QMenu *edit_menu = new QMenu("Edit");
    QMenu *help_menu = new QMenu("Help");
    menu_bar->addMenu(file_menu);
    menu_bar->addMenu(edit_menu);
    menu_bar->addMenu(help_menu);
    auto *open = new QAction("&Open", this);
    auto *exit = new QAction("&Exit", this);
    file_menu->addAction(open);
    file_menu->addAction(exit);

    connect(open, &QAction::triggered,this,&Widget::show_open_dialog);
    connect(exit, &QAction::triggered,this,QApplication::quit);

    update_buttons();
    this->layout()->setMenuBar(menu_bar);
}


void Widget::open_image(const QString full_path)
{
    if(graph_manager.is_file_supported(full_path)){
        graph_manager.open_image(full_path);
        fit_in_view();
    }
    update_buttons();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        prev_image();
        break;
    case Qt::Key_Right:
        next_image();
        break;
    case Qt::Key_O:
        if(event->modifiers() & Qt::ControlModifier)
            show_open_dialog();
        break;
    default:
        break;
    }
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        zoom_in();
    else
        zoom_out();

}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        fit_in_view();
    QWidget::mouseDoubleClickEvent(event);
}

void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* mime_data=event->mimeData();

    if(mime_data->hasUrls()){
        QList<QUrl> urls=mime_data->urls();
        if(graph_manager.is_file_supported(urls[0].toLocalFile())){
            event->acceptProposedAction();
        }
    }
}

void Widget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls=event->mimeData()->urls();
    if(urls.size()){
        open_image(urls[0].toLocalFile());
    }
}

void Widget::on_next_button_clicked()
{
    next_image();
}

void Widget::on_prev_button_clicked()
{
    prev_image();
}

void Widget::on_image_changed(const QString &file_name)
{
    setWindowTitle(QString("%0 - \"%1\"").arg(default_window_title).arg(file_name));
}

void Widget::prev_image()
{
    if (graph_manager.has_prev_image()){
        graph_manager.prev_image();
        fit_in_view();
    }
    update_buttons();
}

void Widget::next_image()
{
    if (graph_manager.has_next_image()){
        graph_manager.next_image();
        fit_in_view();
    }
    update_buttons();
}

void Widget::fit_in_view()
{
    graph_manager.fit_in_view();
}

void Widget::zoom_in()
{
    if(zoom_counter>8)
        return;
    zoom_counter++;
    ui->graph_view->scale(1.1,1.1);
}

void Widget::zoom_out()
{
    if (zoom_counter<-5)
        return;
    zoom_counter--;
    ui->graph_view->scale(0.9,0.9);
}

void Widget::show_open_dialog()
{
    QString image_extensions= QString("Images(%0)").arg(graph_manager.supported_extensions().join(" "));
    QString path=QFileDialog::getOpenFileName(this,"Open Image","",image_extensions);
    if(path.size())
        open_image(path);
}

void Widget::update_buttons()
{
    ui->next_button->setEnabled(graph_manager.has_next_image());
    ui->prev_button->setEnabled(graph_manager.has_prev_image());
}


