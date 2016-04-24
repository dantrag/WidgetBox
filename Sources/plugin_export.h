#ifndef PLUGIN_EXPORT_H
#define PLUGIN_EXPORT_H

#include <QtUiPlugin/QDesignerExportWidget>

#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerPropertySheetExtension>
#include <QtDesigner/QExtensionManager>

#ifdef QT_PLUGIN
#define PLUGIN_EXPORT QDESIGNER_WIDGET_EXPORT
#else
#define PLUGIN_EXPORT
#endif // QT_PLUGIN

#endif // PLUGIN_EXPORT_H
