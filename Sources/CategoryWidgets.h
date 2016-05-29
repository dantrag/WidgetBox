#ifndef CATEGORYWIDGETS_H
#define CATEGORYWIDGETS_H

#include <QWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QLabel>
#include <QCheckBox>

class PageEventFilter;

class AbstractCategory : public QWidget
{
  Q_OBJECT
public:
  AbstractCategory(const QString &text, QTreeWidget *parent, QTreeWidgetItem *item);
  bool isExpanded() const { return mItem->isExpanded(); }
  virtual QString title() const = 0;

public slots:
  virtual void setTitle(QString const &title) = 0;
  virtual void setExpanded(bool expanded) { onPageExpand(expanded); }

protected:
  QTreeWidgetItem *item() const         { return mItem; }
  PageEventFilter *eventFilter() const  { return mEventFilter; }
  int itemIndex() const;

signals:
  void pageExpanded(bool expanded);

protected slots:
  virtual void onButtonPress();
  void onPageExpand(bool expanded = false);

private:
  QTreeWidgetItem* mItem;
  PageEventFilter *mEventFilter;
};

class ButtonCategory : public AbstractCategory
{
  Q_OBJECT
public:
  ButtonCategory(const QString &text, QTreeWidget *parent, QTreeWidgetItem *item);
  QString title() const override { return mButton->text(); }

public slots:
  void setTitle(QString const &title) override;

private slots:
  void onButtonPress() override;

private:
  QPushButton *mButton;
};

class LineCategory : public AbstractCategory
{
  Q_OBJECT
public:
  LineCategory(const QString &text, QTreeWidget *parent, QTreeWidgetItem *item);
  QString title() const override { return mLabel->text(); }

public slots:
  void setTitle(QString const &title) override;
  void setExpanded(bool expanded) override;

private:
  QLabel *mLabel;
  QCheckBox *mCheckBox;
};

#endif // CATEGORYWIDGETS_H
