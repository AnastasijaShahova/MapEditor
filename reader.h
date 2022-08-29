#ifndef READER_H
#define READER_H

#include "object.h"
#include "property.h"

using Properties = std::vector<std::shared_ptr<Property>>;
using Objects = std::vector<Object>;

class Reader
{
public:
    void readFromFile(const QString& fileName);

    std::optional<Properties> readTypeProperties(const QJsonObject& object);

    Objects readObjectsFromJson(const QJsonObject& object);

    QJsonObject createJsonObjectFromFile(const QString& fileName);
};

#endif // READER_H
