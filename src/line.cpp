/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief Implementatin of class Line.
 * @file line.cpp
 */

#include "line.h"
#include "port.h"

Line::Line(const QLineF &line, Port* endPort) : QGraphicsLineItem(line)
{
    this->endPort = endPort;
    setAcceptHoverEvents(true);
    setZValue(-1);
}

void Line::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event)

    if (endPort->areDataSet())
        setToolTip(QString::number(endPort->getData()));
    else
        setToolTip("No value");
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent){
    Q_UNUSED(parent);
    Q_UNUSED(option);

    painter->setPen(QPen(QBrush(Qt::black), 3));
    painter->drawLine(line());
}
