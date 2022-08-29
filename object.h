#ifndef OBJECT_H
#define OBJECT_H

#include "property.h"

using Properties = std::vector<std::shared_ptr<Property>>;

class Object
{
public:
    Object(QString name, QString iconName, bool singlePlaceable, std::optional<Properties> properties = std::nullopt) :
      m_name(name),
      m_iconName(iconName),
      m_singlePlaceable(singlePlaceable),
      m_properties(properties) {}

    QString name() const;
    QString iconName() const;
    bool singlePlaceable() const;
    std::optional<Properties> properties() const;

    void setProperties(const std::optional<Properties>& properties);
    bool operator==(const Object& other);
    friend bool operator==(const Object& lhs, const Object& rhs);

private:
    QString m_name;
    QString m_iconName;
    bool m_singlePlaceable;
    std::optional<Properties> m_properties;
};

#endif // OBJECT_H
