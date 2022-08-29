#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include <QWidget>
#include "actorcontroll.h"
#include "actor.h"
#include "levelcontroller.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QPushButton>
#include <QStyleOptionGraphicsItem>
#include <QTextEdit>

#include <QWidget>

#include <QGraphicsGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QGraphicsRectItem>

#include <QLabel>
#include <QVBoxLayout>

class LevelView : public QWidget
{
    Q_OBJECT
public:
    LevelView(
        const std::shared_ptr<ActivityTracker>& tracker,
        const std::shared_ptr<QGraphicsScene>& scene,
        const std::shared_ptr<ActorControll>& displayController,
        const std::shared_ptr<LevelController>& levelController);

signals:
    void sendActiveActor(
        bool hasActivatedIndex, const std::shared_ptr<Actor>& actor);

public slots:
    void onActorActivated(const std::shared_ptr<Actor>& index);

private:
    std::vector<std::shared_ptr<Actor>> m_activatedActor;
    std::shared_ptr<ActivityTracker> m_tracker;
    std::shared_ptr<QGraphicsScene> m_scene;
    std::shared_ptr<ActorControll> m_displayController;
    std::shared_ptr<LevelController> m_levelController;

};

#endif // LEVELVIEW_H
