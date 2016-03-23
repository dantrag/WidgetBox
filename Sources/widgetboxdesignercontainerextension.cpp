#include "widgetbox.h"
#include "widgetboxdesignercontainerextension.h"


WidgetBoxDesignerContainerExtension::WidgetBoxDesignerContainerExtension(WidgetBox *widget,
                                                                         QObject *parent)
  : QObject(parent)
  , mWidgetBox(widget)
{
}

void WidgetBoxDesignerContainerExtension::addWidget(QWidget *widget)
{
  mWidgetBox->addPage(widget);
}

int WidgetBoxDesignerContainerExtension::count() const
{
  return mWidgetBox->count();
}

int WidgetBoxDesignerContainerExtension::currentIndex() const
{
  return mWidgetBox->currentIndex();
}

void WidgetBoxDesignerContainerExtension::insertWidget(int index, QWidget *widget)
{
  mWidgetBox->insertPage(index, widget);
}

void WidgetBoxDesignerContainerExtension::remove(int index)
{
  mWidgetBox->removePage(index);
}

void WidgetBoxDesignerContainerExtension::setCurrentIndex(int index)
{
  mWidgetBox->setCurrentIndex(index);
}

QWidget* WidgetBoxDesignerContainerExtension::widget(int index) const
{
  return mWidgetBox->widget(index);
}
