#ifndef PIXITEM_H
#define PIXITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

class PixItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit PixItem(QObject *parent = nullptr);
    ~PixItem();
    void setPixmap(QPixmap pix);

signals:

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;


protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;


private:
    QPixmap m_pix;
};

#endif // PIXITEM_H
