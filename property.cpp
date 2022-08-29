#include "property.h"

QString Property::name()
{
    return m_name;
}

QString Property::type()
{
    return m_type;
}

std::any Property::value()
{
    return m_value;
}

WallProperty::WallProperty(const QString &name) :
    Property(name, WALL_TYPE, false)
{
}

void WallProperty::setValue(const bool &value)
{
    m_value.reset();
    m_value = value;
}

void WallProperty::resetValue()
{
    m_value.reset();
    m_value = false;
}

std::shared_ptr<Property> WallProperty::clone()
{
    auto shared = std::make_shared<WallProperty>(*this);
    return std::static_pointer_cast<Property>(shared);
}

LadderProperty::LadderProperty(const QString &name, const Range<double> &range) :
    Property(name, LADDER_TYPE, std::any{}), m_range(range)
{
}

void LadderProperty::setValue(const double &value)
{
    m_value.reset();
    m_value = value;
}

void LadderProperty::resetValue()
{
    m_value.reset();
    m_value = m_range.m_min;
}

std::shared_ptr<Property> LadderProperty::clone()
{
    auto shared = std::make_shared<LadderProperty>(*this);
    return std::static_pointer_cast<Property>(shared);
}

std::shared_ptr<Property> CreateLadderProperty(const QJsonObject& object, const QString& name)
{
    if (object.contains("min") && object.contains("max"))
    {
        return std::make_shared<LadderProperty>(name,
            Range<double>(object.value("min").toDouble(), object.value("max").toDouble()));
    }

    else
    {
        return {};
    }
}

std::shared_ptr<Property> createPropertyFromJson(const QJsonObject &object)
{
    if (object.isEmpty())
    {
        return {};
    }

    else
    {
        if (object.contains("type") && object.contains("name"))
        {
            auto type = object.value("type");
            auto name = object.value("name");

            if (!type.isNull() && !name.isNull())
            {
                auto nameAsString = name.toString();

                if (type.toString() == WALL_TYPE)
                {
                    return std::make_shared<WallProperty>(nameAsString);
                }

                else if (type.toString() == LADDER_TYPE)
                {
                    return CreateLadderProperty(object, nameAsString);
                }
            }
        }
    }

    return {};
}
