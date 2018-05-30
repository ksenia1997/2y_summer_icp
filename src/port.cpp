/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief A representation of a port.
 * @file port.cpp
 *
 *
 */

#include "port.h"
#include "block.h"

Port::Port(Block* parentBlock, portType type, int numberOfPorts, int position)
{
    this->parent = parentBlock;
    this->pType = type;
    dataSet = false;
    selected = false;
    numberOfPort = position;
    setFlag(ItemSendsScenePositionChanges);

    // Calculation of position inside the block
    QRectF rect = parentBlock->boundingRect();
    if (type == InPort)
        relPos = rect.topLeft();
    else
        relPos = rect.topRight();
    int offset = (position+1) * (rect.height() / (numberOfPorts+1));

    relPos.setY(relPos.y() + offset);
    relPos.setY(relPos.y() - boundingRect().height()/2);
    relPos.setX(relPos.x() - boundingRect().width()/2);

    // Calculation of position on the scene
    QPointF p = parentBlock->pos();
    p.setX(p.x() + relPos.x());
    p.setY(p.y() + relPos.y());
    setPos(p);
}

int Port::type() const
{
    return Type;
}

int Port::numberOfPortRec(){
    return numberOfPort;
}

void Port::selectPort()
{
    selected = true;
}

void Port::unselectPort()
{
    selected = false;
}

bool Port::isSelected()
{
    return selected;
}

Port::portType Port::getPortType()
{
    return pType;
}

Block *Port::parentBlock()
{
    return parent;
}

QPointF Port::getRelPos()
{
    return relPos;
}

QRectF Port::boundingRect() const
{
    return QRectF(0, 0, 14, 14);
}

QVariant Port::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        moveLinesToCenter(newPos);
    }
    return QGraphicsItem::itemChange(change, value);
}

void Port::moveLinesToCenter(QPointF newPos) {
    // Convert the port position (top-left) to its center position
    int xOffset = boundingRect().x() + boundingRect().width()/2;
    int yOffset = boundingRect().y() + boundingRect().height()/2;

    QPointF newCenterPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);

    // Move the required point of the line to the center of the port
    foreach(BlockConnection* connection, conList) {
        QPointF p1;
        QPointF p2;
        if (connection->isFirstPoint()) {
            p1 = newCenterPos;
            p2 = connection->getLine()->line().p2();
        } else {
            p1 = connection->getLine()->line().p1();
            p2 = newCenterPos;
        }
        connection->getLine()->setLine(QLineF(p1, p2));
        connection->getLine()->update();
    }
}

void Port::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(option);

    QRectF rectangle = boundingRect();

    // Backup old brush
    const QBrush backup = painter->brush();

    // Set painter
    if (selected) {
        painter->setBrush(Qt::red);
        QPen pen(Qt::darkRed);
        pen.setWidth(4);
        painter->setPen(pen);
    }
    else {
        painter->setBrush(Qt::darkGray);
    }

    // Draw the port
    painter->drawEllipse(rectangle);

    // Restore the old brush
    painter->setBrush(backup);
}

void Port::addConnection(Line* line, bool isFirstPoint, Port* firstPort, Port* secondPort) {
    BlockConnection* newConnect = new BlockConnection(line, isFirstPoint, firstPort, secondPort);
    qDebug() << "Creating a new connection for ports:" << firstPort << secondPort;
    conList.append(newConnect);
}

bool Port::existsConnection(Port *port) {
    foreach(BlockConnection* connection, conList) {
        if ((connection->getFirstPort() == port && connection->getSecondPort() == this) ||
            (connection->getFirstPort() == this && connection->getSecondPort() == port)) {
            return true;
        }
    }
    return false;
}

QList<Block*> Port::getNextBlocks()
{
    QList<Block*> newList;
    foreach (Port* nextPort, getNextPorts()) {
        newList.append(nextPort->parentBlock());
    }
    return newList;
}

QList<Port*> Port::getNextPorts()
{
    QList<Port*> newList;
    foreach (BlockConnection* connection, conList) {
        newList.append(connection->getSecondPort());
    }
    return newList;
}

bool Port::isConnected()
{
    if (conList.size() == 0) return false;
    return true;
}

bool Port::isInput()
{
    if (pType == InPort) return true;
    return false;
}

bool Port::isOutput()
{
    if (pType == OutPort) return true;
    return false;
}

void Port::removeConnections(Port *port) {
    // Remove all connections to a certain port
    foreach(BlockConnection* connection, conList) {
        if (connection->getFirstPort() == port || connection->getSecondPort() == port) {
            conList.removeOne(connection);
        }
    }
}

void Port::removeConnections(QList<QGraphicsLineItem*>* toDelete) {
    // Mark all connections from this port for removal
    foreach(BlockConnection* connection, conList) {
        toDelete->append(connection->getLine());
    }
}

void Port::removeConnection(QGraphicsLineItem* line) {
    // Remove a single connection from the port
    foreach(BlockConnection* connection, conList) {
        if (connection->getLine() == line) {
            conList.removeOne(connection);
        }
    }
}

double Port::getData()
{
    if (!dataSet) return 0;
    return inputData;
}

void Port::setData(double data)
{
    inputData = data;
    dataSet = true;
}

bool Port::areDataSet()
{
    return dataSet;
}

void Port::unsetData()
{
    dataSet = false;
}
