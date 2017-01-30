#include <QHBoxLayout>

#include "widgetbox.h"
#include "CategoryWidgets.h"
#include "PageEventFilter.h"

/**
 * @class AbstractCategoryButton is abstract ancestor class for all categories
 * widgets types
 * @brief AbstractCategoryButton::AbstractCategoryButton
 */

AbstractCategory::AbstractCategory(const QString &, QTreeWidget *parent,
                                   QTreeWidgetItem *item)
  : QWidget(parent)
  , mItem(item)
  , mEventFilter(new PageEventFilter(this, item))
{
  mItem->setExpanded(true);
  connect(mEventFilter, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(onButtonPress()));
}

void AbstractCategory::onButtonPress()
{
  setExpanded(!mItem->isExpanded());
}

void AbstractCategory::onPageExpand(bool expanded)
{
  mItem->setExpanded(expanded);
  int index = mItem->treeWidget()->indexOfTopLevelItem(mItem);
  ((WidgetBox *)mItem->treeWidget()->parent())->setCurrentIndex(index);

  emit pageExpanded(isExpanded());
}

int AbstractCategory::itemIndex() const
{
  return mItem->treeWidget()->indexOfTopLevelItem(mItem);
}

/**
 * @class PageButton
 * @brief The PageButton class: page (category) button for widget box
 */
ButtonCategory::ButtonCategory(const QString &text, QTreeWidget *parent,
                               QTreeWidgetItem *item)
  : AbstractCategory(text, parent, item)

{
  mButton = new QPushButton(text, parent);
  mButton->installEventFilter(eventFilter()); // Send mouse events to tree widget
  // Prefix __qt__passive_ enables mouse events for widget in Qt Designer
  mButton->setObjectName(QString("__qt__passive_ButtonCategory%1").arg(itemIndex()));

  QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
  horizontalLayout->setSpacing(0);
  horizontalLayout->setContentsMargins(0, 0, 0, 0);
  horizontalLayout->addWidget(mButton);    // Add button to layout

  mButton->setIcon(QIcon(":/plugins/widgetbox/expanded.png"));
  // setFlat(true);

  mButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  QFontMetrics fm(font());
  mButton->resize(size().width(), fm.height());
}

void ButtonCategory::setTitle(QString const &title)
{
  mButton->setText(title);
}

void ButtonCategory::onButtonPress()
{
  AbstractCategory::onButtonPress();

  if(isExpanded()) {
    mButton->setIcon(QIcon(":/plugins/widgetbox/expanded.png"));
  } else {
    mButton->setIcon(QIcon(":/plugins/widgetbox/collapsed.png"));
  }
}

LineCategory::LineCategory(const QString &text, QTreeWidget *parent, QTreeWidgetItem *item)
  : AbstractCategory(text, parent, item)
{
  QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
  horizontalLayout->setSpacing(4);
  horizontalLayout->setContentsMargins(0, 0, 6, 0);

  mCheckBox = new QCheckBox(this);
  mCheckBox->setChecked(true);
  mCheckBox->installEventFilter(eventFilter()); // Send mouse events to tree widget
  horizontalLayout->addWidget(mCheckBox);    // Add checkbox to layout
  // Prefix __qt__passive_ enables mouse events for widget in Qt Designer
  mCheckBox->setObjectName(QString("__qt__passive_CheckBox%1").arg(itemIndex()));

  QFrame *line1 = new QFrame(this);
  line1->installEventFilter(eventFilter()); // Send mouse events to tree widget
  // Prefix __qt__passive_ enables mouse events for widget in Qt Designer
  line1->setObjectName(QString("__qt__passive_line1_%1").arg(itemIndex()));
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(line1->sizePolicy().hasHeightForWidth());
  line1->setSizePolicy(sizePolicy);
  line1->setFrameShadow(QFrame::Plain);
  line1->setLineWidth(1);
  line1->setMidLineWidth(1);
  line1->setFrameShape(QFrame::HLine);
  horizontalLayout->addWidget(line1);       // Add line1 to layout

  mLabel = new QLabel(text, this);
  mLabel->installEventFilter(eventFilter()); // Send mouse events to tree widget
  // Prefix __qt__passive_ enables mouse events for widget in Qt Designer
  mLabel->setObjectName(QString("__qt__passive_Label%1").arg(itemIndex()));
  QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
  sizePolicy1.setHorizontalStretch(0);
  sizePolicy1.setVerticalStretch(0);
  sizePolicy1.setHeightForWidth(mLabel->sizePolicy().hasHeightForWidth());
  mLabel->setSizePolicy(sizePolicy1);
  horizontalLayout->addWidget(mLabel);      // Add label to layout

  QFrame *line2 = new QFrame(this);
  line2->installEventFilter(eventFilter()); // Send mouse events to tree widget
  // Prefix __qt__passive_ enables mouse events for widget in Qt Designer
  line2->setObjectName(QString("__qt__passive_line2_%1").arg(itemIndex()));
  sizePolicy.setHeightForWidth(line2->sizePolicy().hasHeightForWidth());
  line2->setSizePolicy(sizePolicy);
  line2->setFrameShadow(QFrame::Plain);
  line2->setLineWidth(1);
  line2->setMidLineWidth(1);
  line2->setFrameShape(QFrame::HLine);
  horizontalLayout->addWidget(line2);       // Add line2 to layout
}

void LineCategory::setExpanded(bool expanded)
{
  AbstractCategory::setExpanded(expanded);
  mCheckBox->setChecked(expanded);
}

void LineCategory::setTitle(QString const &title)
{
  mLabel->setText(title);
}
