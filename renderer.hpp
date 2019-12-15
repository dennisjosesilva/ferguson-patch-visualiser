#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "ferguson_canvas.hpp"
// #include "hermite_curve.hpp"


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{

	Q_OBJECT

public:
	Renderer(QWidget *parent);


	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);


	void interpolateInnerPoint(float u, float v);
	void hideInnerPointInterpolation();

	void showHandlers();
	void hideHandlers();

	~Renderer();

protected:
	void initializeGL() override;
	void paintGL() override;
	void cleanUp();

protected:
	std::shared_ptr<FergusonCanvas> canvas_;
};

#endif