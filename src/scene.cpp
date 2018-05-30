/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief A representation of a scene.
 * @file scene.cpp
 *
 *
 */

#include "scene.h"
#include "mainwindow.h"

Scene::Scene(QObject* parent): QGraphicsScene(parent){
    sceneMode = NoMode;
    firstPort = 0;
    secondPort = 0;
    calculationComplete = false;
    lastCalculated = NULL;
}

void Scene::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode = QGraphicsView::NoDrag;
    if(mode == DrawLine){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
        // Reset the first/second block selection
        if (firstPort) {
            firstPort->unselectPort();
            redrawScene();
        }
        firstPort = 0;
        secondPort = 0;
    }
    else if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
        // Reset the first/second block selection
        if (firstPort) {
            firstPort->unselectPort();
            redrawScene();
        }
        firstPort = 0;
        secondPort = 0;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

Scene::Mode Scene::getMode() {
    return sceneMode;
}

void Scene::addItem(Block *block)
{
    QGraphicsScene::addItem(block);
    foreach (Port* port, block->getPortList()) {
        QGraphicsScene::addItem(port);
    }
}

void Scene::blockListAppend(Block* block) {
    blockList.append(block);
}

QList<Port*> Scene::getScenePorts()
{
    QList<Port*> list;
    foreach (Block* block, blockList) {
        list.append(block->getPortList());
    }
    return list;
}


void Scene::makeItemsControllable(bool areControllable) {
    foreach(QGraphicsItem* item, items()){
        if (item->type() == Block::Type) {
            item->setFlag(QGraphicsItem::ItemIsMovable, areControllable);
            item->setFlag(QGraphicsItem::ItemIsSelectable, areControllable);
        }
        else if (item->type() == QGraphicsLineItem::Type) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, areControllable);
        }
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(sceneMode == DrawLine) {
        if (!firstPort) {
            getClickedFirstPort();
        }
        else {
            getClickedSecondPort();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::getClickedFirstPort()
{
    // Get status bar
    QStatusBar* bar = ((MainWindow*)parent())->statusBar();

    // Find the port under the mouse
    foreach(Port* port, getScenePorts()) {
        if (port->isUnderMouse()) {
            firstPort = port;
            break;
        }
    }
    bar->showMessage(selectedPorts(), 2000);

    // Check the result
    if (firstPort) {
        if (firstPort->isInput() && firstPort->isConnected()) {
            bar->showMessage("Input port can have one connection at most.", 2000);
            firstPort = 0;
        }
        else {
            firstPort->selectPort();
            redrawScene();
        }
    }
}
void Scene::getClickedSecondPort()
{
    // Get status bar
    QStatusBar* bar = ((MainWindow*)parent())->statusBar();

    // Find the port under the mouse
    foreach(Port* port, getScenePorts()){
        if (port->isUnderMouse()) {
            secondPort = port;
            break;
        }
    }
    bar->showMessage(selectedPorts(), 2000);

    // Check the result
    if (secondPort) {
        if (secondPort->isInput() && secondPort->isConnected()) {
            bar->showMessage("Input port can have one connection at most.", 2000);
        } else if (firstPort->parentBlock() == secondPort->parentBlock()) {
            bar->showMessage("Cannot create a connection to the same block.", 2000);
        } else if (firstPort->getPortType() == secondPort->getPortType()) {
            bar->showMessage("Cannot connect input to input or output to output.", 2000);
        } else {
            createConnection();
        }
        secondPort = 0;
    }
}

void Scene::createConnection(Port* firstP, Port* secondP)
{
    if (firstP) firstPort = firstP;
    if (secondP) secondPort = secondP;

    if (firstPort->isInput()) {
        Port* aux = firstPort;
        firstPort = secondPort;
        secondPort = aux;
    }
    QPointF pos1;
    QPointF pos2;
    pos1.setX(firstPort->scenePos().x() + firstPort->boundingRect().width()/2);
    pos1.setY(firstPort->scenePos().y() + firstPort->boundingRect().height()/2);
    pos2.setX(secondPort->scenePos().x() + secondPort->boundingRect().width()/2);
    pos2.setY(secondPort->scenePos().y() + secondPort->boundingRect().height()/2);

    Line* lineToDraw = addLine(QLineF(pos1, pos2));
    firstPort->addConnection(lineToDraw, true, firstPort, secondPort);
    secondPort->addConnection(lineToDraw, false, firstPort, secondPort);
    firstPort->selectPort();
    secondPort->selectPort();
    redrawScene();
    Port* first = firstPort;
    Port* second = secondPort;
    QTimer::singleShot(200, this, [this, first, second] () {portUnselect(first, second); });
    firstPort = 0;
    secondPort = 0;
}

Line* Scene::addLine(const QLineF &line)
{
    Line* newLine = new Line(line, secondPort);
    QGraphicsScene::addItem(newLine);
    return newLine;
}

QString Scene::selectedPorts() {
    QString x1, y1, x2, y2;
    x1 = y1 = x2 = y2 = "  ";
    if (firstPort) {
        x1 = x1.number(firstPort->pos().x());
        y1 = y1.number(firstPort->pos().y());
    }
    if (secondPort) {
        x2 = x2.number(secondPort->pos().x());
        y2 = y2.number(secondPort->pos().y());
    }
    return QString("First port: (%1,%2) Second port: (%3,%4)").arg(x1, y1, x2, y2);
}

void Scene::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete)
        deleteSelectedItems();
    else if (event->key() == Qt::Key_Escape) {
        portUnselect(firstPort, secondPort);
        firstPort = 0;
        secondPort = 0;
    }
    else
        QGraphicsScene::keyPressEvent(event);
}

void Scene::deleteSelectedItems(){
    // First all the selected lines must be deleted
    foreach(QGraphicsItem* item, selectedItems()){
        if (item->type() == QGraphicsLineItem::Type) {
            deleteLine((QGraphicsLineItem*)item);
        }
    }
    // Then all the blocks
    foreach(QGraphicsItem* item, selectedItems()){
        if (item->type() == Block::Type) {
            deleteBlock((Block*)item);
        }
    }
}

void Scene::deleteAll(){

    foreach (Block* delBlock, blockList) {
        deleteBlock(delBlock);
    }
}

void Scene::deleteLine(QGraphicsLineItem* line) {
    foreach(Port* port, getScenePorts()){
        port->removeConnection(line);
    }
    removeItem(line);
    delete line;
}

void Scene::deleteBlock(Block* block) {
    QList<QGraphicsLineItem*> toDelete;
    foreach(Port* port, block->getPortList()) {
        port->removeConnections(&toDelete);
        block->removePort(port);
    }
    foreach(QGraphicsLineItem* line, toDelete) {
        foreach(Port* port, getScenePorts()) {
            port->removeConnection(line);
        }
        removeItem(line);
        delete line;
    }
    blockList.removeOne(block);
    removeItem(block);
    delete block;
}

bool Scene::containsLoops()
{
    foreach (Block* block, blockList) {
        foreach (Block* nextBlock, block->getNextBlocks()) {
            if (nextBlock->containsLoops(block))
                return true;
        }
    }
    return false;
}

bool Scene::allInputPortsConnected()
{
    foreach (Block* block, blockList) {
        if (!block->allInputPortsConnected())
            return false;
    }
    return true;
}

bool Scene::allInputBlocksInitialized()
{
    foreach (Block* block, blockList) {
        if (block->getBlockType() == Block::Input && !block->areDataSet())
            return false;
    }
    return true;
}

void Scene::calculateAll(Block::calcError* err)
{
    if (toCalculate.empty()) {
        toCalculate.append(blockList);
    }
    foreach (Block* block, toCalculate) {
        if (block->getBlockType() == Block::Input) {
            calculateHelperFunc(err, block);
        }
    }
    while (!toCalculate.empty()) {
        foreach (Block* block, toCalculate) {
            if (block->allInputsSet()) {
                calculateHelperFunc(err, block);
                if (*err) {
                    calculationComplete = true;
                    return;
                }
            }
        }
    }
    calculationComplete = true;
}

void Scene::calculateNext(Block::calcError* err)
{
    if (toCalculate.empty()) {
        toCalculate.append(blockList);
    }
    foreach (Block* block, toCalculate) {
        if (block->getBlockType() == Block::Input) {
            calculateHelperFunc(err, block);
            return;
        }
    }
    foreach (Block* block, toCalculate) {
        if (block->allInputsSet()) {
            calculateHelperFunc(err, block);
            if (toCalculate.empty()) {
                calculationComplete = true;
            }
            break;
        }
    }
    if (*err) {
        calculationComplete = true;
        return;
    }
}

void Scene::calculateHelperFunc(Block::calcError* err, Block* block) {
    block->doCalculation(err);
    toCalculate.removeOne(block);
    lastCalculated = block;
    redrawScene();
}

void Scene::resetCalculation()
{
    calculationComplete = false;
    toCalculate.clear();
    lastCalculated = NULL;
    foreach (Block* block, blockList) {
        if (block->getBlockType() == Block::Output) {
            block->clearOutputField();
        }
        if (block->getBlockType() != Block::Input)
            block->unsetData();
        foreach (Port* port, block->getPortList()) {
            port->unsetData();
        }
    }
    redrawScene();
}

bool Scene::allCalculated()
{
    if (calculationComplete)
        return true;
    return false;
}

Block *Scene::getLastCalculated()
{
    return lastCalculated;
}

void Scene::setCalcComplete()
{
    calculationComplete = true;
}

void Scene::redrawScene()
{
    qDebug() << "redrawing scene";
    foreach (QGraphicsItem* item, items()) {
        item->update();
    }
}

void Scene::portUnselect(Port* first, Port* second)
{
    if (first) first->unselectPort();
    if (second) second->unselectPort();
    redrawScene();
}

int Scene::numberOfBlocks()
{
    return blockList.count();
}

QList<Block*> Scene::getBlockList(){
    return blockList;
}

bool Scene::saveBlocksToFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning((QWidget*)parent(), "Unable to open file", file.errorString());
        return false;
    }

    QTextStream out(&file);

    out << QString::fromStdString("<type> <id> <x> <y> <number_of_outputs> <<next_id><port>> <<next_id><port>> ...\n");

    foreach (Block* block, blockList) {
        block->writeToStream(&out);
    }

    file.close();
    return true;
}

Block *Scene::getBlock(int id)
{
    foreach (Block* block, blockList) {
        if (block->idBlock() == id)
            return block;
    }
    return NULL;
}

void Scene::loadConnections(QList<BlockInfo> loadList) {
    foreach (BlockInfo entry, loadList) {
        Port* firstPort = getBlock(entry.id)->getOutPort();
        QPair<int,int> pair;
        foreach (pair, entry.connections) {
            Port* secondPort = getBlock(pair.first)->getPort(pair.second, Port::InPort);
            createConnection(firstPort, secondPort);
        }
    }
}
