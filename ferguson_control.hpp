#ifndef FERGUSON_CONTROL_HPP_INCLUDED
#define FERGUSON_CONTROL_HPP_INCLUDED

#include "renderer.hpp"

#include <QWidget>
#include <QLabel>
#include <QCheckBox>

class FergusonControl : public QWidget
{
public:
	FergusonControl(QWidget *parent, Renderer *renderer);

	void showHandlers_stateChanged(int state);

private:
	QLabel *titlelabel_;
	QCheckBox *showHandlerschk_;
	Renderer *renderer_;
};

#endif