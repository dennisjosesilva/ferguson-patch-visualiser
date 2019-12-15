#ifndef DRAWING_HPP_INCLUDED
#define DRAWING_HPP_INCLUDED

class QKeyEvent;
class QMouseEvent;

class Drawing
{
public:
	virtual void init() = 0;
	virtual void render() = 0;

	// Keyboard events
	virtual void keyPress(QKeyEvent *e) = 0;
	virtual void keyRelease(QKeyEvent *e) = 0;

	// Mouse Events
	virtual void mousePress(QMouseEvent *e) = 0;
	virtual void mouseMove(QMouseEvent *e) = 0;
	virtual void mouseRelease(QMouseEvent *e) = 0;

	virtual void cleanUp() = 0;

	virtual ~Drawing() {};
};

#endif