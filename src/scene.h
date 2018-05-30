/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief An abstract class of a scene.
 * @file scene.h
 *
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QDebug>
#include <QLineEdit>
#include <QStatusBar>
#include <QTimer>
#include <QTextStream>
#include "port.h"
#include "block.h"
#include "line.h"

/**
 * @brief The Scene class contains the information about what is on the scene.
 */
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief The BlockInfo struct contains all information needed to recreate a block.
     * It is used to load data from save file.
     */
    struct BlockInfo {
        Block::blockType type;
        int id;
        int x;
        int y;
        QList<QPair<int, int>> connections;
    };
    /**
     * @brief The Mode enum contains a set of scene's modes.
     */
    enum Mode {NoMode, SelectObject, DrawLine};
    /**
     * @brief Scene is a constructor.
     * @param parent is a pointer to object.
     */
    Scene(QObject* parent = 0);
    /**
     * @brief setMode sets mode of a scene.
     * @param mode is a mode of a scene.
     */
    void setMode(Mode mode);
    /**
     * @brief getMode returns the mode of a scene.
     * @return scene's mode.
     */
    Mode getMode();
    /**
     * @brief numberOfBlocks retruns a count of blocks.
     * @return count of block.
     */
    int numberOfBlocks();
    /**
     * @brief blockListAppend adds block to a list.
     * @param block is a block, what must be added.
     */
    void blockListAppend(Block* block);
    /**
     * @brief addItem adds block to the scene.
     * @param block is block what must be added to the scene.
     */
    void addItem(Block* block);
    /**
     * @brief containsLoops checks if the scheme contains loops.
     * @return bool value. True is if the scheme contains loops, otherwise is a False.
     */
    bool containsLoops();
    /**
     * @brief allInputPortsConnected checks if all Input Ports are connected.
     * @return bool value. True is all InPort are connected, otherwise it is False.
     */
    bool allInputPortsConnected();
    /**
     * @brief allInputBlocksInitialized checks if all blocks are initialized.
     * @return bool value. True is if all block are initialized, otherwise it is False.
     */
    bool allInputBlocksInitialized();
    /**
     * @brief calculateAll calculates all values in the scheme.
     * @param err is parameter for controlling if calculation can be done successful.
     */
    void calculateAll(Block::calcError* err);
    /**
     * @brief calculateNext calculates values step by step.
     * @param err is parameter for controlling if calculation can be done successful.
     */
    void calculateNext(Block::calcError* err);
    /**
     * @brief resetCalculation resets calculation.
     */
    void resetCalculation();
    /**
     * @brief allCalculated checks if everything is caclulated.
     * @return bool value. True if all is calculated, otherwise it is False.
     */
    bool allCalculated();
    /**
     * @brief getLastCalculated returns last calculated block.
     * @return last calculated block.
     */
    Block* getLastCalculated();
    /**
     * @brief setCalcComplete sets true, because calculation is complete.
     */
    void setCalcComplete();
    /**
     * @brief redrawScene Schedules the redraw for all objects on the scene.
     */
    void redrawScene();
    /**
     * @brief deleteAll deletes everything on the scene.
     */
    void deleteAll();
    /**
     * @brief blockListRet returns list of blocks.
     * @return list of blocks.
     */
    QList<Block* >getBlockList();
    /**
     * @brief saveBlocksToFile Writes blocks data to a given file.
     * @param fileName Full path to the file obtained with method QFileDialog::getSaveFileName().
     * @return Returns true if operation was succesful, false otherwise.
     */
    bool saveBlocksToFile(const QString &fileName);
    /**
     * @brief getBlock Finds a block with a given id.
     * @param id An id to identify a block.
     * @return Block with given id if found, NULL otherwise.
     */
    Block* getBlock(int id);
    /**
     * @brief createConnection Creates a connection between two given ports.
     * @param first First port (output)
     * @param second Second port (input)
     */
    void createConnection(Port* first = NULL, Port* second = NULL);
    /**
     * @brief loadConnections Creates connections from information from a save file.
     * @param loadList A list of strucutures BlockInfo containing information about blocks in the scene.
     */
    void loadConnections(QList<BlockInfo> loadList);
public slots:
    /**
     * @brief portUnselect Unselects the selected ports.
     * @param first First selected port.
     * @param second Second selected port.
     */
    void portUnselect(Port* first, Port* second);
protected:
    /**
     * @brief mousePressEvent do actions if mouse button is pressed.
     * @param event is a mouse event.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /**
     * @brief keyPressEvent Executes actions when a keyborad key is pressed.
     * @param event is a key event.
     */
    void keyPressEvent(QKeyEvent *event);
private:
    Mode sceneMode;
    QList<Block*> blockList;
    QList<Block*> toCalculate;
    Port* firstPort;
    Port* secondPort;
    bool calculationComplete;
    Block* lastCalculated;

    QList<Port*> getScenePorts();
    void makeItemsControllable(bool areControllable);
    void deleteSelectedItems();
    void deleteLine(QGraphicsLineItem* line);
    void deleteBlock(Block* block);
    QString selectedPorts();
    void getClickedFirstPort();
    void getClickedSecondPort();
    void calculateHelperFunc(Block::calcError* err, Block* block);
    Line* addLine(const QLineF &line);
};

#endif // SCENE_H
