#include "renderer.hpp"
// #include "hermite_curve.hpp"
#include <QOpenGLShaderProgram>
#include <array>
#include "ferguson_patch.hpp"

Renderer::Renderer(QWidget *parent)
	:QOpenGLWidget(parent)
{
	setFixedSize(800, 600);
	canvas_ = std::make_shared<FergusonCanvas>(this, width(), height());
	// canvas_->insertDrawing(std::make_shared<HermiteCurve>(canvas_));

	QPointF p0{-0.75f, 0.75f}, p1{ 0.75f,  0.75f}, p2{-0.75f, -0.75f}, p3{ 0.75f, -0.75f};

	QPointF t01{ 0.05f,  0.00f}, t02{ 0.00f, -0.05f}, t10{-0.05f, 0.00f}, t13{ 0.00f, -0.05f},
			t20{ 0.00f,  0.05f}, t23{ 0.05f,  0.00f}, t31{ 0.00f, 0.05f}, t32{-0.05f,  0.00f};


	unsigned int res=10;
	HermiteCurveComputer h0{p0, t01, p1, t10, res, 0*(res+4+44)}, h1{p1, t13, p3, t31, res, (res+4+44)*1},
						 h2{p2, t23, p3, t32, res, 2*(res+4+44)}, h3{p0, t02, p2, t20, res, (res+4+44)*3};

	canvas_->insertDrawing(std::make_shared<FergusonPatch>(h0, h1, h2, h3, res, canvas_));
}


void Renderer::interpolateInnerPoint(float u, float v)
{
	makeCurrent();
	std::shared_ptr<FergusonPatch> patch = 
		std::dynamic_pointer_cast<FergusonPatch>(canvas_->getDrawing(0));
	patch->interpolateInnerPoint(u, v);
	patch->showInterpolateLines();
	update();
	doneCurrent();
}

void Renderer::hideInnerPointInterpolation()
{
	makeCurrent();
	std::shared_ptr<FergusonPatch> patch = 
		std::dynamic_pointer_cast<FergusonPatch>(canvas_->getDrawing(0));
	patch->hideInterpolateLines();
	update();
	doneCurrent();
}

void Renderer::showHandlers()
{
	makeCurrent();
	std::shared_ptr<FergusonPatch> patch = 
		std::dynamic_pointer_cast<FergusonPatch>(canvas_->getDrawing(0));
	patch->showHandlers();
	update();
	doneCurrent();
}

void Renderer::hideHandlers()
{
	makeCurrent();
	std::shared_ptr<FergusonPatch> patch = 
		std::dynamic_pointer_cast<FergusonPatch>(canvas_->getDrawing(0));
	patch->hideHandlers();
	update();
	doneCurrent();
}

void Renderer::initializeGL()
{
	canvas_->init();
}

void Renderer::paintGL()
{
	canvas_->render();
}

void Renderer::keyPressEvent(QKeyEvent *e)
{
	canvas_->keyPress(e);
}

void Renderer::keyReleaseEvent(QKeyEvent *e)
{
	canvas_->keyRelease(e);
}

void Renderer::mouseMoveEvent(QMouseEvent *e)
{
	canvas_->mouseMove(e);
}

void Renderer::mousePressEvent(QMouseEvent *e)
{
	canvas_->mousePress(e);
}

void Renderer::mouseReleaseEvent(QMouseEvent *e)
{
	canvas_->mouseRelease(e);
}

Renderer::~Renderer()
{
	canvas_->destroy();
}