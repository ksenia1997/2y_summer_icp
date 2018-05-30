/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief Representation of a line object in a scene.
 * @file line.h
 */

#ifndef LINE_H
#define LINE_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QToolTip>
#include <QPainter>
#include <QPen>

class Port;

/**
 * @brief The Line class
 */
class Line : public QGraphicsLineItem
{
public:
    Line(const QLineF &line, Port* endPort);
private:
    /**
     * @brief hoverMoveEvent Overridden method for handling hover events. Ensures that tooltip with a line value pops up.
     * @param event A mouse hover event.
     */
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
    /**
     * @brief paint Overriden method for line drawing.
     * @param painter A QPainter.
     * @param option Options.
     * @param parent A QWidget* parent.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent);

    Port* endPort;
};

#endif // LINE_H
