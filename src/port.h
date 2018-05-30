/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief An abstract class of a port.
 * @file port.h
 *
 *
 */

#ifndef PORT_H
#define PORT_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#include "blockconnection.h"

class Line;

class Block;
/**
 * @brief The Port class contains the information about ports.
 */
class Port : public QGraphicsItem
{
public:
    enum { Type = UserType + 2 };
    /**
     * @brief The portType enum contains a set of port's types.
     */
    enum portType { InPort, OutPort };
    /**
     * @brief Port it is a constructor.
     * @param parentBlock is a block, where this port is.
     * @param type is a type of a port.
     * @param numberOfPorts is a number of ports what block has.
     * @param position is a position of a port on the block.
     */
    Port(Block* parentBlock, portType type, int numberOfPorts, int position);

    /**
     * @brief type returns the type of a port.
     * @return port's type.
     */
    int type() const;
    /**
     * @brief getPortType returns the type of a port.
     * @return port's type.
     */
    portType getPortType();
    /**
     * @brief parentBlock returns the block, what has this port.
     * @return block containing this port.
     */
    Block* parentBlock();
    /**
     * @brief getRelPos returns the position of a port.
     * @return the port's position.
     */
    QPointF getRelPos();
    /**
     * @brief boundingRect returns the coordinates of a port.
     * @return port's coordinates.
     */
    QRectF boundingRect() const;
    /**
     * @brief itemChange determines the position of a port, a calls a function to move lines.
     * @param change controls port state.
     * @param value is a new position of a port.
     * @return this function.
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    /**
     * @brief paint draws ports.
     * @param painter is a pointer to painter.
     * @param option is a pointer to style's options. Doesn't use.
     * @param parent is a pointer to the parent. Doesn't use.
     */
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *parent);
    /**
     * @brief addConnection adds connection to the list.
     * @param line is a connection between two blocks.
     * @param isFirstPoint is a bool value, what cheks if it is the first port or not.
     * @param firstPort is a first port.
     * @param secondPort is a second port.
     */
    void addConnection(Line *line, bool isFirstPoint, Port* firstPort, Port* secondPort);
    /**
     * @brief existsConnection checks if the connection exists.
     * @param port is a port for what this connection is checking.
     * @return bool value. True if the connection exists. False if it isn't.
     */
    bool existsConnection(Port* port);
    /**
     * @brief getNextBlocks returns the list of blocks, what are connected with this port.
     * @return blocks, what are connected with this port.
     */
    QList<Block*> getNextBlocks();
    /**
     * @brief getNextPorts returns the list of ports, what are connected whith this port.
     * @return list of port.
     */
    QList<Port*> getNextPorts();
    /**
     * @brief isConnected checks if the port is connected or not.
     * @return bool value. True is if it is a connected, otherwise False is.
     */
    bool isConnected();
    /**
     * @brief isInput checks if this port's type is a Inport.
     * @return bool value. True is if it is a connected, otherwise False is.
     */
    bool isInput();
    /**
     * @brief isOutput checks if this port's type is a OutPort.
     * @return bool value. True if it is a connected, otherwise False is.
     */
    bool isOutput();
    /**
     * @brief removeConnections removes selected connections.
     * @param toDelete the connections what must be deleted.
     */
    void removeConnections(QList<QGraphicsLineItem*>* toDelete);
    /**
     * @brief removeConnections removes connections.
     * @param port is the port for what the connection must be deleted.
     */
    void removeConnections(Port* port);
    /**
     * @brief removeConnection removes the selected connection.
     * @param line is the connection, what must be deleted.
     */
    void removeConnection(QGraphicsLineItem* line);
    /**
     * @brief getData return value what contains port's block.
     * @return data of a block.
     */
    double getData();
    /**
     * @brief setData writes data to the block, where this port is.
     * @param data is a value, what must be written.
     */
    void setData(double data);
    /**
     * @brief areDataSet checks if port's block has data.
     * @return bool value. True is if port's block has a data, otherwise it is False.
     */
    bool areDataSet();
    /**
     * @brief unsetData sets data to False.
     */
    void unsetData();
    /**
     * @brief numberOfPortRec returns the number of a port.
     * @return the number of a port.
     */
    int numberOfPortRec();
    /**
     * @brief selectPort Marks a port as selected.
     */
    void selectPort();
    /**
     * @brief unselectPort Marks a port as unselected.
     */
    void unselectPort();
    /**
     * @brief isSelected Check whether the port is selected or not.
     * @return Returns true if port is selected, false otherwise.
     */
    bool isSelected();
private:
    portType pType; /**< type of a port.*/
    Block* parent; /**< block that contains this port.*/
    QPointF relPos; /**< position of a port in the scene relatively to it's block.*/
    double inputData; /**< data contained in the port.*/
    bool dataSet; /**< for checking if the port has data.*/
    bool selected; /**< for checing whether the port is selected.*/
    int numberOfPort; /**< number of the port*/
    QList<BlockConnection*> conList; /**< list of connections*/
    /**
     * @brief moveLinesToCenter moves lines, when block is moving.
     * @param newPos is a new position.
     */
    void moveLinesToCenter(QPointF newPos);
};

#endif // PORT_H
