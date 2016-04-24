#ifndef PAGEEVENTFILTER_H
#define PAGEEVENTFILTER_H

#include <QObject>

class QTreeWidgetItem;

class PageEventFilter : public QObject
{
  Q_OBJECT
public:
  PageEventFilter(QObject *parent, QTreeWidgetItem *item);

protected:
  bool eventFilter(QObject *obj, QEvent *event);

signals:
  void itemClicked(QTreeWidgetItem *item, int column);

private:
  QTreeWidgetItem *mItem;
};


class PageResizeFilter : public QObject
{
  Q_OBJECT
public:
  PageResizeFilter(QObject *parent, QTreeWidgetItem *item);

protected:
  bool eventFilter(QObject *obj, QEvent *event);

private:
  QTreeWidgetItem *mItem;
};

#endif // PAGEEVENTFILTER_H
