CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(WidgetBoxPlugin)
TEMPLATE    = lib

HEADERS     = widgetboxplugin.h widgetbox.h \
              widgetboxextensionfactory.h \
              widgetboxdesignercontainerextension.h \
              CategoryWidgets.h

SOURCES     = widgetboxplugin.cpp widgetbox.cpp \
              widgetboxextensionfactory.cpp \
              widgetboxdesignercontainerextension.cpp \
              CategoryWidgets.cpp

RESOURCES   = icons.qrc
LIBS        += -L.

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
creator_target.path = $$[QTCREATOR_BIN_PATH]/plugins/designer
INSTALLS += target creator_target
