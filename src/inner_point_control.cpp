#include <inner_point_control.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>

InnerPointControl::InnerPointControl(QWidget *parent, Renderer *renderer)
	:QWidget(parent), renderer_{renderer}
{
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setAlignment(Qt::AlignTop);

	QHBoxLayout *titleLayout = new QHBoxLayout();
	titleLayout->setAlignment(Qt::AlignCenter);
	titlelabel_ = new QLabel(tr("Inner Point Interpolation"), this);
	titlelabel_->setStyleSheet("font-weight: bold; font-size: 11pt");
	titlelabel_->setContentsMargins(0, 2, 0, 15);
	titleLayout->addWidget(titlelabel_);
	mainLayout->addLayout(titleLayout);

	QHBoxLayout *innerPointLayout = new QHBoxLayout();
	innerPointchk_ = new QCheckBox("Enable inner Point interpolation");
	
	QObject::connect(
		innerPointchk_, &QCheckBox::stateChanged,
		this, &InnerPointControl::innerPointchk_stateChanged);

 	innerPointLayout->addWidget(innerPointchk_);
 	mainLayout->addLayout(innerPointLayout);

	QHBoxLayout *uLayout = new QHBoxLayout();
	ulabel_ = new QLabel(tr("u: "), this);
	uLayout->addWidget(ulabel_);
	uspin_ = new QDoubleSpinBox(this);
	uspin_->setRange(0.0, 1.0);
	uspin_->setValue(0.5);
	uspin_->setSingleStep(0.05);
	uspin_->setEnabled(false);

	QObject::connect(
		uspin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, &InnerPointControl::uspin_valueChanged);

	uLayout->addWidget(uspin_);
	mainLayout->addLayout(uLayout);

	QHBoxLayout *vLayout = new QHBoxLayout();
	vlabel_ = new QLabel(tr("v: "), this);
	vLayout->addWidget(vlabel_);
	vspin_ = new QDoubleSpinBox(this);
	vspin_->setRange(0.0, 1.0);
	vspin_->setValue(0.5);
	vspin_->setSingleStep(0.05);
	vspin_->setEnabled(false);

	QObject::connect(
		vspin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, &InnerPointControl::vspin_valueChanged);

	vLayout->addWidget(vspin_);
	mainLayout->addLayout(vLayout);

	setLayout(mainLayout);
}

void InnerPointControl::innerPointchk_stateChanged(int state)
{
	switch(state)
	{
		case Qt::Unchecked:
			uspin_->setEnabled(false);
			vspin_->setEnabled(false);
			renderer_->hideInnerPointInterpolation();
			break;
		case Qt::Checked:
			uspin_->setEnabled(true);
			vspin_->setEnabled(true);	
			renderer_->interpolateInnerPoint(float(uspin_->value()), float(vspin_->value()));	
			break;
	}
}

#include <iostream>

void InnerPointControl::vspin_valueChanged(double d)
{
	renderer_->interpolateInnerPoint(float(uspin_->value()), float(vspin_->value()));
}

void InnerPointControl::uspin_valueChanged(double d)
{
	renderer_->interpolateInnerPoint(float(uspin_->value()), float(vspin_->value()));
}