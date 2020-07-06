#include <ferguson_canvas.hpp>


FergusonCanvas::FergusonCanvas(QOpenGLWidget *renderer, int width, int height)
	:renderer_{renderer}, width_{width}, height_{height},
	 viewLeft_{-10.0f}, viewRight_{10.0f}, viewBottom_{-10.0f}, viewTop_{10.0f},
	 nearPlane_{-10.0}, farPlane_{10.0f}
{ }

void FergusonCanvas::init()
{	
	initializeOpenGLFunctions();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	initCameraMatrices();
	for (std::shared_ptr<Drawing> d : drawings_){
		d->init();
	}
}

void FergusonCanvas::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (std::shared_ptr<Drawing> d : drawings_){
		d->setViewMatrixToShader(view_);
		d->setProjectionMatrixToShader(projection_);
		d->render();
	}
}

// Keyboard Event
void FergusonCanvas::keyPress(QKeyEvent *e)
{
	for (std::shared_ptr<Drawing> d : drawings_)
		d->keyPress(e);
}

void FergusonCanvas::keyRelease(QKeyEvent *e)
{
	for (std::shared_ptr<Drawing> d : drawings_)
		d->keyRelease(e);
}

// Mouse Event
void FergusonCanvas::mousePress(QMouseEvent *e)
{
	for (std::shared_ptr<Drawing> d : drawings_)
		d->mousePress(e);
}

void FergusonCanvas::mouseMove(QMouseEvent *e)
{
	for (std::shared_ptr<Drawing> d : drawings_)
		d->mouseMove(e);
}

void FergusonCanvas::mouseRelease(QMouseEvent *e)
{
	for (std::shared_ptr<Drawing> d : drawings_)
		d->mouseRelease(e);
}

void FergusonCanvas::destroy()
{
	for (std::shared_ptr<Drawing> d : drawings_)
		d->cleanUp();
}

void FergusonCanvas::initCameraMatrices()
{
	view_.lookAt(
		QVector3D(0.0f, 0.0f, -10.0f),
		QVector3D(0.0f, 0.0f,   0.0f),
		QVector3D(0.0f, 1.0f,   0.0f));
	
	projection_.ortho(viewLeft_, viewRight_, viewBottom_, viewTop_, nearPlane_, farPlane_);
}