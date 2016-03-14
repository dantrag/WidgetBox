#ifndef WIDGETBOX_H
#define WIDGETBOX_H

#include <QWidget>
#include <QPushButton>
#include <QTreeWidget>

/**
 * @brief The PageButton class: page (category) button for widget box
 */
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

/**
 * @brief The WidgetBox class: Widget similar to the Widget Box in the Qt Designer.
  It contains a list of widgets (pages) separated by categories. Each category
  button can be clicked in order to expand and collapse the list below the button.
 */
class WidgetBox : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
  Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle)
  Q_PROPERTY(bool isPageExpanded READ isPageExpanded WRITE setPageExpanded)

public:
  WidgetBox(QWidget *parent = 0);

  QSize sizeHint() const Q_DECL_OVERRIDE;

  int count() const { return mTreeWidget->topLevelItemCount(); }
  QWidget *page(int index) const;
  QWidget *widget(int index) const;
  QString pageTitle() const;
  bool isPageExpanded() const;

public slots:
  void setCurrentIndex(int index);

  void addPage(QWidget *widget);
  void insertPage(int index, QWidget *widget);
  void removePage(int index);
  int currentIndex() const;

  void setPageTitle(QString const &newTitle);
  void setPageExpanded(bool expanded);

protected:
  QTreeWidgetItem * addCategory(QString pageName);
  QTreeWidgetItem * insertCategory(int index, QString pageName);

  PageButton *categoryButton(int index) const;

  void createContainerWidget(QTreeWidgetItem* page, QWidget *widget);
  void createCategoryButton(QTreeWidgetItem* page, QString pageName);

signals:
    void currentIndexChanged(int index);
    void pageTitleChanged(const QString &title);

private:
  bool checkIndex(int index) const;
  void setupWidget(QWidget *widget);

  QTreeWidget *mTreeWidget;
};

#endif
