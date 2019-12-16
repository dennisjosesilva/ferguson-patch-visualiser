#ifndef HERMITE_CURVE_HPP_INCLUDED
#define HERMITE_CURVE_HPP_INCLUDED


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <QPointF>
#include <vector>
#include <memory>

#include <drawing.hpp>
#include <ferguson_canvas.hpp>

class Circle 
{
public:
	Circle(QPointF centre, float radius, unsigned int resolution);

	unsigned int  resolution() const { return resolution_; }
	unsigned int& resolution() { return resolution_; }
	void resolution(unsigned int val) { resolution_ = val; }

	const QPointF& centre() const { return centre_; }
	QPointF& centre() { return centre_; }
	void centre(QPointF val) { centre_ = val; }

	float radius() const { return radius_; }
	float& radius() { return radius_; }
	void radius(float val) { radius_ = val; }

	bool contains(const QPointF p) const;

	std::vector<float> computePoints() const;

	void select()  { selected_ = true; }
	void unselect()  { selected_ = false; }
	bool isSelected() const { return selected_; }

private:
	bool selected_;
	QPointF centre_;
	float radius_;
	unsigned int resolution_;
};

class HermiteCurve : public Drawing, protected QOpenGLFunctions
{
public:
	HermiteCurve(std::shared_ptr<FergusonCanvas> canvas);

	// starting point 
	const QPointF &p0() const { return p0_; }
	QPointF       &p0() {  return p0_; }
	void           p0(QPointF val) { p0_ = val; }

	// derivative at starting point
	const QPointF &d0() const { return d0_; }
	QPointF       &d0() { return d0_; }
	void           d0(QPointF val) { d0_ = val; }

	// end point
	const QPointF &p2() const { return p1_; }
	QPointF       &p2() { return p1_; }
	void           p2(QPointF val) { p1_ = val; }

	// derivative at end point
	const QPointF &d2() const { return d1_; }
	QPointF       &d2() { return d1_; }
	void           d2(QPointF val) { d1_ = val; }

	std::vector<float> computePoints(unsigned int num_points) const;

	void init() override;
	void render() override;

	// keyboard events
	void keyPress(QKeyEvent *e) override;
	void keyRelease(QKeyEvent *e) override;

	// mouse events
	void mousePress(QMouseEvent *e) override;
	void mouseMove(QMouseEvent *e) override;
	void mouseRelease(QMouseEvent *e) override;

	void cleanUp() override;

	~HermiteCurve();

private: 
	// basis functions
	float b0(float u3, float u2, float u1) const;
	float b1(float u3, float u2, float u1) const;
	float b2(float u3, float u2, float u1) const;
	float b3(float u3, float u2, float u1) const;

	void setupShaders();
	void setupGeometry();

	void updateGPUBuffers();

	QPointF toViewportCoordSystem(const QPointF& screenCoords) const;

private:
	int resolution_;
	QOpenGLShaderProgram *shader_;
	QOpenGLVertexArrayObject vao_;
	QOpenGLBuffer vbo_;

private:
	std::shared_ptr<FergusonCanvas> canvas_;

	QPointF p0_;
	QPointF d0_;
	QPointF p1_;
	QPointF d1_;

	// circles for each point
	Circle cp0_;  // circle for control point 0
	Circle cp1_;  // circle for control point 1
	Circle cd0_;  // circle for tangent of control point 0
	Circle cd1_;  // circle for tangent of control point 1
};

#endif 