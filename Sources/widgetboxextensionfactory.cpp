#include "widgetbox.h"
#include "widgetboxextensionfactory.h"
#include "widgetboxdesignercontainerextension.h"


WidgetBoxExtensionFactory::WidgetBoxExtensionFactory(QExtensionManager *parent)
  : QExtensionFactory(parent)
{

}

QObject *WidgetBoxExtensionFactory::createExtension(QObject *object,
                                                    const QString &iid,
                                                    QObject *parent) const
{
  WidgetBox *widget = qobject_cast<WidgetBox*>(object);

  if (widget && (iid == Q_TYPEID(QDesignerContainerExtension)))
  {
    return new WidgetBoxDesignerContainerExtension(widget, parent);
  }
  else
  {
    return 0;
  }
}
