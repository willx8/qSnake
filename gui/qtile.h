#ifndef QTILE_H
#define QTILE_H

#include <QLabel>

class Tile;

class QTile : public QLabel
{
    Q_OBJECT
public:
    //QTile(const QString & text);
    QTile();

    void draw_head();
    void draw_background();
    void draw_food();

private:

signals:

public slots:

};

#endif // QTILE_H
