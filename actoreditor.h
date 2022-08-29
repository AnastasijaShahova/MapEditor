#ifndef ACTOREDITOR_H
#define ACTOREDITOR_H

#include "actor.h"
#include "property.h"
#include "reader.h"
#include "actorcontroll.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class ActorTypeWidget : public QWidget
{
    Q_OBJECT
public:
    ActorTypeWidget(const std::optional<Objects>& objects);

    bool hasValidType();
    QString currentText();
    void setCurrentIndex(const std::optional<Object>& object);
signals:
    void typeChanged(const std::optional<Object>& object);
    void currentIndexChanged();

private:
    QComboBox* m_comboBox;
    std::optional<Objects> m_objects;
};

class PropertyBaseWidget : public QWidget
{
    Q_OBJECT
public:
    virtual ~PropertyBaseWidget(){}

    virtual void init() = 0;
    virtual std::shared_ptr<Property> property() = 0;

public slots:
    virtual void onChangedComplete() = 0;
};

class LadderWidget : public PropertyBaseWidget
{
    Q_OBJECT
public:
    LadderWidget(
        const std::shared_ptr<LadderProperty>& property);

    void init() override;

    std::shared_ptr<Property> property() override
    {
        return std::static_pointer_cast<Property>(m_property);
    }
public slots:
    void onChangedComplete() override
    {
        m_property->setValue(m_spinBox->value());
    }

private:
    std::shared_ptr<LadderProperty> m_property;
    QDoubleSpinBox* m_spinBox;
};

class WallWidget : public PropertyBaseWidget
{
    Q_OBJECT
public:
    WallWidget(const std::shared_ptr<WallProperty>& property);

    void init() override;
    std::shared_ptr<Property> property() override
    {
        return std::static_pointer_cast<Property>(m_property);
    }
public slots:
    void onChangedComplete() override
    {
        m_property->setValue(m_comboBox->itemData(m_comboBox->currentIndex()).toBool());
    }

private:
    std::shared_ptr<WallProperty> m_property;
    QComboBox* m_comboBox;
};

template <typename PropertyWidget, typename PropertyType>
void createPropertyWidget(QVBoxLayout* layout, const std::shared_ptr<Property>& property,
                          std::vector<std::shared_ptr<PropertyBaseWidget>>& propertiesWidgets)
{
    auto propertyWidget = std::shared_ptr<PropertyBaseWidget>();
    propertyWidget.reset( new PropertyWidget(std::static_pointer_cast<PropertyType>(property)));
    propertyWidget->init();

    propertiesWidgets.emplace_back(propertyWidget);

    layout->addWidget(propertyWidget.get());
}

class ActorEditor : public QWidget
{
    Q_OBJECT

    void createPropertiesWidgets(const std::optional<Properties>& properties,
        QVBoxLayout* sublayout, std::vector<std::shared_ptr<PropertyBaseWidget>>& propertiesWidgets);

public:
    ActorEditor(
        const std::optional<Objects>& objects,
        std::shared_ptr<ActorControll> displayController);

public slots:
    void receiveActiveActor(const std::shared_ptr<Actor>& actor);
    void onActorTypeChanged(const std::optional<Object>& object);
    void saveObjectToActor();

private:
    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_propertiesLayout;
    ActorTypeWidget* m_actorTypeWidget = nullptr;
    QPushButton* m_saveButton = nullptr;
    std::optional<Objects> m_objects;
    std::shared_ptr<ActorControll> m_displayController;
    std::shared_ptr<Actor> m_activeActor;
    std::vector<std::shared_ptr<PropertyBaseWidget>> m_propertiesWidgets;
};

#endif // ACTOREDITOR_H
