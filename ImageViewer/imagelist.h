#ifndef IMAGELIST_H
#define IMAGELIST_H

//#include "graphmanager.h";
#include <QListView>

class ImageList
{
public:
    ImageList();
    void fill_list_view();

private:

    QListView listview;

};

#endif // IMAGELIST_H
