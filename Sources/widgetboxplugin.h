#ifndef WIDGETBOXPLUGIN_H
#define WIDGETBOXPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class WidgetBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
#endif // QT_VERSION >= 0x050000

public:
  WidgetBoxPlugin(QObject *parent = 0);

  bool isContainer() const;
  bool isInitialized() const;
  QIcon icon() const;
  QString domXml() const;
  QString group() const;
  QString includeFile() const;
  QString name() const;
  QString toolTip() const;
  QString whatsThis() const;
  QWidget *createWidget(QWidget *parent);
  void initialize(QDesignerFormEditorInterface *formEditor);

private slots:
    void currentIndexChanged(int index);
    void pageTitleChanged(const QString &title);

private:
  bool mInitialized;
};

#endif
