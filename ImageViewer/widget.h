#ifndef WIDGET_H
#define WIDGET_H

//#include <QMainWindow>
#include <QWidget>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class GraphManager;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(GraphManager& gm,QWidget *parent = nullptr);
    ~Widget();

    void open_image(const QString full_path);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void on_next_button_clicked();
    void on_prev_button_clicked();
    void on_image_changed(const QString& file_name);

    void prev_image();
    void next_image();
    void fit_in_view();
    void zoom_in();
    void zoom_out();
    void show_open_dialog();
    void update_buttons();
    void make_view_background_black();

    void connect_buttons();
    void create_menu_bar();
    void open_default_view();
    void open_image_view();

    void fill_list_view();
    void set_list_index();
    void selected_list_item(QString selected_item);
    void list_item_clicked(QListWidgetItem* item);
    void open_cliced_item(const QString item_path);


private:
    Ui::Widget *ui;
    GraphManager& graph_manager;
    const QString default_window_title="ImageViewer";
    int zoom_counter=0;
};
#endif // WIDGET_H
