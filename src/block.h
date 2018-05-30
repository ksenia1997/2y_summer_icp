/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief An abstract class of a block.
 * @file block.h
 *
 *
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QDebug>
#include <QtMath>
#include <QTextStream>
#include "port.h"

class Scene;
/**
 * @brief The Block class is a class of a block.
 */
class Block : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };
    /**
     * @brief The blockType enum contains a set of block's types.
     */
    enum blockType { Add, Sub, Mul, Div, Pow2, PowX, Sqrt, Input, Output };
    /**
     * @brief The calcError enum contains a set of calculational errors.
     */
    enum calcError { NoErr=0, DivByZero };
    /**
     * @brief Block is the constructor.
     * @param type type of a block.
     * @param scene is te place where the block is.
     * @param position Position of the block in the scene.
     * @param id If specified, this id will be used for the block. It has to be a positive integer (or zero) and the id cannot be already in use in the scene.
     */
    Block(blockType type, Scene* scene, QPoint position = QPoint(0,0), int id = -1);
    /**
     * @brief getPort Gets a port in block with given position.
     * @param position Position in a block, starts with zero.
     * @param type Type of port.
     * @return Port with given position if found, NULL otherwise.
     */
    Port* getPort(int position, Port::portType type = Port::InPort);
    /**
     * @brief type returns the type of a block.
     * @return type of a block.
     */
    int type() const;
    /**
     * @brief getBlockType returns the type of a block from the enum blockType's set.
     * @return
     */
    blockType getBlockType();
    /**
     * @brief boundingRect contains the size of a block.
     * @return  the size of a block.
     */
    QRectF boundingRect() const;
    /**
     * @brief itemChange determines the position of the block and calls the function to move ports of this block.
     * @param change controls block state.
     * @param value is a new position of a block.
     * @return this function.
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    /**
     * @brief paint draws blocks.
     * @param painter is a pointer to QPainter.
     * @param option is a pointer to style's options. Doesn't use.
     * @param parent is a pointer to the parent. Doesn't use.
     */
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *parent);
    /**
     * @brief getPortList returns a list of all ports what block contains.
     * @return a list of block's ports.
     */
    QList<Port*> getPortList();
    /**
     * @brief getNextBlocks returns a list of blocks what are connected with this block.
     * @return a list of next connected blocks.
     */
    QList<Block*> getNextBlocks();
    /**
     * @brief allInputPortsConnected checks if all Input ports are connected.
     * @return bool value. True is if all ports are connected, otherwise False is returned.
     */
    bool allInputPortsConnected();
    /**
     * @brief allInputsSet checks if all Input ports have a value.
     * @return bool value. True is if all Input ports have a value, otherwise is False.
     */
    bool allInputsSet();
    /**
     * @brief containsLoops checks if the scheme doesn't contain cycles. Recursively checks all blocks.
     * @param block is the block for what it is checking.
     * @return bool value. True is if it contains a loop, False is if it doesn't.
     */
    bool containsLoops(Block* block);
    /**
     * @brief removePort deletes a port.
     * @param port is the port, what must be deleted.
     */
    void removePort(Port* port);
    /**
     * @brief doCalculation does calculating of the scheme's values.
     * @param err is information about errors.
     */
    void doCalculation(calcError* err = NULL);
    /**
     * @brief getData returns a value of a block.
     * @return data of a block.
     */
    double getData();
    /**
     * @brief setData writes data of a block.
     * @param data is a value what should be written.
     */
    void setData(double data);
    /**
     * @brief areDataSet checks if a block has data.
     * @return bool value. True if block has data, otherwise it is False.
     */
    bool areDataSet();
    /**
     * @brief unsetData set the block's data to False.
     */
    void unsetData();
    /**
     * @brief clearOutputField clear the field of output port.
     */
    void clearOutputField();
    /**
     * @brief idBlock returns the id of a block.
     * @return id of a block.
     */
    int idBlock();
    /**
     * @brief getOutPort returns an output port of a block.
     * @return output port.
     */
    Port* getOutPort();
    /**
     * @brief getInPortList returns a list of input ports of a block.
     * @return list of input ports.
     */
    QList<Port*> getInPortList();
    /**
     * @brief writeToStream Writes block parameters to a stream.
     * @param stream Stream to write the parameters to.
     */
    void writeToStream(QTextStream* stream);
    /**
     * @brief getInPortConnectedToBlock Returns a port which is connected to the given block.
     * @param block Block, about which we want to know, to what port it is connected.
     * @return Returns a pointer to a port, if it is found, or nullPtr otherwise.
     */
    Port* getInPortConnectedToBlock(Block* block);
private slots:
    /**
     * @brief inputChanged insert changed text of a input block.
     * @param text is a changed text.
     */
    void inputChanged(const QString &text);
private:
    Scene* parentScene; /**< pointer to the Scene, where we make a scheme.*/
    static int idCounter; /**< id of a block.*/
    int id;  /**< id of a block*/
    blockType bType; /**< type of a block.*/
    QList<Port*> inPortList; /**< List of input ports*/
    QList<Port*> outPortList; /**< List of output ports*/
    const char* title; /**< type of a block, what is written on the block.*/
    double blockData; /**< value of a block.*/
    bool dataSet; /**< variable what checks if block has data*/
    QLineEdit* textBox; /**< place where is written a block's value */

    /**
     * @brief movePortsWithBlock moves block's ports with the block.
     * @param newPos is new position of a block.
     */
    void movePortsWithBlock(QPointF newPos);
    /**
     * @brief createPorts creates ports, when was created a block.
     * @param numberOfPorts is a number of port. 1 is if it is output port. 2 or 1 is if it is a input port.
     * @param type is a type of a port.
     */
    void createPorts(int numberOfPorts, Port::portType type);
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
};

#endif // BLOCK_H
