#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerPropertySheetExtension>

#include <QtPlugin>

#include "widgetbox.h"
#include "widgetboxplugin.h"
#include "widgetboxextensionfactory.h"


WidgetBoxPlugin::WidgetBoxPlugin(QObject *parent)
  : QObject(parent)
{
  mInitialized = false;
}

void WidgetBoxPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
  if (mInitialized)
    return;

  // Add extension registrations, etc. here
  QExtensionManager *manager = formEditor->extensionManager();
  QExtensionFactory *factory = new WidgetBoxExtensionFactory(manager);

  Q_ASSERT(manager != 0);
  manager->registerExtensions(factory, Q_TYPEID(QDesignerContainerExtension));

  mInitialized = true;
}

bool WidgetBoxPlugin::isInitialized() const
{
  return mInitialized;
}

QWidget *WidgetBoxPlugin::createWidget(QWidget *parent)
{
  WidgetBox *widget = new WidgetBox(parent);

  connect(widget, &WidgetBox::currentIndexChanged,
          this, &WidgetBoxPlugin::currentIndexChanged);
  connect(widget, &WidgetBox::pageTitleChanged,
          this, &WidgetBoxPlugin::pageTitleChanged);

  return widget;
}

QString WidgetBoxPlugin::name() const
{
  return QLatin1String("WidgetBox");
}

QString WidgetBoxPlugin::group() const
{
  return QLatin1String("Containers");
}

QIcon WidgetBoxPlugin::icon() const
{
  return QIcon(":/plugins/widgetbox/widgetbox.png");
}

QString WidgetBoxPlugin::toolTip() const
{
  return QLatin1String("Widget Box is a Qt widget which contains a list of "
                       "widgets (pages) separated by categories");
}

QString WidgetBoxPlugin::whatsThis() const
{
  return QLatin1String("Widget similar to the Widget Box in the Qt Designer. "
                       "It contains a list of widgets (pages) separated by "
                       "categories. Each category button can be clicked in "
                       "order to expand and collapse the list below the button.");
}

bool WidgetBoxPlugin::isContainer() const
{
  return true;
}

QString WidgetBoxPlugin::domXml() const
{
  return QLatin1String("\
  <ui language=\"c++\">\
    <widget class=\"WidgetBox\" name=\"widgetBox\">\
      <property name=\"geometry\">\
        <rect>\
          <x>0</x>\
          <y>0</y>\
          <width>130</width>\
          <height>210</height>\
        </rect>\
      </property>\
    </widget>\
    <customwidgets>\
      <customwidget>\
        <class>WidgetBox</class>\
        <extends>QWidget</extends>\
        <addpagemethod>addPage</addpagemethod>\
        </customwidget>\
    </customwidgets>\
  </ui>");
}

QString WidgetBoxPlugin::includeFile() const
{
  return QLatin1String("widgetbox.h");
}

void WidgetBoxPlugin::currentIndexChanged(int index)
{
  Q_UNUSED(index);
  WidgetBox *widget = qobject_cast<WidgetBox*>(sender());

  if(widget)
  {
    QDesignerFormWindowInterface *form = QDesignerFormWindowInterface::findFormWindow(widget);
    if(form)
      form->emitSelectionChanged();
  }
}

void WidgetBoxPlugin::pageTitleChanged(const QString &title)
{
  Q_UNUSED(title);
  WidgetBox *widget = qobject_cast<WidgetBox*>(sender());

  if(widget)
  {
    QWidget *page = widget->widget(widget->currentIndex());
    QDesignerFormWindowInterface *form;
    form = QDesignerFormWindowInterface::findFormWindow(widget);

    if(form)
    {
      QDesignerFormEditorInterface *editor = form->core();
      QExtensionManager *manager = editor->extensionManager();

      QDesignerPropertySheetExtension *sheet;
      sheet = qt_extension<QDesignerPropertySheetExtension*>(manager, page);
      const int propertyIndex = sheet->indexOf(QLatin1String("windowTitle"));
      sheet->setChanged(propertyIndex, true);
    }
  }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(WidgetBoxPlugin, WidgetBoxPlugin)
#endif // QT_VERSION < 0x050000
