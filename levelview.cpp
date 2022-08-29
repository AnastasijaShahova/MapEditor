#include "levelview.h"


LevelView::LevelView(const std::shared_ptr<ActivityTracker> &tracker, const std::shared_ptr<QGraphicsScene> &scene,
                     const std::shared_ptr<ActorControll> &displayController, const std::shared_ptr<LevelController> &levelController) :
  m_tracker(tracker),
  m_scene(scene),
  m_displayController(displayController),
  m_levelController(levelController)
{
    auto layout = new QVBoxLayout;
    auto view = new QGraphicsView;

    auto createSize = []() { return QSize{470, 320}; };

    auto viewSize = createSize();
    view->setFixedSize(viewSize);

    auto grid = new QGraphicsGridLayout;
    grid->setColumnFixedWidth(0, 30);
    grid->setRowFixedHeight(0, 30);

    auto mini = new QGraphicsWidget;
    mini->setMinimumHeight(30);
    mini->setMinimumWidth(30);

    mini->paint(new QPainter, new QStyleOptionGraphicsItem);
    grid->addItem(mini, 0, 1);

    QObject::connect(tracker.get(), &ActivityTracker::activated, this, &LevelView::onActorActivated);

    static auto ptr_map = std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>>{};

    for (auto i = 0; i < 10; ++i)
    {
        static auto a = 0.0;
        static auto b = 0.0;
        for (auto j = 0; j < 15; ++j)
        {

            static auto c = 30.0;
            static auto d = 30.0;

            auto icon = std::make_shared<QGraphicsPixmapItem>();
            auto alias = std::make_shared<QGraphicsTextItem>();

            auto actor = std::make_shared<Actor>( Position{i, j},
                QRectF{a, b, c, d},
                std::optional<Object>{std::nullopt},
                icon,
                alias,
                tracker);

            auto actorProxy = std::make_shared<ActorProxy>(actor);
            ptr_map[actorProxy] = actor;

            grid->addItem(actorProxy.get(), i, j);

            a += 30;
        }
        a = 0;
        b += 30;
    }

    m_displayController->setActorsScene(ptr_map);
    m_levelController->setActorsOnScene(ptr_map);

    m_scene->setSceneRect(0, 0, 450, 300);

    auto mainWindget = new QGraphicsWidget;
    mainWindget->setLayout(grid);

    m_scene->addItem(mainWindget);

    view->setScene(m_scene.get());
    layout->addWidget(view);

    setLayout(layout);

    QObject::connect(
        m_levelController.get(),
        &LevelController::showLoadedActor,
        m_displayController.get(),
        &ActorControll::showRepresentation);

    QObject::connect(
        m_levelController.get(),
        &LevelController::hideDestroyedActor,
        m_displayController.get(),
        &ActorControll::hideRepresentation);
}

void LevelView::onActorActivated(const std::shared_ptr<Actor> &index)
{
    if (index)
    {
        if (m_activatedActor.empty())
        {
            m_activatedActor.push_back(index);
        }

        else
        {
            if (m_activatedActor[0] != index)
            {
                m_activatedActor[0]->toggleActivated();
                m_activatedActor[0] = index;
            }

            else
            {
                m_activatedActor.pop_back();
            }

            if (!m_activatedActor.size())
            {
                emit m_tracker->activated(nullptr);
            }
        }
    }
}
