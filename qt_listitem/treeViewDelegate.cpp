#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <qpainter.h>

class TreeViewDelegate : public QStyledItemDelegate
{
public:
    explicit TreeViewDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, Qt::gray);
        }
    }
};