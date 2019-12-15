#include "window.hpp"
#include "renderer.hpp"
#include "inner_point_control.hpp"
#include "ferguson_control.hpp"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

Window::Window()
{
	setWindowTitle(tr("Ferguson curve visualiser"));
	Renderer *renderer = new Renderer(this);
	InnerPointControl *innerPointControl = new InnerPointControl(this, renderer);
	FergusonControl *fergusonControl = new FergusonControl(this, renderer);

	QGridLayout *layout = new QGridLayout();
	QVBoxLayout *controlLayout = new QVBoxLayout();
	controlLayout->addWidget(fergusonControl);
	controlLayout->addWidget(innerPointControl);
	controlLayout->setAlignment(Qt::AlignTop);
	controlLayout->addStretch(0);
	layout->addLayout(controlLayout, 0, 0);
	layout->addWidget(renderer, 0, 1);
	
	setLayout(layout);
}