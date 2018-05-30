/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief Main window file.
 * @file mainwindow.h
 *
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QDebug>

#include "scene.h"
#include "block.h"
/**
 * @brief The MainWindow class contains the information about application's buttons.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    /**
     * @brief linesActionGroupClicked creates a group of a line's mode(to move blocks, to create connection between blocks).
     */
    void linesActionGroupClicked(QAction*);
    /**
     * @brief addBlockClicked If clicked the button "ADD", calls function createBlock(), type of a block is Add.
     */
    void addBlockClicked();
    /**
     * @brief subBlockClicked If clicked the button "SUB", calls function createBlock(), type of a block is Sub.
     */
    void subBlockClicked();
    /**
     * @brief mulBlockClicked If clicked the button "MUL", calls function createBlock(), type of a block is Mul.
     */
    void mulBlockClicked();
    /**
     * @brief divBlockClicked If clicked the button "DIV", calls function createBlock(), type of a block is Div.
     */
    void divBlockClicked();
    /**
     * @brief pow2BlockClicked If clicked the button "POW2", calls function createBlock(), type of a block is Pow2.
     */
    void pow2BlockClicked();
    /**
     * @brief powXBlockClicked Call function createBlock() when "POWX" button is clicked, type of a block is PowX.
     */
    void powXBlockClicked();
    /**
     * @brief sqrtBlockClicked Call function createBlock() when "SQRT" button is clicked, type of a block is Sqrt.
     */
    void sqrtBlockClicked();
    /**
     * @brief inBlockClicked If clicked the button "INPUT", calls function createBlock(), type of a block is Input.
     */
    void inBlockClicked();
    /**
     * @brief outBlockClicked If clicked the button "OUTPUT", calls function createBlock(), type of a block is Output.
     */
    void outBlockClicked();
    /**
     * @brief createBlock creates a block of a given type.
     * @param type is a type of a block, what must be created.
     * @param position Position of the block in the scene.
     * @param id Id of the block to create. Cannot be negative and must not be used.
     */
    void createBlock(Block::blockType type, QPoint position = QPoint(0,0), int id = -1);
    /**
     * @brief help writes the information in a new window, how to work in the application when clicked the button "Help".
     */
    void help();
    /**
     * @brief save save a file, when clicked the button "Save".
     */
    void save();
    /**
     * @brief open open a file, when clicked the button "Open".
     */
    void open();
    /**
     * @brief newFile Creates an empty workspace.
     */
    void newFile();
    /**
     * @brief aboutQt Brings up a dialoge window with information about Qt.
     */
    void aboutQt();
    /**
     * @brief calculateNext gradually calculates values. Exists for calculating step by step.
     */
    void calculateNext();
    /**
     * @brief calculateAll calculates all values in the scheme.
     */
    void calculateAll();
    /**
     * @brief reset resets output values.
     */
    void reset();
    /**
     * @brief clearing clears the scene.
     */
    void clear();
    /**
     * @brief exit Exits the application.
     */
    void exit();
private:
    QGraphicsView* view;
    Scene* scene;
    Block* block;
    QAction* lineAction;
    QAction* selectAction;
    QActionGroup* linesActionGroup;
    QAction* calculateAllButton;
    QAction* calculateNextButton;
    QAction* resetButton;
    QAction* clearButton;
    QAction* helpButtonAct;
    QAction* newButtonAct;
    QAction* saveButtonAct;
    QAction* openButtonAct;

    QAction* addBlock;
    QAction* subBlock;
    QAction* mulBlock;
    QAction* divBlock;
    QAction* pow2Block;
    QAction* powXBlock;
    QAction* sqrtBlock;
    QAction* outBlock;
    QAction* inBlock;

    QMenu* fileMenu;
    QMenu* helpMenu;
    QMenu* editMenu;
    QMenu* calculationMenu;
    QMenu* blocksMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QToolBar* drawingToolBar;
    QToolBar* blocksToolBar;
    QToolBar* controlToolBar;

    bool calculationReady();
    void ensureModeIsSelect();
    void createActions();
    void createMenus();
};

#endif // MAINWINDOW_H
