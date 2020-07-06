#include <ferguson_patch.hpp>
#include <cmath>
#include <QMouseEvent>


#include <iostream>

// ------------------------ Circle ------------------------------------------------
Circle::Circle(QPointF centre, float radius, unsigned int resolution)
	:centre_{centre}, radius_{radius}, resolution_{resolution}, selected_{false}
{}

std::vector<float> Circle::computePoints() const 
{
	float angleStep = (2.f * M_PI) / float(resolution_ - 1);

	std::vector<float> vertices(resolution_*2+2);
	vertices[0] = centre_.x();
	vertices[1] = centre_.y();

	for (unsigned int i = 0; i < resolution_; ++i) {
		float angle = angleStep * i;
		vertices[i*2] = radius_ * cos(angle) + centre_.x();
		vertices[i*2+1] = radius_ * sin(angle)  + centre_.y();
	}

	return vertices;
}

bool Circle::contains(const QPointF p) const 
{
	float dx = p.x() - centre_.x();
	float dy = p.y() - centre_.y();
	float dx2 = dx*dx;
	float dy2 = dy*dy;
	float r2 = radius_ * radius_;
	return dx2 + dy2 <= r2;
}

// ------------------------------ FERGUSON PATCH -----------------------------------------------------
HermiteCurveComputer::HermiteCurveComputer(QPointF p0, QPointF t0, QPointF p1, QPointF t1, 
	unsigned int resolution, unsigned int startIndex, std::shared_ptr<Canvas> canvas,
	float tangentScale)
	:p0_{p0}, t0_{t0}, p1_{p1}, t1_{t1}, 
	 resolution_{resolution}, 
	 startIndex_{startIndex},
	 startTangentIndex_{startIndex_ + resolution_},
	 cp0_{p0_, 0.2, 10}, ct0_{p0_+t0_, 0.2, 10}, cp1_{p1_, 0.2, 10}, ct1_{p1_+t1_, 0.2, 10},
	 canvas_{canvas}, tangentScale_{tangentScale}
{ }

std::vector<float> HermiteCurveComputer::computePoints() const
{
	std::vector<float> vertices(resolution_*2);
	float stepSize = 1.f / float(resolution_-1);

	// Curve
	for (unsigned int i = 0; i < resolution_; ++i) {
		float u1 = stepSize * float(i);
		float u2 = u1*u1;
		float u3 = u2*u1;

		QPointF p = b0(u3,u2,u1) * p0_ + b1(u3,u2,u1) * p1_ + 
			b2(u3,u2,u1) * tangentScale_* t0_ + b3(u3,u2,u1) * tangentScale_ * t1_;
		vertices[2*i] = p.x();
		vertices[2*i+1] = p.y();
	}

	// tangents 
	vertices.push_back(p0_.x());               vertices.push_back(p0_.y());
	vertices.push_back(p0_.x() + t0_.x());      vertices.push_back(p0_.y() + t0_.y());

	vertices.push_back(p1_.x());               vertices.push_back(p1_.y());
	vertices.push_back(p1_.x() + t1_.x());     vertices.push_back(p1_.y() + t1_.y());

	std::vector<float> circleVertices = cp0_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());

	circleVertices = ct0_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());	

	circleVertices = cp1_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());	

	circleVertices = ct1_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());	

	return vertices;
}

float HermiteCurveComputer::b0(float u3, float u2, float u1) const
{
	return 2.f * u3 + -3.f * u2 + 1.f;
}

float HermiteCurveComputer::b1(float u3, float u2, float u1) const
{
	return -2.f * u3 + 3. * u2;
}

float HermiteCurveComputer::b2(float u3, float u2, float u1) const
{
	return u3 - 2.f * u2 + u1;
}

float HermiteCurveComputer::b3(float u3, float u2, float u1) const
{
	return u3 - u2;
}


QPointF HermiteCurveComputer::toWorldCoordinate(const QPointF &screenCoords) const
{
	std::shared_ptr<FergusonCanvas> fcanvas = std::dynamic_pointer_cast<FergusonCanvas>(canvas_); 
	
	float rl = (fcanvas->viewRight() - fcanvas->viewLeft()) / 2.0f;
	float bt = (fcanvas->viewTop() - fcanvas->viewBottom()) / 2.0f;

	float hmid = fcanvas->viewLeft() + rl;
	float vmid = fcanvas->viewBottom() + bt;

	return QPointF(hmid + rl * -screenCoords.x(), vmid + bt * screenCoords.y());
}

void HermiteCurveComputer::mousePress(QPointF pos)
{
	pos = toWorldCoordinate(pos);

	if (cp0_.contains(pos)) 
		cp0_.select();

	else if (ct0_.contains(pos)) 
		ct0_.select(); 

	else if (cp1_.contains(pos)) 
		cp1_.select(); 

	else if (ct1_.contains(pos)) 
		ct1_.select(); 
}

void HermiteCurveComputer::mouseMove(QPointF pos)
{
	pos = toWorldCoordinate(pos);

	if (cp0_.isSelected()){
		p0_ = pos;
		cp0_.centre(pos);
		ct0_.centre(p0_ + t0_);
	}
	else if (cp1_.isSelected()){
		p1_ = pos;
		cp1_.centre(pos);
		ct1_.centre(p1_ + t1_);
	}
	else if (ct0_.isSelected()){
		t0_ = pos - p0_;
		ct0_.centre(pos);
	}
	else if (ct1_.isSelected()){
		t1_ = pos - p1_;
		ct1_.centre(pos);
	}
}

void HermiteCurveComputer::mouseRelease(QPointF pos)
{
	cp0_.unselect();
	ct0_.unselect();
	cp1_.unselect();
	ct1_.unselect();
}

bool HermiteCurveComputer::hasControlPointSelected() const
{
	return cp0_.isSelected() || ct0_.isSelected() || cp1_.isSelected() || ct1_.isSelected();
}


// ------------------------------- FERGUSON PATCH ------------------------------------------------------
FergusonPatch::FergusonPatch(
	HermiteCurveComputer h0, HermiteCurveComputer h1,
	HermiteCurveComputer h2, HermiteCurveComputer h3,
	unsigned int resolution, std::shared_ptr<FergusonCanvas> canvas)
	:h0_{h0}, h1_{h1}, h2_{h2}, h3_{h3}, resolution_{resolution}, canvas_{canvas},
	 shouldShowInterpolateLines_{false}, lastu_{0.5f}, lastv_{0.5f}, shouldShowHandlers_{true}
{}

std::vector<float> FergusonPatch::computePoints() const
{
	std::vector<float> vertices = h0_.computePoints();
	
	std::vector<float> vCurve = h1_.computePoints();
	vertices.insert(vertices.end(), vCurve.begin(), vCurve.end());
	
	vCurve = h2_.computePoints();
	vertices.insert(vertices.end(), vCurve.begin(), vCurve.end());

	vCurve = h3_.computePoints();
	vertices.insert(vertices.end(), vCurve.begin(), vCurve.end());

	return vertices;
}

void FergusonPatch::interpolateInnerPoint(float u, float v)
{	
	lastu_ = u;
	lastv_ = v;
	updateGPUBuffersInterpolatingLines(u, v);
}

std::vector<float> FergusonPatch::computePointsForInterpolatingLines(float u, float v)
{
	std::vector<float> vertices(2*resolution_*2);
	float step = 1.f / float(resolution_-1);

	for (unsigned i = 0; i < resolution_; i++) {
		QPointF p = s(u , step * float(i));
		
		vertices[2*i] = p.x();
		vertices[2*i+1] = p.y();
	}

	for (unsigned i = 0; i < resolution_; i++) {
		QPointF p = s(step * float(i), v);
		vertices[(2*resolution_)+(2*i)] = p.x();
		vertices[(2*resolution_)+(2*i+1)] = p.y();
	}

	QPointF p = s(u, v);
	Circle circle(p, 0.02, 10);
	std::vector<float> circleVertices = circle.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end()); 

	return vertices;
}

float FergusonPatch::b0(float u3, float u2, float u1) const
{
	return 2.f * u3 - 3.f * u2 + 1.f;
}

float FergusonPatch::b1(float u3, float u2, float u1) const
{
	return u3 - 2.f * u2 + u1;
}

float FergusonPatch::b2(float u3, float u2, float u1) const
{
	return u3 - u2;
}

float FergusonPatch::b3(float u3, float u2, float u1) const
{
	return -2.f * u3 + 3.* u2;
}

QPointF FergusonPatch::s(float u, float v) const
{
	float u1 = u;
	float u2 = u1*u1;
	float u3 = u2*u1;

	float v1 = v;
	float v2 = v1*v1;
	float v3 = v2*v1;

	QPointF p0  = h0_.p0(), p1  = h0_.p1(), p2  = h2_.p0(), p3  = h2_.p1();
	QPointF t01 = h0_.st0(), t10 = -h0_.st1(), t13 = h1_.st0(), t31 = -h1_.st1(),
			t23 = h2_.st0(), t32 = -h2_.st1(), t02 = h3_.st0(), t20 = -h3_.st1(); 

	// return
	// 	b0(u3, u2, u1) * (b0(v3, v2, v1)*p0  + b1(v3, v2, v1)*t02 + b2(v3, v2, v1)*t20 + b3(v3, v2, v1)*p2 ) + 
	// 	b1(u3, u2, u1) * (b0(v3, v2, v1)*t01                                           + b3(v3, v2, v1)*t10) + 
	// 	b2(u3, u2, u1) * (b0(v3, v2, v1)*t23                                           + b3(v3, v2, v1)*t32) +
	// 	b3(u3, u2, u1) * (b0(v3, v2, v1)*p1  + b1(v3, v2, v1)*t13 + b2(v3, v2, v1)*t31 + b3(v3, v2, v1)*p3 );
	return
		b0(v3, v2, v1) * (b0(u3, u2, u1)*p0  + b1(u3, u2, u1)*t02 + b2(u3, u2, u1)*t20 + b3(u3, u2, u1)*p2 ) + 
		b1(v3, v2, v1) * (b0(u3, u2, u1)*t01                                           + b3(u3, u2, u1)*t23) + 
		b2(v3, v2, v1) * (b0(u3, u2, u1)*t10                                           + b3(u3, u2, u1)*t32) +
		b3(v3, v2, v1) * (b0(u3, u2, u1)*p1  + b1(u3, u2, u1)*t13 + b2(u3, u2, u1)*t31 + b3(u3, u2, u1)*p3 );
}

void FergusonPatch::init()
{
	initializeOpenGLFunctions();
	setupShaders();
	setupGeometry();
	interpolateInnerPoint(0.5f, 0.5f);
}


void FergusonPatch::setViewMatrixToShader(const QMatrix4x4 &viewMatrix) const
{
	shader_->bind();
	shader_->setUniformValue("view", viewMatrix);
}

void FergusonPatch::setProjectionMatrixToShader(const QMatrix4x4 &projMatrix) const
{
	shader_->bind();
	shader_->setUniformValue("projection", projMatrix);
}

void FergusonPatch::setupShaders()
{
	shader_ = new QOpenGLShaderProgram();
	shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/ferguson.vs");
	shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/ferguson.fs");
	shader_->bindAttributeLocation("pos", 0);
	shader_->link();
	shader_->bind();
}

void FergusonPatch::setupGeometry()
{
	vao_.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);

	std::vector<float> vertices = computePoints();

	vbo_.create();
	vbo_.bind();
	vbo_.allocate(vertices.data(), vertices.size() * sizeof(float) * resolution_*2*2);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void FergusonPatch::updateGPUBuffers(const HermiteCurveComputer &h)
{
	canvas_->makeCurrent();
	std::vector<float> vertices = h.computePoints();
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);
	
	vbo_.bind();
	vbo_.write(h.startIndex() * sizeof(float)*2, vertices.data(), vertices.size() * sizeof(float));
	canvas_->update();
	canvas_->doneCurrent();
}


void FergusonPatch::updateGPUBuffersInterpolatingLines(float u, float v)
{
	canvas_->makeCurrent();
	std::vector<float> vertices = computePointsForInterpolatingLines(u, v);
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);
	 
	vbo_.bind();
	vbo_.write(
		(resolution_+4+44)*4 * sizeof(float)*2, 
		vertices.data(),
		vertices.size() * sizeof(float));

	canvas_->update();
	canvas_->doneCurrent();
}

QPointF FergusonPatch::toViewportCoordSystem(const QPointF &screenCoords) const
{
	return QPointF(2.f * screenCoords.x() / canvas_->width() -1.f, 1.f - 2.f * screenCoords.y() / canvas_->height());
}

void FergusonPatch::render()
{
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);
	shader_->bind();
	shader_->setUniformValue("colour", QVector3D(0.0f, 0.0f, 0.0f));
	
	glDrawArrays(GL_LINE_STRIP, h0_.startIndex(), h0_.resolution());
	glDrawArrays(GL_LINE_STRIP, h1_.startIndex(), h1_.resolution());
	glDrawArrays(GL_LINE_STRIP, h2_.startIndex(), h2_.resolution());
	glDrawArrays(GL_LINE_STRIP, h3_.startIndex(), h3_.resolution());


	if (shouldShowHandlers_)
	{
		// Draw tangents
		shader_->setUniformValue("colour", QVector3D(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, h0_.startIndex()+h0_.resolution(), 4);
		glDrawArrays(GL_LINES, h1_.startIndex()+h1_.resolution(), 4);
		glDrawArrays(GL_LINES, h2_.startIndex()+h2_.resolution(), 4);
		glDrawArrays(GL_LINES, h3_.startIndex()+h3_.resolution(), 4);

		// draw circles
		glDrawArrays(GL_TRIANGLE_FAN, h0_.startIndex()+h0_.resolution()+4+11*0, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h0_.startIndex()+h0_.resolution()+4+11*1, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h0_.startIndex()+h0_.resolution()+4+11*2, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h0_.startIndex()+h0_.resolution()+4+11*3, 11);

		glDrawArrays(GL_TRIANGLE_FAN, h1_.startIndex()+h1_.resolution()+4+11*0, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h1_.startIndex()+h1_.resolution()+4+11*1, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h1_.startIndex()+h1_.resolution()+4+11*2, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h1_.startIndex()+h1_.resolution()+4+11*3, 11);

		glDrawArrays(GL_TRIANGLE_FAN, h2_.startIndex()+h2_.resolution()+4+11*0, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h2_.startIndex()+h2_.resolution()+4+11*1, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h2_.startIndex()+h2_.resolution()+4+11*2, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h2_.startIndex()+h2_.resolution()+4+11*3, 11);

		glDrawArrays(GL_TRIANGLE_FAN, h3_.startIndex()+h3_.resolution()+4+11*0, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h3_.startIndex()+h3_.resolution()+4+11*1, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h3_.startIndex()+h3_.resolution()+4+11*2, 11);
		glDrawArrays(GL_TRIANGLE_FAN, h3_.startIndex()+h3_.resolution()+4+11*3, 11);
	}

	// Draw interpolate lines
	if (shouldShowInterpolateLines_) {
		shader_->setUniformValue("colour", QVector3D(0.0, 0.0, 1.0));
		glDrawArrays(GL_LINE_STRIP, h3_.startIndex()+h3_.resolution()+4+11*4, resolution_);
		glDrawArrays(GL_LINE_STRIP, (h3_.startIndex()+h3_.resolution()+4+11*4)+resolution_, resolution_);
		glDrawArrays(GL_TRIANGLE_FAN, (h3_.startIndex()+h3_.resolution()+4+11*4)+2*resolution_, 11);
	}
}

void FergusonPatch::keyPress(QKeyEvent *e)
{ }

void FergusonPatch::keyRelease(QKeyEvent *e)
{ }

void FergusonPatch::mousePress(QMouseEvent *e)
{
	if (shouldShowHandlers_) {
		QPointF p = toViewportCoordSystem(e->localPos());
		h0_.mousePress(p);
		h1_.mousePress(p);
		h2_.mousePress(p);
		h3_.mousePress(p);
	}
}

void FergusonPatch::mouseMove(QMouseEvent *e)
{ 
	QPointF p = toViewportCoordSystem(e->localPos());

	if (h0_.hasControlPointSelected()) {
		h0_.mouseMove(p);
		updateGPUBuffers(h0_);
		interpolateInnerPoint(lastu_, lastv_);
	}
	if (h1_.hasControlPointSelected()) {
		h1_.mouseMove(p);
		updateGPUBuffers(h1_);
		interpolateInnerPoint(lastu_, lastv_);
	}
	if (h2_.hasControlPointSelected()) {
		h2_.mouseMove(p);
		updateGPUBuffers(h2_);
		interpolateInnerPoint(lastu_, lastv_);
	}
	if (h3_.hasControlPointSelected()) {
		h3_.mouseMove(p);
		updateGPUBuffers(h3_);
		interpolateInnerPoint(lastu_, lastv_);
	}
}

void FergusonPatch::mouseRelease(QMouseEvent *e)
{
	QPointF p = toViewportCoordSystem(e->localPos());
	h0_.mouseRelease(p);
	h1_.mouseRelease(p);
	h2_.mouseRelease(p);
	h3_.mouseRelease(p);
}

void FergusonPatch::cleanUp()
{
	if (shader_ != nullptr) {
		vao_.destroy();
		vbo_.destroy();

		delete shader_;
		shader_ = nullptr;
	}
}

FergusonPatch::~FergusonPatch()
{
	cleanUp();
}