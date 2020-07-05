#include <window.hpp>
#include <renderer.hpp>
#include <inner_point_control.hpp>
#include <ferguson_control.hpp>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <memory>

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

MainWidget::MainWidget()
{
	setWindowTitle(tr("Ferguson patch visualiser"));
	renderer_ = new Renderer(this);
	InnerPointControl *innerPointControl = new InnerPointControl(this, renderer_);
	FergusonControl *fergusonControl = new FergusonControl(this, renderer_);

	QGridLayout *layout = new QGridLayout();
	QVBoxLayout *controlLayout = new QVBoxLayout();
	controlLayout->addWidget(fergusonControl);
	controlLayout->addWidget(innerPointControl);
	controlLayout->setAlignment(Qt::AlignTop);
	controlLayout->addStretch(0);
	layout->addLayout(controlLayout, 0, 0);
	layout->addWidget(renderer_, 0, 1);

	setLayout(layout);
}

void MainWidget::save()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), 
		QDir::currentPath(), tr("PNG (*.png);;JPEG (*.jpg, *.jpeg)")); 
	
	if (!renderer_->save(filename))
		QMessageBox::warning(this, tr("Save Image"), tr("Error saving image"));
}


Window::Window()
{
	QWidget *mainWidget = new MainWidget();
	setCentralWidget(mainWidget);

	saveAct_ = new QAction(tr("&Save..."), this);
	saveAct_->setShortcuts(QKeySequence::Save);
	saveAct_->setStatusTip(tr("Save canvas into a file"));
	connect(saveAct_, &QAction::triggered, this, &Window::save);

	fileMenu_ = menuBar()->addMenu(tr("&File"));
	fileMenu_->addAction(saveAct_);
}

void Window::save()
{
	MainWidget *w = dynamic_cast<MainWidget*>(centralWidget());
	w->save();
}