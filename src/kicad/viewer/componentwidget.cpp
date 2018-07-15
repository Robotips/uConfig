#include "componentwidget.h"

#include <QLabel>
#include <QLayout>

ComponentWidget::ComponentWidget(QWidget *parent)
    : QWidget(parent)
{
    _component = Q_NULLPTR;
    createWidgets();
}

Component *ComponentWidget::component() const
{
    return _component;
}

void ComponentWidget::setComponent(Component *component)
{
    _component = component;
    _comboUnit->clear();

    if (_component)
    {
        char c = 'A';
        for (int i=0; i<_component->unitCount(); i++,c++)
            _comboUnit->addItem(QString("Unit ") + c);
        _comboUnit->setEnabled(_component->unitCount() != 1);
    }

    _viewer->setComponent(_component);
}

void ComponentWidget::createWidgets()
{
    _viewer = new ComponentViewer();

    QLayout *_layout = new QVBoxLayout();
    _layout->setMargin(0);
    QLayout *_toolsLayout = new QHBoxLayout();

    _toolGrid = new QToolButton();
    _toolGrid->setText("grid");
    _toolGrid->setCheckable(true);
    _toolGrid->setChecked(true);
    _toolGrid->setMinimumSize(30, 30);
    _toolsLayout->addWidget(_toolGrid);
    connect(_toolGrid, &QToolButton::toggled, _viewer, &ComponentViewer::setGridVisible);

    _toolElecType = new QToolButton();
    _toolElecType->setText("elec type");
    _toolElecType->setCheckable(true);
    _toolElecType->setChecked(true);
    _toolElecType->setMinimumSize(30, 30);
    _toolsLayout->addWidget(_toolElecType);
    connect(_toolElecType, &QToolButton::toggled, _viewer, &ComponentViewer::setElecTypeVisible);

    _toolsLayout->addItem(new QSpacerItem(5, 5));
    _toolsLayout->addWidget(new QLabel("Unit: "));
    _comboUnit = new QComboBox();
    _toolsLayout->addWidget(_comboUnit);
    connect(_comboUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(setUnit(int)));

    _toolsLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));

    _layout->addItem(_toolsLayout);
    _layout->addWidget(_viewer);

    setLayout(_layout);
}

ComponentViewer *ComponentWidget::viewer() const
{
    return _viewer;
}

void ComponentWidget::setUnit(int unit)
{
    _viewer->setComponent(_component, unit+1);
}
