#ifndef FERGUSON_PATCH_HPP_INCLUDED
#define FERGUSON_PATCH_HPP_INCLUDED

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

	float  radius() const { return radius_; }
	float& radius() { return radius_; }
	void radius(float val) { radius_ = val; }

	bool contains(const QPointF p) const;

	std::vector<float> computePoints() const;


	void select() { selected_ = true; }
	void unselect() { selected_ = false; }
	bool isSelected() const { return selected_; }
private:
	bool selected_;
	QPointF centre_;
	float radius_;
	unsigned int resolution_;
};

class HermiteCurveComputer
{
public:
	HermiteCurveComputer(
		QPointF p0=QPointF( 0.25f, 0.0f), 
		QPointF t0=QPointF( 0.10f, 0.0f), 
		QPointF p1=QPointF( 0.75f, 0.0f), 
		QPointF t1=QPointF(-0.10f, 0.0f),
		unsigned int resolution = 10,
		unsigned int startIndex = 0);

	unsigned int  startIndex() const { return startIndex_; }
	unsigned int& startIndex() { return startIndex_; }
	void startIndex(unsigned int val) { startIndex_ = val; }

	unsigned int  startTangentIndex() const { return startTangentIndex_; }
	unsigned int &startTangentIndex() { return startTangentIndex_; }
	void startTangentIndex(unsigned int val) { startTangentIndex_ = val; }

	unsigned int  resolution() const { return resolution_; }
	unsigned int &resolution() { return resolution_; }
	void resolution(unsigned int val) { resolution_ = val; }

	const QPointF  &p0() const { return p0_; }
	QPointF        &p0() { return p0_; }
	void            p0(QPointF val) { p0_ = val; }

	const QPointF  &t0() const { return t0_; }
	QPointF        &t0() { return t0_; }
	void            t0(QPointF val) { t0_ = val; }

	const QPointF  &p1() const { return p1_; }
	QPointF        &p1() { return p1_; }
	void            p1(QPointF val) { p1_ = val; }

	const QPointF  &t1() const { return t1_; }
	QPointF        &t1() { return t1_; }
	void            t1(QPointF val) { t1_ = val; }

	std::vector<float> computePoints() const;

	bool hasControlPointSelected() const;

	void mousePress(QPointF pos);
	void mouseMove(QPointF pos);
	void mouseRelease(QPointF pos);

private:
	float b0(float u3, float u2, float u1) const;
	float b1(float u3, float u2, float u1) const;
	float b2(float u3, float u2, float u1) const;
	float b3(float u3, float u2, float u1) const;

private:
	unsigned int startIndex_;
	unsigned int startTangentIndex_;
	
	unsigned resolution_; 

	QPointF p0_;
	QPointF t0_;
	QPointF p1_;
	QPointF t1_;

	Circle cp0_;
	Circle ct0_;
	Circle cp1_;
	Circle ct1_;
};

class FergusonPatch : public Drawing, protected QOpenGLFunctions
{
public:
	FergusonPatch(
		HermiteCurveComputer h0, HermiteCurveComputer h1,
		HermiteCurveComputer h2, HermiteCurveComputer h3,
		unsigned int resolution, std::shared_ptr<FergusonCanvas> canvas);

	unsigned int  resolution() const { return resolution_; }
	unsigned int &resolution() { return resolution_; }
	void resolution(unsigned int val) { resolution_ = val; }

	std::vector<float> computePoints() const;

	void init() override;
	void render() override; 

	// keyboard events
	void keyPress(QKeyEvent *e) override;
	void keyRelease(QKeyEvent *e) override;

	// mouse events
	void mousePress(QMouseEvent *e) override;
	void mouseMove(QMouseEvent *e) override;
	void mouseRelease(QMouseEvent *e) override;

	void interpolateInnerPoint(float u, float v);

	void cleanUp() override;

	~FergusonPatch();

	inline void showInterpolateLines() { shouldShowInterpolateLines_ = true; }
	inline void hideInterpolateLines() { shouldShowInterpolateLines_ = false; }

	inline void showHandlers() { shouldShowHandlers_ = true; }
	inline void hideHandlers() { shouldShowHandlers_ = false; }

private:
	float b0(float u3, float u2, float u1) const;
	float b1(float u3, float u2, float u1) const;
	float b2(float u3, float u2, float u1) const;
	float b3(float u3, float u2, float u1) const;
	QPointF s(float u, float v) const;

	std::vector<float> computePointsForInterpolatingLines(float u, float v);

private:
	void setupShaders();
	void setupGeometry();

	void updateGPUBuffers(const HermiteCurveComputer &h);
	void updateGPUBuffersInterpolatingLines(float u, float v);

	QPointF toViewportCoordSystem(const QPointF &screenCoords) const;
private:
	unsigned int resolution_;

	float lastu_;
	float lastv_;

	HermiteCurveComputer h0_;
	HermiteCurveComputer h1_;
	HermiteCurveComputer h2_;
	HermiteCurveComputer h3_;

	std::shared_ptr<FergusonCanvas> canvas_;

	QOpenGLShaderProgram *shader_;
	QOpenGLVertexArrayObject vao_;
	QOpenGLBuffer vbo_;

	bool shouldShowInterpolateLines_;
	bool shouldShowHandlers_;
};

#endif