/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief Mainwindow file.
 * @file mainwindow.cpp
 *
 *
 */

#include "mainwindow.h"
#include "block.h"

MainWindow::MainWindow()
{
    scene = new Scene(this);
    scene->setSceneRect(0,0,700,400);
    setMinimumSize(160, 160);
    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing);
    setCentralWidget(view);
    setWindowTitle("BlockEditor");
    QLocale().setDefault(QLocale::C);
    statusBar()->showMessage("Ready", 2000);

    createActions();
    createMenus();
}

void MainWindow::aboutQt()
{
    statusBar()->showMessage(tr("Invoked <b>Help|About Qt</b>"), 2000);
}

void MainWindow::createActions()
{
    lineAction = new QAction("Connect", this);
    lineAction->setData(int(Scene::DrawLine));
    lineAction->setIcon(QIcon(":/icons/line.png"));
    lineAction->setCheckable(true);
    lineAction->setShortcut(QKeySequence(Qt::Key_C));

    selectAction = new QAction("Select", this);
    selectAction->setData(int(Scene::SelectObject));
    selectAction->setIcon(QIcon(":/icons/select.png"));
    selectAction->setCheckable(true);
    selectAction->setShortcut(QKeySequence(Qt::Key_S));

    linesActionGroup = new QActionGroup(this);
    linesActionGroup->setExclusive(true);
    linesActionGroup->addAction(lineAction);
    linesActionGroup->addAction(selectAction);

    addBlock = new QAction("Add", this);
    addBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    subBlock = new QAction("Sub", this);
    subBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_S));
    mulBlock = new QAction("Mul", this);
    mulBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_M));
    divBlock = new QAction("Div", this);
    divBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_D));
    pow2Block = new QAction("Pow2", this);
    pow2Block->setShortcut(QKeySequence(Qt::ALT + Qt::Key_P));
    powXBlock = new QAction("PowX", this);
    powXBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_X));
    sqrtBlock = new QAction("Sqrt", this);
    sqrtBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_R));
    outBlock = new QAction("Output", this);
    outBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_O));
    inBlock = new QAction("Input", this);
    inBlock->setShortcut(QKeySequence(Qt::ALT + Qt::Key_I));

    calculateAllButton = new QAction("Calculate all", this);
    calculateAllButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    calculateNextButton = new QAction("Calculate next", this);
    calculateNextButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    resetButton = new QAction("Reset", this);
    resetButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    clearButton = new QAction("Clear", this);
    clearButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    // Create connections
    connect(linesActionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(linesActionGroupClicked(QAction*)));
    connect(addBlock, SIGNAL(triggered()), this, SLOT(addBlockClicked()));
    connect(subBlock, SIGNAL(triggered()), this, SLOT(subBlockClicked()));
    connect(mulBlock, SIGNAL(triggered()), this, SLOT(mulBlockClicked()));
    connect(pow2Block, SIGNAL(triggered()), this, SLOT(pow2BlockClicked()));
    connect(powXBlock, SIGNAL(triggered()), this, SLOT(powXBlockClicked()));
    connect(sqrtBlock, SIGNAL(triggered()), this, SLOT(sqrtBlockClicked()));
    connect(divBlock, SIGNAL(triggered()), this, SLOT(divBlockClicked()));
    connect(inBlock,  SIGNAL(triggered()), this, SLOT(inBlockClicked()));
    connect(outBlock, SIGNAL(triggered()), this, SLOT(outBlockClicked()));
    connect(calculateAllButton, SIGNAL(triggered()), this, SLOT(calculateAll()));
    connect(calculateNextButton, SIGNAL(triggered()), this, SLOT(calculateNext()));
    connect(resetButton, SIGNAL(triggered()), this, SLOT(reset()));
    connect(clearButton, SIGNAL(triggered()), this, SLOT(clear()));

    helpButtonAct = new QAction("Help", this);
    helpButtonAct->setIcon(QIcon(":/icons/help.png"));
    connect(helpButtonAct, SIGNAL(triggered()), this, SLOT(help()));

    saveButtonAct = new QAction("Save", this);
    saveButtonAct->setIcon(QIcon(":/icons/save.png"));
    connect(saveButtonAct, SIGNAL(triggered()), this, SLOT(save()));

    openButtonAct = new QAction("Open", this);
    openButtonAct->setIcon(QIcon(":/icons/open.png"));
    connect(openButtonAct, SIGNAL(triggered()), this, SLOT(open()));

    newButtonAct = new QAction("New", this);
    newButtonAct->setIcon(QIcon(":/icons/new.png"));
    connect(newButtonAct, SIGNAL(triggered()), this, SLOT(newFile()));

    // Create toolbars
    controlToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, controlToolBar);
    controlToolBar->addAction(newButtonAct);
    controlToolBar->addAction(openButtonAct);
    controlToolBar->addAction(saveButtonAct);
    controlToolBar->addAction(helpButtonAct);

    addToolBarBreak();
    drawingToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, drawingToolBar);
    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(lineAction);
    drawingToolBar->addAction(clearButton);
    drawingToolBar->addAction(calculateAllButton);
    drawingToolBar->addAction(calculateNextButton);
    drawingToolBar->addAction(resetButton);

    blocksToolBar = new QToolBar;
    addToolBar(Qt::LeftToolBarArea, blocksToolBar);
    blocksToolBar->addAction(addBlock);
    blocksToolBar->addAction(subBlock);
    blocksToolBar->addAction(mulBlock);
    blocksToolBar->addAction(divBlock);
    blocksToolBar->addAction(pow2Block);
    blocksToolBar->addAction(powXBlock);
    blocksToolBar->addAction(sqrtBlock);
    blocksToolBar->addAction(outBlock);
    blocksToolBar->addAction(inBlock);

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exit);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    aboutAct->setShortcut(QKeySequence(Qt::CTRL+ Qt::Key_H));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::help);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, QApplication::instance(), &QApplication::aboutQt);
    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(lineAction);
    editMenu->addAction(selectAction);
    editMenu->addSeparator();
    editMenu->addAction(clearButton);

    calculationMenu = menuBar()->addMenu(tr("&Calculation"));
    calculationMenu->addAction(calculateAllButton);
    calculationMenu->addAction(calculateNextButton);
    calculationMenu->addAction(resetButton);

    blocksMenu = menuBar()->addMenu(tr("&Blocks"));
    blocksMenu->addAction(addBlock);
    blocksMenu->addAction(subBlock);
    blocksMenu->addAction(mulBlock);
    blocksMenu->addAction(divBlock);
    blocksMenu->addAction(pow2Block);
    blocksMenu->addAction(powXBlock);
    blocksMenu->addAction(sqrtBlock);
    blocksMenu->addAction(outBlock);
    blocksMenu->addAction(inBlock);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::newFile() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "New file",
                                  "Are you sure you want to create a new file?\n"
                                  "All unsaved changes will be lost.",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      clear();
    }
}

void MainWindow::linesActionGroupClicked(QAction *action){
    // Set scene mode to the corresponding value
    scene->setMode(Scene::Mode(action->data().toInt()));
}

void MainWindow::addBlockClicked() {
    createBlock(Block::Add);
}

void MainWindow::subBlockClicked() {
    createBlock(Block::Sub);
}

void MainWindow::mulBlockClicked() {
    createBlock(Block::Mul);
}

void MainWindow::divBlockClicked() {
    createBlock(Block::Div);
}

void MainWindow::pow2BlockClicked() {
    createBlock(Block::Pow2);
}

void MainWindow::sqrtBlockClicked()
{
    createBlock(Block::Sqrt);
}

void MainWindow::powXBlockClicked()
{
    createBlock(Block::PowX);
}
void MainWindow::inBlockClicked() {
    createBlock(Block::Input);
}

void MainWindow::outBlockClicked() {
    createBlock(Block::Output);
}

void MainWindow::createBlock(Block::blockType type, QPoint position, int id) {
    block = new Block(type, scene, position, id);
    scene->addItem(block);
    scene->blockListAppend(block);
    selectAction->setChecked(true);
    lineAction->setChecked(false);
    linesActionGroup->triggered(selectAction);
}

void MainWindow::help()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Help");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("BlockEditor is an application for creating computational block schemes.<br>"
                   "<br>"
                   "To <b>add</b> a block, click on the operation name.<br>"
                   "<br>"
                   "To <b>connect</b> two blocks, first click the <i>Connect</i> operation and "
                   "then click on two ports.<br>"
                   "<br>"
                   "To <b>cancel selection of the port</b>, hit the <i>Esc</i> button on your keyboard.<br>"
                   "<br>"
                   "To <b>move</b> objects, first click the <i>Select</i> operation and then click "
                   "and drag the object. You can also create and move a rectangle selection.<br>"
                   "<br>"
                   "To <b>delete</b> a block or a connection, first click the <i>Select</i> operation, "
                   "then create a selection and press the <i>Del</i> button on your keyboard.<br>"
                   "<br>"
                   "To <b>calculate the whole scheme</b>, click <i>Calculate all</i>.<br>"
                   "<br>"
                   "To <b>calculate a next step</b>, click <i>Calculate next</i>.<br>"
                   "<br>"
                   "To <b>reset the calculation</b>, click <i>Reset</i>.<br>"
                   "<br>"
                   "To <b>clear</b> the scene, click <i>Clear</i>.<br>"
                    "<br>"
                    "To see all <b>shortcuts</b>, view the context menu.");
    msgBox.exec();
}

void MainWindow::ensureModeIsSelect() {
    selectAction->setChecked(true);
    lineAction->setChecked(false);
    linesActionGroup->triggered(selectAction);
}

void MainWindow::clear() {
    ensureModeIsSelect();
    scene->deleteAll();
}

void MainWindow::exit()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit",
                                  "Are you sure you want to exit?\n"
                                  "All unsaved changes will be lost.",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      QWidget::close();
    }
}

void MainWindow::save() {
    QString fileName =  QFileDialog::getSaveFileName(this, tr("Save a scheme"), "", tr("All Files (*)"));
    scene->saveBlocksToFile(fileName);
}

void MainWindow::open() {
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Open a scheme"), "", tr("All Files (*)"));
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning((QWidget*)parent(), "Unable to open file", file.errorString());
        return;
    }

    QTextStream in(&file);
    in.readLine(); // Get rid of the first line

    QList<Scene::BlockInfo> loadList;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QRegExp re("[\\d\\s-]*");
        if (!re.exactMatch(line)){
            QMessageBox::warning((QWidget*)parent(), "Corrupted file", "File is corrupted.");
            clear();
            return;
        }
        QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

        if (list.size()-5 < 0 || (list.size()-5)%2 != 0 || QString(list.at(4)).toInt() != (list.size()-5)/2) {
            QMessageBox::warning((QWidget*)parent(), "Corrupted file", "File is corrupted.");
            clear();
            return;
        }

        QString type = list.at(0);
        QString id = list.at(1);
        QString x = list.at(2);
        QString y = list.at(3);
        QString conns = list.at(4);

        Scene::BlockInfo block;
        block.type = Block::blockType(type.toInt());
        block.id = id.toInt();
        block.x = x.toInt();
        block.y = y.toInt();
        for (int i = 0; i < conns.toInt(); i++) {
            QString id = list.at(5 + 2*i);
            QString port = list.at(6 + 2*i);
            QPair<int, int> pair;
            pair.first = id.toInt();
            pair.second = port.toInt();
            block.connections.append(pair);
        }
        loadList.append(block);
    }

    clear();
    foreach (Scene::BlockInfo entry, loadList) {
        QPoint position = QPoint(entry.x, entry.y);
        createBlock(entry.type, position, entry.id);
    }
    scene->loadConnections(loadList);

    file.close();
}

void MainWindow::calculateNext()
{
    ensureModeIsSelect();
    if (scene->allCalculated()) {
        statusBar()->showMessage("Calculation complete. Restart to continue.", 2000);
        return;
    }
    if (!calculationReady())
        return;

    statusBar()->showMessage("Calculating next...", 2000);
    Block::calcError err = Block::NoErr;
    scene->calculateNext(&err);
    if (err) {
        statusBar()->showMessage("Error: Division by zero.", 2000);
        scene->setCalcComplete();
    }
}

void MainWindow::calculateAll()
{
    ensureModeIsSelect();
    if (scene->allCalculated()) {
        statusBar()->showMessage("Calculation complete. Restart to continue.", 2000);
        return;
    }
    if (!calculationReady())
        return;
    statusBar()->showMessage("Calculating all...", 2000);
    Block::calcError err = Block::NoErr;
    scene->calculateAll(&err);
    if (err) {
        statusBar()->showMessage("Error: Division by zero.", 2000);
        scene->setCalcComplete();
    }
}

void MainWindow::reset()
{
    ensureModeIsSelect();
    scene->resetCalculation();
}

bool MainWindow::calculationReady()
{
    if (scene->numberOfBlocks() == 0) {
        statusBar()->showMessage("Scheme does not contain any blocks to calculate.", 2000);
        return false;
    }
    if (!scene->allInputPortsConnected()) {
        statusBar()->showMessage("All input ports must be connected.", 2000);
        return false;
    }
    if (scene->containsLoops()) {
        statusBar()->showMessage("Scheme must not contain loops.", 2000);
        return false;
    }
    if (!scene->allInputBlocksInitialized()) {
        statusBar()->showMessage("All input blocks must be given a value.", 2000);
        return false;
    }
    return true;
}
