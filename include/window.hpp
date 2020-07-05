#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <renderer.hpp>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

class MainWidget: public QWidget
{
public:
	MainWidget();
	void save();
private:
	Renderer *renderer_;
};

class Window: public QMainWindow
{	
public:
	Window();	
	void save();

private:
	QMenu *fileMenu_;
	QAction *saveAct_;
};



#endif