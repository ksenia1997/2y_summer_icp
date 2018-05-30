/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief A representation of a block.
 * @file block.cpp
 *
 *
 */

#include "block.h"
#include "scene.h"

int Block::idCounter = 0;

Block::Block(blockType type, Scene* scene, QPoint position, int newId){
    bType = type;
    this->parentScene = scene;

    if (newId >= 0) {
        id = newId;
        idCounter = qMax(idCounter+1, id);
    }
    else {
        id = idCounter++;
    }

    setPos(position);
    dataSet = false;
    setFlag(ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);

    QGraphicsProxyWidget* pMyProxy;
    QRectF rect;
    switch (bType) {
    case Add:
        createPorts(2, Port::InPort);
        createPorts(1, Port::OutPort);
        title = "Addition";
        break;
    case Sub:
        createPorts(2, Port::InPort);
        createPorts(1, Port::OutPort);
        title = "Subtraction";
        break;
    case Mul:
        createPorts(2, Port::InPort);
        createPorts(1, Port::OutPort);
        title = "Multiplication";
        break;
    case Div:
        createPorts(2, Port::InPort);
        createPorts(1, Port::OutPort);
        title = "Division";
        break;
    case Pow2:
        createPorts(1, Port::InPort);
        createPorts(1, Port::OutPort);
        title = "Power of 2";
        break;
    case PowX:
        createPorts(2, Port::InPort);
        createPorts(1, Port::OutPort);
        title = "Power of X";
        break;
    case Sqrt:
        createPorts(1, Port::InPort);
        createPorts(1, Port::OutPort);
        title = "Square root";
        break;
    case Input:
        createPorts(1, Port::OutPort);
        title = "Input";
        textBox = new QLineEdit();
        textBox->setPlaceholderText("input");
        textBox->setValidator(new QDoubleValidator(-1.79769e+308, 1.79769e+308, 50, this));
        pMyProxy = new QGraphicsProxyWidget(this);
        rect = boundingRect();
        rect.setX(rect.x() + 20);
        rect.setY(rect.y() + 20);
        rect.setWidth(rect.width() - 20);
        rect.setHeight(20);
        pMyProxy->setWidget(textBox);
        pMyProxy->setGeometry(rect);
        QObject::connect(textBox, SIGNAL(textChanged(QString)), this, SLOT(inputChanged(QString)));
        break;
    case Output:
        createPorts(1, Port::InPort);
        title = "Output";
        textBox = new QLineEdit();
        textBox->setReadOnly(true);
        pMyProxy = new QGraphicsProxyWidget(this);
        rect = boundingRect();
        rect.setX(rect.x() + 20);
        rect.setY(rect.y() + 20);
        rect.setWidth(rect.width() - 20);
        rect.setHeight(20);
        pMyProxy->setWidget(textBox);
        pMyProxy->setGeometry(rect);
        break;
    }
}

int Block::type() const {
    return Type;
}

Port *Block::getPort(int position, Port::portType type)
{
    if (type == Port::InPort) {
        foreach (Port* port, inPortList) {
            if (port->numberOfPortRec() == position)
                return port;
        }
    }
    else {
        foreach (Port* port, outPortList) {
            if (port->numberOfPortRec() == position)
                return port;
        }
    }
    return NULL;
}

int Block::idBlock() {
    return id;
}

Block::blockType Block::getBlockType()
{
    return bType;
}

QRectF Block::boundingRect() const {
    return QRectF(0, 0, 120, 50);
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent){
    Q_UNUSED(parent);
    Q_UNUSED(option);

    // Get block rectangle size
    QRectF rectangle = boundingRect();

    // Draw the rectangle
    if (parentScene->getLastCalculated() == this) {
        QPen pen(Qt::darkBlue);
        pen.setWidth(4);
        painter->setPen(pen);
    }

    QBrush brush(Qt::gray);
    painter->fillRect(rectangle, brush);
    painter->drawRect(rectangle);
    painter->drawText(rectangle, Qt::AlignHCenter, title);
}

QVariant Block::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        qDebug() << "blockChange:" << newPos;
        movePortsWithBlock(newPos);
    }
    return QGraphicsItem::itemChange(change, value);
}

void Block::movePortsWithBlock(QPointF newPos) {
    QPointF position;
    int x, y;
    foreach (Port* port, getPortList()) {
        x = port->getRelPos().x() + newPos.x();
        y = port->getRelPos().y() + newPos.y();
        position = QPointF(x, y);
        port->setPos(position);
    }
}

void Block::createPorts(int numberOfPorts, Port::portType type)
{
    for (int i = 0; i < numberOfPorts; i++) {
        Port* port = new Port(this, type, numberOfPorts, i);
        if (type == Port::InPort)
            inPortList.append(port);
        else
            outPortList.append(port);
    }
}

void Block::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event)

    if (dataSet)
        setToolTip(QString::number(blockData));
    else
        setToolTip("No value");
}

void Block::writeToStream(QTextStream* stream)
{
    *stream << QString::number(int(bType)) << " ";
    *stream << QString::number(id) << " ";
    *stream << QString::number(pos().x()) << " ";
    *stream << QString::number(pos().y()) << " ";
    *stream << QString::number(getNextBlocks().size()) << " ";
    int id_previous = -1;
    foreach (Block* next, getNextBlocks()) {
        *stream << QString::number(next->idBlock()) << " ";
        if (id_previous == next->idBlock())
            *stream << QString::number(next->getInPortConnectedToBlock(this)->numberOfPortRec()+1) << " ";
        else
            *stream << QString::number(next->getInPortConnectedToBlock(this)->numberOfPortRec()) << " ";
        id_previous = next->idBlock();
    }
    *stream << "\n";
}

Port *Block::getInPortConnectedToBlock(Block *block)
{
    Port* outPort = block->getOutPort();
    foreach (Port* port, inPortList) {
        if (port->existsConnection(outPort))
            return port;
    }
    return NULL;
}

QList<Port*> Block::getPortList()
{
    QList<Port*> list;
    list.append(inPortList);
    list.append(outPortList);
    return list;
}

Port* Block::getOutPort()
{
    // QList::value() returns default constructed value
    // if the position is out of bounds
    return outPortList.value(0);
}

QList<Port*> Block::getInPortList()
{
    return inPortList;
}

QList<Block*> Block::getNextBlocks()
{
    QList<Block*> list;
    foreach (Port* port, outPortList) {
        list.append(port->getNextBlocks());
    }
    return list;
}

bool Block::allInputPortsConnected()
{
    foreach (Port* port, inPortList) {
        if (!port->isConnected())
            return false;
    }
    return true;
}

bool Block::allInputsSet()
{
    foreach (Port* port, inPortList) {
        if (!port->areDataSet())
            return false;
    }
    return true;
}

void Block::removePort(Port *port)
{
    inPortList.removeOne(port) || outPortList.removeOne(port);
    parentScene->removeItem(port);
    delete port;
}

void Block::doCalculation(Block::calcError* err)
{
    if (!allInputPortsConnected())
        return;

    double input1;
    double input2;
    switch (bType) {
    case Add:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        input2 = inPortList.at(1)->getData();
        // Calculate new value
        setData(input1 + input2);
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case Sub:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        input2 = inPortList.at(1)->getData();
        // Calculate new value
        setData(input1 - input2);
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case Mul:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        input2 = inPortList.at(1)->getData();
        // Calculate new value
        setData(input1 * input2);
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case Div:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        input2 = inPortList.at(1)->getData();
        // Calculate new value
        if (input2 == 0) {
            if (err)
                *err = DivByZero;
            return;
        }
        setData(input1 / input2);
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case Pow2:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        // Calculate new value
        setData(input1*input1);
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case PowX:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        input2 = inPortList.at(1)->getData();
        // Calculate new value
        setData(qPow(input1, input2));
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case Sqrt:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        // Calculate new value
        setData(qSqrt(input1));
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case Input:
        // Delegate
        foreach (Port* nextPort, outPortList.first()->getNextPorts()) {
            nextPort->setData(this->blockData);
        }
        break;
    case Output:
        // Get inputs
        input1 = inPortList.at(0)->getData();
        // Calculate new value
        setData(input1);
        // Delegate
        textBox->setText(QString::number(blockData));
        break;
    }
    qDebug() << "Calculating block:" << id;
}

double Block::getData()
{
    return blockData;
}

void Block::setData(double data)
{
    qDebug() << "block setData:" << data;
    blockData = data;
    dataSet = true;
}

bool Block::areDataSet()
{
    return dataSet;
}

void Block::unsetData()
{
    dataSet = false;
}

void Block::clearOutputField()
{
    if (bType != Output) return;
    textBox->setText(QString(""));
}

void Block::inputChanged(const QString &text)
{
    setData(QLocale().toDouble(text));
    qDebug() << "Value updated:" << getData();
}

bool Block::containsLoops(Block *checkedBlock)
{
    if (checkedBlock == this) return true;

    foreach (Port* outPort, outPortList) {
        if (!outPort->isConnected())
            continue;
        foreach (Block* nextBlock, outPort->getNextBlocks()) {
            if (nextBlock->containsLoops(checkedBlock))
                return true;
        }
    }
    return false;
}


