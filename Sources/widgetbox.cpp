#include <QDebug>
#include <QEvent>
#include <QBoxLayout>
#include <QHeaderView>

#include "widgetbox.h"
#include "CategoryWidgets.h"
#include "PageEventFilter.h"

/*!
 * \class WidgetBox
 * \brief The WidgetBox class: Widget similar to the Widget Box in the Qt Designer.
 * It contains a list of widgets (pages) separated by categories. Each category
 * can be clicked in order to expand and collapse the list below the category.
 *
*/

WidgetBox::WidgetBox(QWidget *parent) : QWidget(parent)
{
  // Set default tree widget settings
  mTreeWidget = new QTreeWidget(this);
  mTreeWidget->setFrameStyle(QFrame::NoFrame);
  mTreeWidget->setAnimated(true);
  mTreeWidget->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
  mTreeWidget->header()->hide();
  mTreeWidget->setRootIsDecorated(false);
  mTreeWidget->setIndentation(0);
  mTreeWidget->setUniformRowHeights(false);
  mTreeWidget->setSelectionMode(QAbstractItemView::NoSelection);
  mTreeWidget->setExpandsOnDoubleClick(false);

  // Make tree widget same color as normal widget
  QPalette pal = mTreeWidget->palette();
  pal.setColor(QPalette::Base, palette().background().color());
  mTreeWidget->setPalette(pal);

  // Create WidgetBox layout
  QBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mTreeWidget);

  connect(mTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
          SLOT(onItemClicked(QTreeWidgetItem*, int)));
  connect(mTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
          SLOT(onItemClicked(QTreeWidgetItem*, int)));
}
WidgetBox::~WidgetBox()
{
  delete mSheet;
}


#if defined(QT_PLUGIN)
QSize WidgetBox::sizeHint() const
{
  return QSize(130, 210);
}
#endif

/*!
 * \brief WidgetBox::createCategory create page category widget
 * \param page page item
 * \param pageName page name to display in category
 */
void WidgetBox::createCategory(QTreeWidgetItem* page, QString pageName)
{
  AbstractCategory *category;
  switch (mCategoryType)
  {
    case Button:
      category = new ButtonCategory(pageName, mTreeWidget, page);
      break;
    case Line:
      category = new LineCategory(pageName, mTreeWidget, page);
      break;
    default:
      category = new ButtonCategory(pageName, mTreeWidget, page);
      break;
  }
  // Set new item widget: Qt removes old widget automatically, no need to delete old one
  mTreeWidget->setItemWidget(page, 0, category);
  // Change property in Designer on page expand/unexpand
  connect(category, SIGNAL(pageExpanded(bool)), SLOT(setPageExpandedProperty()));
}

/*!
 * \brief WidgetBox::recreateCategories recreates categories widgets if
 * categoryType was changed
 */
void WidgetBox::recreateCategories()
{
  for(int i = 0; i < count(); i++)
  {
    QString pageName = pageTitle(i);
    bool expanded = isPageExpanded(i);
    // Qt removes old widget automatically, no need to delete old one
    createCategory(mTreeWidget->topLevelItem(i), pageName);
    category(i)->setExpanded(expanded);
  }
}

void WidgetBox::setCategoryType(const CategoryType type)
{
  mCategoryType = type;
  recreateCategories();
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
  widget->setPalette(palette());
  widget->setBackgroundRole(backgroundRole());
  widget->setStyle(style());  // Set parent widget style
}

void WidgetBox::createContainerWidget(QTreeWidgetItem* page, QWidget *widget)
{
  QTreeWidgetItem* container = new QTreeWidgetItem();
  container->setDisabled(true);
  page->addChild(container);
  mTreeWidget->setItemWidget(container, 0, widget);
  setupWidget(widget);

  // Send mouse events from page widget to tree widget
  PageEventFilter *eventFilter = new PageEventFilter(this, container);
  widget->installEventFilter(eventFilter);
  widget->setObjectName(QString("__qt__passive_Page%1").arg(count() + 1));

#if defined(QT_PLUGIN)
  QSize size(mTreeWidget->width(), mTreeWidget->width() / 1.618);
  container->setSizeHint(0, size);
  widget->resize(size);
  // Change item size hint in Qt Designer on page widget resize
  // as seems QTreeWidgetItem knows nothing about its widget in design time
  // and does not automatically adjust item size like it does in run-time
  PageResizeFilter *filter = new PageResizeFilter(this, container);
  widget->installEventFilter(filter);
#endif
}

void WidgetBox::addPage(QWidget *widget)
{
  insertPage(count(), widget);
}

QTreeWidgetItem * WidgetBox::insertCategory(int index, QString pageName)
{
  QTreeWidgetItem* page = new QTreeWidgetItem();
  mTreeWidget->insertTopLevelItem(index, page);
  createCategory(page, pageName);

  return page;
}

void WidgetBox::insertPage(int index, QWidget *widget)
{
  if (index < 0 || index > count())
    return;

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
          category(index), SLOT(setTitle(QString)));
}

void WidgetBox::removePage(int index)
{
  QTreeWidgetItem *topLevelItem = mTreeWidget->topLevelItem(index);
  if (!topLevelItem)
    return;

  if (topLevelItem->childCount() > 0)
    mTreeWidget->removeItemWidget(topLevelItem->child(0), 0);
  mTreeWidget->removeItemWidget(topLevelItem, 0);

  delete topLevelItem;
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
  if (checkIndex(currentIndex()))
  {
    category(currentIndex())->setTitle(newTitle);
    // Qt doc: use QWidget::windowTitle property to store the page title.
    // Note that currently there is no way of adding a custom property
    // (e.g., a page title) to the pages without using a predefined property as placeholder.
    page(currentIndex())->setWindowTitle(newTitle);
    emit pageTitleChanged(newTitle);
  }
}

/*!
 * \brief WidgetBox::pageTitle Current page title
 * \return
 */
QString WidgetBox::pageTitle() const
{
  return pageTitle(currentIndex());
}

/*!
 * \brief WidgetBox::pageTitle page title
 * \param index page index
 * \return
 */
QString WidgetBox::pageTitle(int index) const
{
  if (checkIndex(index))
  {
    return category(index)->title();
  }
  else
  {
    return QString();
  }
}

AbstractCategory *WidgetBox::category(int index) const
{
  return (AbstractCategory *)mTreeWidget->itemWidget(mTreeWidget->topLevelItem(index), 0);
}


/*!
 * \brief WidgetBox::isPageExpanded Checks current page is expanded
 * \return
 */
bool WidgetBox::isPageExpanded() const
{
  return isPageExpanded(currentIndex());
}

/*!
 * \brief WidgetBox::isPageExpanded Checks page with given index is expanded
 * \param index page index
 * \return true if specified page expanded
 */
bool WidgetBox::isPageExpanded(int index) const
{
  if (checkIndex(index))
  {
    return category(index)->isExpanded();
  }
  else
  {
    return false;
  }
}

void WidgetBox::setPageExpandedProperty()
{
  changeQtDesignerProperty("isPageExpanded", isPageExpanded(currentIndex()), true);
}

void WidgetBox::setPageExpanded(bool expanded)
{
  if(checkIndex(currentIndex()))
  {
    setPageExpanded(currentIndex(), expanded);
    setPageExpandedProperty();
  }
}

void WidgetBox::setPageExpanded(int index, bool expanded)
{
  if(checkIndex(index)) category(index)->setExpanded(expanded);
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

  changeQtDesignerProperty("currentIndex", index);
}

void WidgetBox::changeQtDesignerProperty(QString propertyName, QVariant value,
                                         bool markChangedOnly)
{
#if defined(QT_PLUGIN)
  QDesignerFormWindowInterface *form =
      QDesignerFormWindowInterface::findFormWindow(this);

  if(form)
  {
    if(!mSheet) // Need to create sheet only once
    {
      QDesignerFormEditorInterface *editor = form->core();
      QExtensionManager *manager = editor->extensionManager();
      mSheet = qt_extension<QDesignerPropertySheetExtension*>(manager, this);
    }
    // Set property in Qt Designer
    int propertyIndex = mSheet->indexOf(propertyName);
    if(!markChangedOnly) mSheet->setProperty(propertyIndex, value);
    mSheet->setChanged(propertyIndex, true);
  }
#else
  Q_UNUSED(propertyName);
  Q_UNUSED(value);
  Q_UNUSED(markChangedOnly);
#endif
}
