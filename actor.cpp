#include "actor.h"
#include <QGraphicsScene>

Actor::Actor(const Position &position, const QRectF &coords, const std::optional<Object> &object,
    const std::shared_ptr<QGraphicsPixmapItem> &icon, const std::shared_ptr<QGraphicsTextItem> &alias, const std::shared_ptr<ActivityTracker> &tracker) :
  QGraphicsRectItem(coords),
  m_position(position),
  m_coords(coords),
  m_object(object),
  m_icon(icon),
  m_alias(alias),
  m_tracker(tracker),
  m_isActive(false)
{}

void Actor::toggleActivated()
{
    m_isActive = !m_isActive;
    static auto pen = QPen(Qt::yellow, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    static auto blackPen = QPen(QColor("Black"));
    setPen(m_isActive ? pen : blackPen);
}

Position Actor::position()
{
    return m_position;
}

QRectF Actor::coords()
{
    return m_coords;
}

std::optional<Object> Actor::object()
{
    return m_object;
}

void Actor::setObject(const std::optional<Object> &object)
{
    m_object = object;
}

bool operator==(const Actor &actor, const Actor &other)
{
    return actor.m_position == other.m_position
           && actor.m_coords == other.m_coords
           && actor.m_object == other.m_object && actor.m_icon == other.m_icon
           && actor.m_alias == other.m_alias;
}

void Actor::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    toggleActivated();
    m_tracker->activated(this->shared_from_this());
}

ActorProxy::ActorProxy(const std::shared_ptr<Actor> actor) : m_actor(actor)
{
    setGraphicsItem(m_actor.get());
}

std::shared_ptr<Actor> ActorProxy::value()
{
    return m_actor;
}

bool operator==(const Position &position, const Position &other)
{
    return position.m_x == other.m_x && position.m_y == other.m_y;
}
