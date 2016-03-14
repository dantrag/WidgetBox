#include <QDebug>
#include <QBoxLayout>
#include <QHeaderView>
#include "widgetbox.h"


/**
 * @class PageButton
 * @brief The PageButton class: page (category) button for widget box
 */
PageButton::PageButton(const QString &text, QTreeWidget* parent,
                       QTreeWidgetItem *item)
  : QPushButton(text, parent)
  , mItem(item)
{
  mItem->setExpanded(true);
  setIcon(QIcon(":/plugins/widgetbox/expanded.png"));
  // setFlat(true);

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  QFontMetrics fm(font());
  resize(size().width(), fm.height());

  connect(this, SIGNAL(pressed()), this, SLOT(buttonPressed()));
}

void PageButton::setTitle(QString const &title)
{
  setText(title);
}

void PageButton::buttonPressed()
{
  mItem->setExpanded(!mItem->isExpanded());
  int index = mItem->treeWidget()->indexOfTopLevelItem(mItem);
  ((WidgetBox *)mItem->treeWidget()->parent())->setCurrentIndex(index);

  if(mItem->isExpanded()) {
    setIcon(QIcon(":/plugins/widgetbox/expanded.png"));
  } else {
    setIcon(QIcon(":/plugins/widgetbox/collapsed.png"));
  }
}

/*!
 * \class WidgetBox
 * \brief The WidgetBox class: Widget similar to the Widget Box in the Qt Designer.
 * It contains a list of widgets (pages) separated by categories. Each category
 * button can be clicked in order to expand and collapse the list below the button.
 *
*/

WidgetBox::WidgetBox(QWidget *parent) : QWidget(parent)
{
  // Set default tree widget settings
  mTreeWidget = new QTreeWidget(this);
  mTreeWidget->setFrameStyle(QFrame::NoFrame);
  mTreeWidget->setAnimated(true);
  mTreeWidget->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);

  // Make tree widget same color as normal widget
  QPalette pal = mTreeWidget->palette();
  pal.setColor(QPalette::Base, palette().background().color());
  mTreeWidget->setPalette(pal);

  mTreeWidget->header()->hide();
  mTreeWidget->setRootIsDecorated(false);
  mTreeWidget->setIndentation(0);

  // Create WidgetBox layout
  QBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mTreeWidget);

  connect(mTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
          SLOT(onItemClicked(QTreeWidgetItem*,int)));
  connect(mTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
          SLOT(onItemClicked(QTreeWidgetItem*,int)));
}

QSize WidgetBox::sizeHint() const
{
  return QSize(130, 210);
}

void WidgetBox::createCategoryButton(QTreeWidgetItem* page, QString pageName)
{
  PageButton *button = new PageButton(pageName, mTreeWidget, page);
  mTreeWidget->setItemWidget(page, 0, button);
}

QTreeWidgetItem * WidgetBox::addCategory(QString pageName)
{
  return insertCategory(count(), pageName);
}

/**
 * @brief WidgetBox::setupWidget set default widget properties
 * @param widget
 */
void WidgetBox::setupWidget(QWidget *widget)
{
  // The given widget's autoFillBackground property must be set to true,
  // otherwise the widget's background will be transparent,
  // showing both the model data and the tree widget item.
  widget->setAutoFillBackground(true);
  widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
  widget->setStyle(style());  // Set parent widget style
}

void WidgetBox::createContainerWidget(QTreeWidgetItem* page, QWidget *widget)
{
  QTreeWidgetItem* container = new QTreeWidgetItem();
  container->setSizeHint(0, QSize(13, 130));
  container->setDisabled(true);
  page->addChild(container);
  mTreeWidget->setItemWidget(container, 0, widget);
  setupWidget(widget);
}

void WidgetBox::addPage(QWidget *widget)
{
  insertPage(count(), widget);
}

QTreeWidgetItem * WidgetBox::insertCategory(int index, QString pageName)
{
  QTreeWidgetItem* page = new QTreeWidgetItem();
  mTreeWidget->insertTopLevelItem(index, page);
  createCategoryButton(page, pageName);

  return page;
}

void WidgetBox::insertPage(int index, QWidget *widget)
{
  // Set default page name
  QString pageName = widget->windowTitle();
  if (pageName.isEmpty())
  {
    pageName = tr("Page %1").arg(count() + 1);
    widget->setWindowTitle(pageName);
  }

  QTreeWidgetItem* page = insertCategory(index, pageName);
  createContainerWidget(page, widget);

  connect(widget, SIGNAL(windowTitleChanged(QString)),
          categoryButton(index), SLOT(setTitle(QString)));
}

void WidgetBox::removePage(int index)
{
  delete mTreeWidget->topLevelItem(index);
}

int WidgetBox::currentIndex() const
{
  if(count() > 0) {
    return mTreeWidget->currentIndex().row();
  } else {
    return -1;
  }
}

void WidgetBox::setCurrentIndex(int index)
{
  if (index != currentIndex() && checkIndex(index))
  {
    mTreeWidget->setCurrentItem(mTreeWidget->topLevelItem(index));
    emit currentIndexChanged(index);
  }
}

bool WidgetBox::checkIndex(int index) const
{
  return index >= 0 && index < count();
}

QWidget *WidgetBox::page(int index) const
{
  if(checkIndex(index))
  {
    QTreeWidgetItem* page = mTreeWidget->topLevelItem(index);
    return mTreeWidget->itemWidget(page->child(0), 0);
  }
  else
  {
    return 0;
  }
}

QWidget* WidgetBox::widget(int index) const
{
  return page(index);
}

void WidgetBox::setPageTitle(QString const &newTitle)
{
  if(currentIndex() >= 0 && count() > 0)
  {
    categoryButton(currentIndex())->setText(newTitle);
    emit pageTitleChanged(newTitle);
  }
}

QString WidgetBox::pageTitle() const
{
  if(currentIndex() >= 0) {
    return categoryButton(currentIndex())->text();
  } else {
    return QString();
  }
}

PageButton *WidgetBox::categoryButton(int index) const
{
  return (PageButton *)mTreeWidget->itemWidget(mTreeWidget->topLevelItem(index), 0);
}

bool WidgetBox::isPageExpanded() const
{
  if(currentIndex() >= 0) {
    return categoryButton(currentIndex())->isExpanded();
  } else {
    return false;
  }
}

void WidgetBox::setPageExpanded(bool expanded)
{
  if(currentIndex() >= 0) {
    categoryButton(currentIndex())->setExpanded(expanded);
  }
}

int WidgetBox::getPageIndex(QTreeWidgetItem *item)
{
  if (!item) return -1;

  QTreeWidgetItem *parent = item->parent();
  if(parent)  // Parent is top level item
  {
    return mTreeWidget->indexOfTopLevelItem(parent);
  }
  else        // Current item is top level
  {
    return item->treeWidget()->indexOfTopLevelItem(item);
  }
}

void WidgetBox::onItemClicked(QTreeWidgetItem *item, int )
{
  int index = getPageIndex(item);
  setCurrentIndex(index);
}
