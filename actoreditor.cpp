#include "actoreditor.h"

ActorTypeWidget::ActorTypeWidget(const std::optional<Objects> &objects) :
  m_objects(objects)
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Actor type: "));

    m_comboBox = new QComboBox;
    layout->addWidget(m_comboBox);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);


    m_comboBox->addItem("");
    if (objects)
    {
        for (const auto &object: objects.value())
        {
            m_comboBox->addItem(object.name());
        }
    }

    auto slot = [=](const QString &objectName) {
        auto predicate = [&objectName](const auto &element) {
            return objectName == element.name();
        };

        auto find = std::find_if(
            objects.value().begin(), objects.value().end(), predicate);

        auto objectToSend = find != objects.value().end() ?
                                std::optional<Object>{*find} :
                                std::nullopt;

        emit typeChanged(objectToSend);
    };

    QObject::connect(m_comboBox, &QComboBox::currentTextChanged, slot);
    QObject::connect(m_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ActorTypeWidget::currentIndexChanged);
}

bool ActorTypeWidget::hasValidType()
{
    return m_comboBox->currentIndex() != 0;
}

QString ActorTypeWidget::currentText()
{
    return m_comboBox->currentText();
}

void ActorTypeWidget::setCurrentIndex(const std::optional<Object> &object)
{
    m_comboBox->setCurrentIndex(object ? m_comboBox->findText(object.value().name()) : 0);
}

LadderWidget::LadderWidget(const std::shared_ptr<LadderProperty> &property) :
  m_property(property) {}

void LadderWidget::init()
{
    auto layout = new QHBoxLayout;

    if (m_property)
    {
        auto label = new QLabel(m_property->name());
        layout->addWidget(label);

        m_spinBox = new QDoubleSpinBox;
        auto min = m_property->range().m_min;
        auto max = m_property->range().m_max;
        m_spinBox->setRange(min, max);
        m_spinBox->setToolTip(QString{"From %1 to %2"}.arg(min, max));
        auto value = m_property->value();
        m_spinBox->setValue( value.has_value() ? std::any_cast<double>(value) : 0.0);
        layout->addWidget(m_spinBox);
    }

    setLayout(layout);
}

WallWidget::WallWidget(const std::shared_ptr<WallProperty> &property) :
  m_property(property){}

void WallWidget::init()
{
    auto layout = new QHBoxLayout;

    auto label = new QLabel(m_property->name());
    layout->addWidget(label);

    m_comboBox = new QComboBox;
    m_comboBox->addItem("true", QVariant{true});
    m_comboBox->addItem("false", QVariant{false});
    m_comboBox->setCurrentIndex(std::any_cast<bool>(m_property->value()) ? 0 : 1);
    layout->addWidget(m_comboBox);

    setLayout(layout);
}

void ActorEditor::createPropertiesWidgets(const std::optional<Properties> &properties,
    QVBoxLayout *sublayout, std::vector<std::shared_ptr<PropertyBaseWidget>> &propertiesWidgets)
{
    if (properties)
    {
        for (const auto &property: properties.value())
        {
            if (property->type() == WALL_TYPE)
            {
                createPropertyWidget<WallWidget, WallProperty>(sublayout, property, propertiesWidgets);
            }

            else if (property->type() == LADDER_TYPE)
            {
                createPropertyWidget<LadderWidget, LadderProperty>(sublayout, property, propertiesWidgets);
            }
        }
    }
}

ActorEditor::ActorEditor(const std::optional<Objects> &objects, const std::shared_ptr<ActorControll> displayController) :
  m_objects(objects), m_displayController(displayController)
{
    m_actorTypeWidget = new ActorTypeWidget(objects);
    m_actorTypeWidget->hide();

    m_propertiesLayout = new QVBoxLayout;

    QObject::connect( m_actorTypeWidget, &ActorTypeWidget::typeChanged, this, &ActorEditor::onActorTypeChanged);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->addWidget(m_actorTypeWidget);

    auto propertiesLabel = new QLabel("Actor properties: ");
    m_mainLayout->addWidget(propertiesLabel);
    m_mainLayout->setAlignment(propertiesLabel, Qt::AlignHCenter);

    if (!m_actorTypeWidget->hasValidType())
    {
        propertiesLabel->hide();
    }

    QObject::connect( m_actorTypeWidget, &ActorTypeWidget::currentIndexChanged, this,
        [=]() {
            m_actorTypeWidget->hasValidType() ? propertiesLabel->show() :
                                                propertiesLabel->hide();
        });

    m_mainLayout->addLayout(m_propertiesLayout);
    m_mainLayout->setAlignment(m_actorTypeWidget, Qt::AlignTop);
    m_mainLayout->setAlignment(m_propertiesLayout, Qt::AlignTop);

    setLayout(m_mainLayout);
}

void ActorEditor::receiveActiveActor(const std::shared_ptr<Actor> &actor)
{
    m_activeActor = actor;

    if (actor)
    {
        auto object = actor->object();
        if (!m_actorTypeWidget)
        {
            m_actorTypeWidget = new ActorTypeWidget(m_objects);
            m_mainLayout->addWidget(m_actorTypeWidget);
        }

        else
        {
            m_actorTypeWidget->show();
        }

        m_actorTypeWidget->setCurrentIndex(object);

        if (!m_saveButton)
        {
            m_mainLayout->addItem(new QSpacerItem(0, 300));
            m_saveButton = new QPushButton("Save changes", m_actorTypeWidget);
            m_mainLayout->addWidget(m_saveButton);

            QObject::connect(m_saveButton, &QPushButton::clicked, this, [this]() {
                    if (m_actorTypeWidget->hasValidType())
                    {
                        auto it = std::find_if(
                            m_objects.value().begin(),
                            m_objects.value().end(),
                            [this](const auto &element) {
                                return m_actorTypeWidget->currentText() == element.name();
                            });

                        if (it != m_objects.value().end())
                        {
                            m_displayController->showRepresentation(
                                m_activeActor->coords(),
                                it->iconName(),
                                it->name());
                        }
                    }
                    else
                    {
                        m_displayController->hideRepresentation(m_activeActor->coords());
                    }
                });

            QObject::connect(
                m_saveButton,
                &QPushButton::clicked,
                this,
                &ActorEditor::saveObjectToActor);
        }

        if (m_propertiesLayout->count())
        {
            for (auto counter = 0; counter < m_propertiesLayout->count();
                 ++counter)
            {
                m_propertiesLayout->removeItem(
                    m_propertiesLayout->itemAt(counter));
            }
            m_propertiesWidgets.clear();
        }

        if (object && object.value().properties())
        {
            createPropertiesWidgets(
                object.value().properties(),
                m_propertiesLayout,
                m_propertiesWidgets);
        }
    }
}

void ActorEditor::saveObjectToActor()
{
    if (m_objects)
    {
        auto it = std::find_if(
            m_objects.value().begin(),
            m_objects.value().end(),
            [this](const auto &element) {
                return m_actorTypeWidget->currentText() == element.name();
            });

        if (it != m_objects.value().end())
        {
            auto object = *it;
            auto properties = Properties{};

            for (const auto &propertyWidget: m_propertiesWidgets)
            {
                auto property = propertyWidget->property();
                propertyWidget->onChangedComplete();
                properties.push_back(property->clone());
                property->resetValue();
            }
            object.setProperties(properties.size() ? std::make_optional(properties) : std::nullopt);

            if (object.singlePlaceable())
            {
                auto actorsOnScene = m_displayController->actorsOnScene();
                auto find = std::find_if(
                    actorsOnScene.begin(),
                    actorsOnScene.end(),
                    [](const auto &element) {
                        auto object = element.second->object();
                        if (object)
                        {
                            return object.value().singlePlaceable();
                        }
                        return false;
                    });

                if (find != actorsOnScene.end())
                {
                    find->second->setObject(std::nullopt);
                    m_displayController->hideRepresentation( find->second->coords());
                }
            }

            m_activeActor->setObject(object);
        }
    }
}

void ActorEditor::onActorTypeChanged(const std::optional<Object> &object)
{
    if (m_propertiesLayout->count())
    {
        for (auto counter = 0; counter < m_propertiesLayout->count(); ++counter)
        {
            m_propertiesLayout->removeItem(m_propertiesLayout->itemAt(counter));
        }

        m_propertiesWidgets.clear();
    }

    if (object && object.value().properties())
    {
        createPropertiesWidgets(
            object.value().properties(),
            m_propertiesLayout,
            m_propertiesWidgets);
    }
}
