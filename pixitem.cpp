#include "pixitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>

PixItem::PixItem(QObject *parent)
    : QObject{parent}
{
    qDebug()<<"PixItem()";
}

PixItem::~PixItem()
{
    qDebug() << "~PixItem()";
}

void PixItem::setPixmap(QPixmap pix)
{
    m_pix = pix;
    update();
}

QRectF PixItem::boundingRect() const
{
    return QRectF(-m_pix.width() / 2, -m_pix.height() / 2, m_pix.width(), m_pix.height());
}

void PixItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawPixmap(boundingRect().topLeft(), m_pix);
}

void PixItem::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->text();
}

void PixItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->pos();
    qDebug() << event->scenePos();
    qDebug() << event->screenPos();
    QGraphicsPixmapItem::mouseDoubleClickEvent(event);
}
