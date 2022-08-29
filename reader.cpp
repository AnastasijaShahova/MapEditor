#include "reader.h"
#include <QFile>
#include <QJsonArray>
#include "QtCore/qjsondocument.h"

void Reader::readFromFile(const QString &fileName)
{
    auto file = QFile(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto fileToString = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileToString);
    QJsonObject set = doc.object();
    qWarning() << set;
    QJsonValue value = set.value(QString("appName"));
    qWarning() << value;
    QJsonObject item = value.toObject();
}

std::optional<Properties> Reader::readTypeProperties(const QJsonObject &object)
{
    if (object.contains("properties") && !object.value("properties").isNull())
    {
        auto properties = object.value("properties").toArray();
        if(!properties.empty())
        {
            auto result = Properties{};

            for (const auto &property: properties)
            {
                result.push_back(createPropertyFromJson(property.toObject()));
            }

            return result;
        }
    }

    return {};
}

Objects Reader::readObjectsFromJson(const QJsonObject &object)
{
    if (object.contains("types"))
    {
        auto types = object.value("types").toArray();
        if(!types.empty())
        {
            auto result = Objects{};

            for (const auto &type: types)
            {
                auto typeAsObject = type.toObject();
                if (!typeAsObject.isEmpty() && typeAsObject.contains("name")
                    && typeAsObject.contains("iconName") && typeAsObject.contains("singlePlaceable")
                    && !typeAsObject.value("name").isNull()
                    && !typeAsObject.value("iconName").isNull() && !typeAsObject.value("singlePlaceable").isNull())
                {
                    auto name = typeAsObject.value("name").toString();
                    auto iconName = typeAsObject.value("iconName").toString();
                    auto singlePlaceable = typeAsObject.value("singlePlaceable").toBool();
                    result.push_back( Object {name, iconName, singlePlaceable, readTypeProperties(typeAsObject) } );
                }
            }

            return result;
        }
    }

    return {};
}

QJsonObject Reader::createJsonObjectFromFile(const QString &fileName)
{
    try
    {
        auto file = QFile(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        auto fileToString = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(fileToString);
        auto object = doc.object();
        return object;
    }
    catch(std::exception& ex)
    {
        qDebug() << "Cannot read file: " << ex.what();
    }

    return {};
}
