#ifndef WIDGETBOX_H
#define WIDGETBOX_H

#include <QWidget>
#include <QPushButton>
#include <QTreeWidget>
#include "plugin_export.h"

class AbstractCategory;

class PLUGIN_EXPORT WidgetBox : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
  Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle STORED true)
  Q_PROPERTY(bool isPageExpanded READ isPageExpanded WRITE setPageExpanded)
  Q_PROPERTY(CategoryType categoryType READ categoryType WRITE setCategoryType)
  Q_ENUMS(CategoryType)

public:
  enum CategoryType {
    Button,
    Line,
    Frame   // TODO
  };

  WidgetBox(QWidget *parent = 0);
  ~WidgetBox();

  CategoryType categoryType() const             { return mCategoryType; }
  void setCategoryType(const CategoryType type);

#if defined(QT_PLUGIN)
  QSize sizeHint() const Q_DECL_OVERRIDE;
#endif

  int count() const { return mTreeWidget->topLevelItemCount(); }
  QWidget *page(int index) const;
  QWidget *widget(int index) const;
  QString pageTitle() const;
  QString pageTitle(int index) const;
  bool isPageExpanded() const;
  bool isPageExpanded(int index) const;
  int currentIndex() const;

public slots:
  void setCurrentIndex(int index);

  void addPage(QWidget *widget);
  void insertPage(int index, QWidget *widget);
  void removePage(int index);

  void setPageTitle(QString const &newTitle);
  void setPageExpanded(bool expanded);
  void setPageExpanded(int index, bool expanded);

protected:
  QTreeWidgetItem * addCategory(QString pageName);
  QTreeWidgetItem * insertCategory(int index, QString pageName);

  AbstractCategory *category(int index) const;

  void createContainerWidget(QTreeWidgetItem* page, QWidget *widget);
  void createCategory(QTreeWidgetItem* page, QString pageName);

  void changeQtDesignerProperty(QString propertyName, QVariant value,
                                bool markChangedOnly = false);

protected slots:
  void onItemClicked(QTreeWidgetItem *item, int);
  void setPageExpandedProperty();

signals:
    void currentIndexChanged(int index);
    void pageTitleChanged(const QString &title);

private:
  bool checkIndex(int index) const;
  void setupWidget(QWidget *widget);
  int getPageIndex(QTreeWidgetItem *item);
  void recreateCategories();

  QTreeWidget *mTreeWidget;
  CategoryType mCategoryType = Button;
  // Necessary only in Qt Designer
  QDesignerPropertySheetExtension *mSheet = 0;
};

#endif
