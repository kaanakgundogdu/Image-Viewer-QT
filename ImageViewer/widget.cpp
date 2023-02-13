#include "widget.h"
#include "./ui_widget.h"
#include "graphmanager.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMenuBar>
#include <QSplitter>
#include <QTimer>

Widget::Widget(GraphManager& gm,QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::Widget)
    , graph_manager(gm)
{
    ui->setupUi(this);

    //ui->layoutWidget->setStyleSheet("background-color: red");

    setWindowTitle(default_window_title);

    ui->graph_view->setScene(graph_manager.scene());

    connect_buttons();
    create_menu_bar();
    open_default_view();
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
    connect(ui->open_image_button,&QPushButton::clicked,
            this,&Widget::show_open_dialog);

//    connect(ui->image_names_list_widget,&QListWidget::currentTextChanged,
//            this,&Widget::selected_list_item);

    connect(ui->image_names_list_widget,&QListWidget::itemClicked,
            this,&Widget::list_item_clicked);
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

    QMenuBar *bar = new QMenuBar(menu_bar);

    QAction *change_bg_action = new QAction("Change BG Color", bar);
    bar->addAction(change_bg_action);

    menu_bar->setCornerWidget(bar);

    QAction *open = new QAction("&Open", this);
    QAction *exit = new QAction("&Exit", this);
    QAction *makebgblack = new QAction("&Change BG Color", this);

    file_menu->addAction(open);
    file_menu->addAction(exit);
    edit_menu->addAction(makebgblack);

    connect(open, &QAction::triggered,this,&Widget::show_open_dialog);
    connect(exit, &QAction::triggered,this,QApplication::quit);
    connect(makebgblack, &QAction::triggered,this,&Widget::make_view_background_black);
    connect(change_bg_action, &QAction::triggered,this,&Widget::make_view_background_black);

    update_buttons();
    this->layout()->setMenuBar(menu_bar);

}

void Widget::open_default_view()
{
    ui->graph_view->hide();
    ui->next_button->hide();
    ui->prev_button->hide();
    ui->open_image_button->show();
    ui->image_names_list_widget->hide();
}

void Widget::open_image_view()
{
    ui->graph_view->show();
    ui->next_button->show();
    ui->prev_button->show();
    ui->open_image_button->hide();
}

void Widget::open_image(const QString full_path)
{
    if(ui->graph_view->isHidden())
        open_image_view();

    if(graph_manager.is_file_supported(full_path)){
        graph_manager.open_image(full_path);
        fill_list_view();
        QTimer::singleShot(50, [this]{fit_in_view();});
    }
    zoom_counter=0;

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

void Widget::resizeEvent(QResizeEvent *event)
{
    if(ui->graph_view->isHidden())
        return;
    fit_in_view();
    zoom_counter=0;
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
    zoom_counter=0;
    //setWindowTitle(QString("%0 - \"%1\"").arg(default_window_title).arg(file_name));
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
    ui->graph_view->scale(1.10000,1.10000);
}

void Widget::zoom_out()
{
    if (zoom_counter<-6)
        return;
    zoom_counter--;
    ui->graph_view->scale(0.90000,0.9000);
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
    set_list_index();
}

void Widget::fill_list_view()
{
    QStringList ls= graph_manager.get_file_names();
    ui->image_names_list_widget->show();
    for (int i = 0; i < ls.size(); ++i)
        ui->image_names_list_widget->addItem(ls[i]);
}

void Widget::set_list_index()
{
    auto current_index=graph_manager.get_current_index();
    ui->image_names_list_widget->setCurrentItem(ui->image_names_list_widget->item(current_index));
}

void Widget::selected_list_item(QString selected_item)
{
    qInfo()<<selected_item;
}

void Widget::list_item_clicked(QListWidgetItem *item)
{
    QString cp = graph_manager.get_current_path();
    int i=0;

    for (i = cp.size()-1; i>0; --i) {
        if(cp.at(i) == '/'){
            break;
        }
    }

    if(i+1<cp.size()-1){
        cp.remove(i+1,cp.size()-1);
        cp= cp+item->text();
        open_cliced_item(cp);
    }
}

void Widget::open_cliced_item(const QString item_path)
{
    graph_manager.open_selected_item(item_path);
    fit_in_view();
    update_buttons();
}

void Widget::make_view_background_black()
{
    if(ui->graph_view->backgroundBrush()==Qt::black)
        ui->graph_view->setBackgroundBrush(Qt::white);
    else
        ui->graph_view->setBackgroundBrush(Qt::black);
}













