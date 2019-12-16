#ifndef FERGUSON_CANVAS_HPP_INCLUDED
#define FERGUSON_CANVAS_HPP_INCLUDED

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <canvas.hpp>
#include <drawing.hpp>
#include <memory>
#include <vector>

class FergusonCanvas : public Canvas, protected QOpenGLFunctions  
{
public:
	FergusonCanvas(QOpenGLWidget *renderer, int width, int height);

	void init() override;
	void render() override;

	// Keyboard Event
	void keyPress(QKeyEvent *e) override;
	void keyRelease(QKeyEvent *e) override;

	// Mouse Event
	void mousePress(QMouseEvent *e) override;
	void mouseMove(QMouseEvent *e) override;
	void mouseRelease(QMouseEvent *e) override;

	void insertDrawing(std::shared_ptr<Drawing> d) { drawings_.push_back(d); }

	void destroy() override;

	int height() const override { return height_; }
	int width() const override { return width_; }

	void update() const { renderer_->update(); } ;
	void makeCurrent() const { renderer_->makeCurrent(); }
	void doneCurrent() const { renderer_->doneCurrent(); }

	inline std::shared_ptr<Drawing> getDrawing(unsigned int index) { return drawings_[index]; }
private:
	std::vector<std::shared_ptr<Drawing>> drawings_;
	QOpenGLWidget *renderer_;
	int width_;
	int height_;
};

#endif