#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "levelcontroller.h"
#include "reader.h"
#include "actorcontroll.h"
#include "actoreditor.h"
#include "levelview.h"

#include <QMenu>
#include <QGraphicsScene>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>

constexpr auto OBJECTS_FILE_NAME = "/Users/sahovaanastasia/GameEditor/objects.json";


Objects readObjectsFromConfig()
{
    Reader reader{};
    auto object = reader.createJsonObjectFromFile(OBJECTS_FILE_NAME);
    auto types = reader.readObjectsFromJson(object);

    return types;
}

QWidget* CreateMainWindget(std::shared_ptr<LevelController>& levelController, const Objects& types)
{
    auto scene = std::make_shared<QGraphicsScene>();
    auto displayController = std::make_shared<ActorControll>(scene);
    auto actorEditor = new ActorEditor(types, displayController);

    auto levelViewLayout = new QHBoxLayout;

   auto tracker = std::shared_ptr<ActivityTracker>(new ActivityTracker);

   auto levelView = new LevelView(tracker, scene, displayController, levelController);

    levelViewLayout->addWidget(levelView);
    levelViewLayout->addWidget(new QSplitter);
    levelViewLayout->addWidget(actorEditor);

    QObject::connect(tracker.get(), &ActivityTracker::activated, actorEditor, &ActorEditor::receiveActiveActor);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addItem(levelViewLayout);
    mainLayout->setAlignment(Qt::AlignHCenter);
    auto widget = new QWidget;
    widget->setLayout(mainLayout);

    return widget;
}


MainWindow::MainWindow() :
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto types = readObjectsFromConfig();
    auto levelController = std::make_shared<LevelController>(types);

     QToolBar *fileToolBar = addToolBar(tr("File"));
     fileToolBar->addAction(ui->actionnew);
     fileToolBar->addAction(ui->actionopen);
     fileToolBar->addAction(ui->actionsave);
     ui->actionnew->setShortcut(QKeySequence::New);
     ui->actionnew->setStatusTip(tr("Create a new level"));
     connect(ui->actionnew, &QAction::triggered, levelController.get(), &LevelController::createNewLevel);

     ui->actionopen->setShortcuts(QKeySequence::Open);
     ui->actionopen->setStatusTip(tr("Open an existing level"));
     connect(ui->actionopen, &QAction::triggered, levelController.get(), &LevelController::openExistedLevel);

     ui->actionsave->setShortcuts(QKeySequence::Save);
     ui->actionsave->setStatusTip(tr("Save an existing level"));
     connect(ui->actionsave, &QAction::triggered, levelController.get(), &LevelController::saveLevel);


     auto mainWidget = CreateMainWindget(levelController, types);
     setCentralWidget(mainWidget);

     setFixedSize(700, 410);

}

MainWindow::~MainWindow()
{
    delete ui;
}

