#ifndef TRACKER_H
#define TRACKER_H

#include <QObject>

#include <memory>

class Actor;

class ActivityTracker: public QObject
{
    Q_OBJECT
signals:
    void activated(const std::shared_ptr<Actor>& actor);
};

#endif // TRACKER_H
