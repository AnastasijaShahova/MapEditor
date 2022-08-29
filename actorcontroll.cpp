#include "actorcontroll.h"

QString coordsToId(const QRectF& rect)
{
    return QString("%1,%2,%3,%4")
        .arg(rect.x())
        .arg(rect.y())
        .arg(rect.width())
        .arg(rect.height());
}

void showActorOnLevel(const std::shared_ptr<QGraphicsScene> scene, ActorsDisplayStorage& storage,   const QRectF coords,
        const std::optional<QString>& iconName,
        const std::optional<QString>& aliasName, const QString& id)
{
    auto data = ActorDisplayData{};
    if (iconName)
    {
        QImage image(iconName.value());
        image = image.scaled({30, 30});

        auto icon = std::make_shared<QGraphicsPixmapItem>();
        icon->setPixmap(QPixmap::fromImage(image));
        scene->addItem(icon.get());
        icon->setPos(coords.center().x() - 15, coords.center().y() - 15);

        data.m_icon = IconData{iconName, icon};
        data.m_alias = AliasData{aliasName, nullptr};
    }
    else if (aliasName)
    {
        auto alias = std::make_shared<QGraphicsTextItem>();
        alias->setPlainText(aliasName.value());
        alias->setDefaultTextColor(QColor("Red"));
        scene->addItem(alias.get());
        alias->setPos(coords.center().x() - 15, coords.center().y() - 15);

        data.m_icon = IconData{iconName, nullptr};
        data.m_alias = AliasData{aliasName, alias};
    }
    storage[id] = data;
}

ActorControll::ActorControll(
    const std::shared_ptr<QGraphicsScene>& scene) :
  m_scene(scene)
{
}

void ActorControll::setActorsScene(const std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor> >& actorsOnScene)
{
    m_actorsOnScene = actorsOnScene;
}

std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>>ActorControll::actorsOnScene()
{
    return m_actorsOnScene;
}

void ActorControll::showRepresentation(const QRectF coords, const std::optional<QString>& iconName, const std::optional<QString>& aliasName)
{
    auto id = coordsToId(coords);
    auto find = m_storage.find(id);

    if (find != m_storage.end())
    {
        hideRepresentation(coords);
    }
    showActorOnLevel(m_scene, m_storage, coords, iconName, aliasName, id);
}

void ActorControll::hideRepresentation(const QRectF& coords)
{
    auto find = m_storage.find(coordsToId(coords));

    if (find != m_storage.end())
    {
        auto iconPtr = find->second.m_icon.second;
        auto aliasPtr = find->second.m_alias.second;

        if (iconPtr)
        {
            m_scene->removeItem(iconPtr.get());
            iconPtr.reset();
        }
        else if (aliasPtr)
        {
            m_scene->removeItem(aliasPtr.get());
            aliasPtr.reset();
        }
        m_storage.erase(find);
    }
}
