#ifndef FERGUSON_CANVAS_HPP_INCLUDED
#define FERGUSON_CANVAS_HPP_INCLUDED

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

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

	void initCameraMatrices();

	// Keyboard Event
	void keyPress(QKeyEvent *e) override;
	void keyRelease(QKeyEvent *e) override;

	// Mouse Event
	void mousePress(QMouseEvent *e) override;
	void mouseMove(QMouseEvent *e) override;
	void mouseRelease(QMouseEvent *e) override;

	void insertDrawing(std::shared_ptr<Drawing> d) { drawings_.push_back(d); }

	void destroy() override;

	inline float viewLeft() const { return viewLeft_; }
	inline float viewRight() const { return viewRight_; }
	inline float viewBottom() const { return viewBottom_; }
	inline float viewTop() const { return viewTop_; }
	inline float farPlane() const { return farPlane_; }
	inline float nearPlane() const { return nearPlane_; }

	

	int height() const override { return height_; }
	int width() const override { return width_; }

	void update() const { renderer_->update(); } ;
	void makeCurrent() const { renderer_->makeCurrent(); }
	void doneCurrent() const { renderer_->doneCurrent(); }

	inline std::shared_ptr<Drawing> getDrawing(unsigned int index) { return drawings_[index]; }
private:
	std::vector<std::shared_ptr<Drawing>> drawings_;
	QOpenGLWidget *renderer_;

	QMatrix4x4 view_;
	QMatrix4x4 projection_;

	float viewLeft_;
	float viewRight_;
	float viewBottom_;
	float viewTop_;
	float farPlane_;
	float nearPlane_;

	int width_;
	int height_;
};

#endif