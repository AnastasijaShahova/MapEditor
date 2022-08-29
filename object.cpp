#include "object.h"

bool operator==(const Object &lhs, const Object &rhs)
{
    return lhs.m_name == rhs.m_name &&
            lhs.m_iconName == rhs.m_iconName &&
            lhs.m_singlePlaceable == rhs.m_singlePlaceable &&
                    lhs.m_properties == rhs.m_properties;
}

QString Object::name() const
{
    return m_name;
}

QString Object::iconName() const
{
    return m_iconName;
}

bool Object::singlePlaceable() const
{
    return m_singlePlaceable;
}

std::optional<Properties> Object::properties() const
{
    return m_properties;
}

void Object::setProperties(const std::optional<Properties> &properties)
{
    m_properties = properties;
}

bool Object::operator==(const Object &other)
{
    return m_name == other.m_name && m_iconName == other.m_iconName
            && m_singlePlaceable == other.m_singlePlaceable
            && m_properties == other.m_properties;
    ;
}
