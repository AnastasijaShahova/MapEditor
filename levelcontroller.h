#ifndef LEVELCONTROLLER_H
#define LEVELCONTROLLER_H

#include <QObject>
#include "object.h"
#include "actor.h"

class LevelController : public QObject
{
    Q_OBJECT
public:
    LevelController(const std::vector<Object>& objects): m_objects(objects) {}

    void createNewLevel();
    void openExistedLevel();
    void saveLevel();
    void setActorsOnScene(const std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>> &actorsOnScene);
signals:
    void showLoadedActor( const QRectF coords, const std::optional<QString>& iconName, const std::optional<QString>& aliasName);
    void hideDestroyedActor(const QRectF coords);

private:
    std::vector<Object> m_objects;
    std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>> m_actorsOnScene;

};

#endif // LEVELCONTROLLER_H
