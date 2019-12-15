#include "window.hpp"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QSurfaceFormat fmt;
	fmt.setSamples(4);
	fmt.setDepthBufferSize(24);
	fmt.setStencilBufferSize(8);
	fmt.setVersion(3, 2);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);

	Window window;
	window.show();
	return app.exec();
}