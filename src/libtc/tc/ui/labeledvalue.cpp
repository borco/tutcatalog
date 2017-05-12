#include "labeledvalue.h"

#include <QLabel>
#include <QVBoxLayout>

namespace tc {
namespace ui {

LabeledValue::LabeledValue(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout;
    layout->setMargin(0);

    auto label = new QLabel;
    layout->addWidget(label);
    connect(this, &LabeledValue::labelChanged, label, &QLabel::setText);

    auto value = new QLabel;
    layout->addWidget(value, 1, Qt::AlignRight);
    connect(this, &LabeledValue::valueChanged, value, &QLabel::setText);

    setLayout(layout);
}

} // namespace ui
} // namespace tc
