#ifndef WIDGETBOX_H
#define WIDGETBOX_H

#include <QWidget>
#include <QPushButton>
#include <QTreeWidget>


class PageButton : public QPushButton
{
  Q_OBJECT
public:
  PageButton(const QString& text, QTreeWidget* parent, QTreeWidgetItem* item);
  bool isExpanded() const { return mItem->isExpanded(); }

public slots:
  void setTitle(QString const &title);
  void setExpanded(bool expanded) { mItem->setExpanded(expanded); }

private slots:
  void buttonPressed();

private:
  QTreeWidgetItem* mItem;
};


class WidgetBox : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
  Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle STORED true)
  Q_PROPERTY(bool isPageExpanded READ isPageExpanded WRITE setPageExpanded)

public:
  WidgetBox(QWidget *parent = 0);

#if defined(QT_PLUGIN)
  QSize sizeHint() const Q_DECL_OVERRIDE;
#endif

  int count() const { return mTreeWidget->topLevelItemCount(); }
  QWidget *page(int index) const;
  QWidget *widget(int index) const;
  QString pageTitle() const;
  bool isPageExpanded() const;
  int currentIndex() const;

public slots:
  void setCurrentIndex(int index);

  void addPage(QWidget *widget);
  void insertPage(int index, QWidget *widget);
  void removePage(int index);

  void setPageTitle(QString const &newTitle);
  void setPageExpanded(bool expanded);

protected:
  QTreeWidgetItem * addCategory(QString pageName);
  QTreeWidgetItem * insertCategory(int index, QString pageName);

  PageButton *categoryButton(int index) const;

  void createContainerWidget(QTreeWidgetItem* page, QWidget *widget);
  void createCategoryButton(QTreeWidgetItem* page, QString pageName);

protected slots:
  void onItemClicked(QTreeWidgetItem *item, int);

signals:
    void currentIndexChanged(int index);
    void pageTitleChanged(const QString &title);

private:
  bool checkIndex(int index) const;
  void setupWidget(QWidget *widget);
  int getPageIndex(QTreeWidgetItem *item);

  QTreeWidget *mTreeWidget;
};

#endif
