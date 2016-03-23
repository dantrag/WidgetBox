#ifndef WIDGETBOXEXTENSIONFACTORY_H
#define WIDGETBOXEXTENSIONFACTORY_H

#include <QtDesigner/QExtensionFactory>

class WidgetBoxExtensionFactory : public QExtensionFactory
{
  Q_OBJECT

public:
  WidgetBoxExtensionFactory(QExtensionManager *parent = 0);

protected:
  QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const Q_DECL_OVERRIDE;
};

#endif // WIDGETBOXEXTENSIONFACTORY_H
