#include "ferguson_canvas.hpp"


FergusonCanvas::FergusonCanvas(QOpenGLWidget *renderer, int width, int height)
	:renderer_{renderer}, width_{width}, height_{height} 
{ }

void FergusonCanvas::init()
{	
	initializeOpenGLFunctions();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	for (std::shared_ptr<Drawing> d : drawings_){
		d->init();
	}
}

void FergusonCanvas::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (std::shared_ptr<Drawing> d : drawings_)
		d->render();
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

#include <iostream>
void FergusonCanvas::destroy()
{
	for (std::shared_ptr<Drawing> d : drawings_)
		d->cleanUp();
}