#ifndef INNER_POINT_CONTROL_HPP_INCLUDED
#define INNER_POINT_CONTROL_HPP_INCLUDED

#include <renderer.hpp>

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>

class InnerPointControl : public QWidget
{
public:
	InnerPointControl(QWidget *parent, Renderer *renderer);

	void innerPointchk_stateChanged(int state);
	void vspin_valueChanged(double d);
	void uspin_valueChanged(double d);

private:
	QLabel *titlelabel_;
	QLabel *ulabel_;
	QLabel *vlabel_;
	QDoubleSpinBox *uspin_;
	QDoubleSpinBox *vspin_;
	QCheckBox *innerPointchk_;
	Renderer *renderer_;
};

#endif