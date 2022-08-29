#ifndef PROPERTY_H
#define PROPERTY_H

#include <QJsonObject>
#include <QString>

#include <any>

enum class typeProperty
{
    WallType,
    LadderType
};

template <typename T>
struct Range
{
    T m_min;
    T m_max;

    Range(){}
    Range(double min, double max) : m_min(std::move(min)), m_max(std::move(max)) {}
};

const auto WALL_TYPE = "boolean";
const auto LADDER_TYPE = "ranged";


class Property
{
public:
    Property(const QString& name, const QString& type, const std::any& value) :
      m_name(name), m_type(type), m_value(value) {}

protected:
    QString m_name;
    QString m_type;
    std::any m_value;

public:
    virtual void resetValue() = 0;

    virtual std::shared_ptr<Property> clone() = 0;

    QString name();
    QString type();
    std::any value();
};

class WallProperty : public Property
{
public:
    WallProperty(const QString& name);
    virtual ~WallProperty() {};

    void setValue(const bool& value);
    void resetValue() override;
    std::shared_ptr<Property> clone() override;
};

class LadderProperty : public Property
{
public:
    LadderProperty(const QString& name, const Range<double>& range);
    virtual ~LadderProperty() {};

public:
    void setValue(const double& value);
    void resetValue() override;
    std::shared_ptr<Property> clone() override;
    Range<double> range(){ return m_range; }

private:
    Range<double> m_range;
};

std::shared_ptr<Property> createPropertyFromJson(const QJsonObject &object);


#endif // PROPERTY_H
