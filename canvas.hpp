#ifndef CANVAS_HPP_INCLUDED
#define CANVAS_HPP_INCLUDED

class QMouseEvent;
class QKeyEvent;

class Canvas
{
public:
	virtual void init() = 0;
	virtual void render() = 0;

	// Keyboard Event
	virtual void keyPress(QKeyEvent *e) = 0;
	virtual void keyRelease(QKeyEvent *e) = 0;

	// Mouse Event
	virtual void mousePress(QMouseEvent *e) = 0;
	virtual void mouseMove(QMouseEvent *e) = 0;
	virtual void mouseRelease(QMouseEvent *e) = 0;

	virtual void destroy() = 0;

	virtual int width() const = 0;
	virtual int height() const = 0;
};




#endif