#include <ferguson_control.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>

FergusonControl::FergusonControl(QWidget *parent, Renderer *renderer)
	:QWidget(parent), renderer_{renderer}
{
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setAlignment(Qt::AlignTop) ;

	QHBoxLayout *titleLayout = new QHBoxLayout();
	titleLayout->setAlignment(Qt::AlignCenter);
	titlelabel_ = new QLabel(tr("Ferguson Patch Control"), this);
	titlelabel_->setStyleSheet("font-weight: bold; font-size: 11pt;");
	titlelabel_->setContentsMargins(0, 2, 0 , 15);
	titleLayout->addWidget(titlelabel_);
	mainLayout->addLayout(titleLayout);

	QHBoxLayout *showHandlerLayout = new QHBoxLayout();
	showHandlerschk_ = new QCheckBox("Show handlers");
	showHandlerschk_->setCheckState(Qt::Checked);

	QObject::connect(showHandlerschk_, &QCheckBox::stateChanged, 
		this, &FergusonControl::showHandlers_stateChanged);

	showHandlerLayout->addWidget(showHandlerschk_);
	mainLayout->addLayout(showHandlerLayout);

	setLayout(mainLayout);
}

void FergusonControl::showHandlers_stateChanged(int state)
{
	switch(state)
	{
		case Qt::Unchecked:
			renderer_->hideHandlers();
		break;

		case Qt::Checked:
			renderer_->showHandlers();
		break;
	}
}