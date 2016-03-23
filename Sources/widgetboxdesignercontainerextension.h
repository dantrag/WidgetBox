#ifndef WIDGETBOXDESIGNERCONTAINEREXTENSION_H
#define WIDGETBOXDESIGNERCONTAINEREXTENSION_H

#include <QtDesigner/QDesignerContainerExtension>

QT_BEGIN_NAMESPACE
class QExtensionManager;
QT_END_NAMESPACE
class WidgetBox;

class WidgetBoxDesignerContainerExtension : public QObject,
                                            public QDesignerContainerExtension
{
  Q_OBJECT
  Q_INTERFACES(QDesignerContainerExtension)

public:
  explicit WidgetBoxDesignerContainerExtension(WidgetBox *widget, QObject *parent);

  void addWidget(QWidget *widget) Q_DECL_OVERRIDE;
  int count() const Q_DECL_OVERRIDE;
  int currentIndex() const Q_DECL_OVERRIDE;
  void insertWidget(int index, QWidget *widget) Q_DECL_OVERRIDE;
  void remove(int index) Q_DECL_OVERRIDE;
  void setCurrentIndex(int index) Q_DECL_OVERRIDE;
  QWidget *widget(int index) const Q_DECL_OVERRIDE;

private:
  WidgetBox *mWidgetBox;
};

#endif // WIDGETBOXDESIGNERCONTAINEREXTENSION_H
