#include "hermite_curve.hpp"
#include <cmath>
#include <QMouseEvent>

HermiteCurve::HermiteCurve(std::shared_ptr<FergusonCanvas> canvas)
	:p0_{QPointF(-0.5,0.0)},    d0_{QPointF( 0.5,  0.5)}, 
	 p1_{QPointF( 0.5,0.0)},    d1_{QPointF(-0.5,  0.5)},
	 cp0_{p0_, 0.015, 15},      cp1_{p1_, 0.015, 15},
	 cd0_{p0_+d0_, 0.015, 15},  cd1_{p1_+d1_, 0.015, 15}, 
	 resolution_{30},
	 canvas_{canvas}
{}

std::vector<float> HermiteCurve::computePoints(unsigned int num_points) const
{
	std::vector<float> vertices(num_points*2);
	float stepSize = 1.f / float(num_points-1);

	// Curve
	for (unsigned int i = 0; i < num_points; i++) {
		float u1 = stepSize * float(i);
		float u2 = u1*u1;
		float u3 = u2*u1;

		QPointF p = b0(u3,u2,u1) * p0_ + b1(u3,u2,u1) * p1_ + b2(u3,u2,u1) * d0_ + b3(u3,u2,u1) * d1_;
		vertices[2*i]   = p.x();
		vertices[2*i+1] = p.y();
	}

	// tangents
	vertices.push_back(p0_.x()); 	           vertices.push_back(p0_.y());
	vertices.push_back(p0_.x() + d0_.x());     vertices.push_back(p0_.y() + d0_.y());
	
	vertices.push_back(p1_.x()); 	           vertices.push_back(p1_.y());
	vertices.push_back(p1_.x() + d1_.x());     vertices.push_back(p1_.y() + d1_.y());

	std::vector<float> circleVertices = cp0_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());
	circleVertices = cp1_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());
	circleVertices = cd0_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());
	circleVertices = cd1_.computePoints();
	vertices.insert(vertices.end(), circleVertices.begin(), circleVertices.end());

	return vertices;
}

float HermiteCurve::b0(float u3, float u2, float u1) const
{
	return 2.f * u3 + -3.f * u2 + 1.f;
}

float HermiteCurve::b1(float u3, float u2, float u1) const
{
	return -2.f * u3 + 3.* u2;
}

float HermiteCurve::b2(float u3, float u2, float u1) const
{
	return u3 - 2.f * u2 + u1;
}

float HermiteCurve::b3(float u3, float u2, float u1) const
{
	return u3 - u2;
}

#include <iostream>

void HermiteCurve::init()
{
	initializeOpenGLFunctions();
	setupShaders();
	setupGeometry();
}

void HermiteCurve::setupShaders()
{
	shader_ = new QOpenGLShaderProgram();
	shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/ferguson.vs");
	shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/ferguson.fs");
	shader_->bindAttributeLocation("pos", 0);
	shader_->link();
	shader_->bind();
}

void HermiteCurve::setupGeometry()
{
	vao_.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);

	std::vector<float> vertices = computePoints(resolution_);

	vbo_.create();
	vbo_.bind();
	vbo_.allocate(vertices.data(), vertices.size() * sizeof(float));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

QPointF HermiteCurve::toViewportCoordSystem(const QPointF& screenCoords) const
{

	return QPointF(2.f*screenCoords.x()/canvas_->width() - 1.f, 1.f - 2.f*screenCoords.y()/canvas_->height());
}

void HermiteCurve::updateGPUBuffers()
{
	canvas_->makeCurrent();
	std::vector<float> vertices = computePoints(resolution_);
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);
	vbo_.bind();

	vbo_.write(0, vertices.data(), vertices.size() * sizeof(float));
	canvas_->update();
	canvas_->doneCurrent();
}

void HermiteCurve::render()
{
	unsigned int offset = 0;
	QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);
	shader_->bind();
	shader_->setUniformValue("colour", QVector3D(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_LINE_STRIP, offset, resolution_);
	offset += resolution_;
	shader_->setUniformValue("colour", QVector3D(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_LINES, offset, 4);
	offset += 4;
	
	// draw circles
	shader_->setUniformValue("color", QVector3D(0.0f, 1.0f, 0.0f));
	glDrawArrays(GL_TRIANGLE_FAN, offset, cp0_.resolution());
	offset += cp0_.resolution()+1;
	// shader_->setUniformValue("color", QVector3D(0.0f, 1.0f, 0.0f));
	glDrawArrays(GL_TRIANGLE_FAN, offset, cp1_.resolution());
	offset += cp1_.resolution()+1;

	glDrawArrays(GL_TRIANGLE_FAN, offset, cp1_.resolution());
	offset += cd0_.resolution()+1;

	glDrawArrays(GL_TRIANGLE_FAN, offset, cp1_.resolution());
	offset += cd1_.resolution()+1;

	shader_->release();
}

void HermiteCurve::cleanUp()
{
	if (shader_ != nullptr) {
		vao_.destroy();		
		vbo_.destroy();

		delete shader_;
		shader_ = nullptr;
	}
}

void HermiteCurve::keyPress(QKeyEvent *e)
{ }

void HermiteCurve::keyRelease(QKeyEvent *e)
{ }

void HermiteCurve::mousePress(QMouseEvent *e)
{ 
	QPointF pos = e->localPos();
	pos = toViewportCoordSystem(pos);
	
	if(cp0_.contains(pos))
		cp0_.select();

	else if(cp1_.contains(pos))
		cp1_.select();		

	else if(cd0_.contains(pos))
		cd0_.select();		
	
	else if(cd1_.contains(pos))
		cd1_.select();		
}

void HermiteCurve::mouseMove(QMouseEvent *e)
{		
	QPointF pos = toViewportCoordSystem(e->localPos());
	
	if (cp0_.isSelected()) {
		p0_ = pos;
		cd0_.centre(p0_ + d0_);
		cp0_.centre(pos);
		updateGPUBuffers();
	}

	else if (cp1_.isSelected()) {
		p1_ = pos;
		cd1_.centre(p1_ + d1_);
		cp1_.centre(pos);
		updateGPUBuffers();
	}

	else if (cd0_.isSelected()) {
		d0_ = pos - p0_;
		cd0_.centre(pos);
		updateGPUBuffers();
	}

	else if (cd1_.isSelected()) {
		d1_ = pos - p1_;
		cd1_.centre(pos);
		updateGPUBuffers();
	}  
}

void HermiteCurve::mouseRelease(QMouseEvent *e)
{
	cp0_.unselect();
	cp1_.unselect(); 
	cd0_.unselect();
	cd1_.unselect();
}


HermiteCurve::~HermiteCurve()
{
	cleanUp();
}


// ---------------------- Circle -----------------------------------

Circle::Circle(QPointF centre, float radius, unsigned int resolution)
	:centre_{centre}, radius_{radius}, resolution_{resolution}
{}

std::vector<float> Circle::computePoints() const
{
	float angleStep = (2.0f * M_PI) / float(resolution_-1);

	std::vector<float> vertices(resolution_*2+2);
	vertices[0] = centre_.x();
	vertices[1] = centre_.y();

	for (unsigned int i = 0; i < resolution_; i++) {
		float angle = angleStep * i;	
		vertices[i*2] = radius_ * cos(angle) + centre_.x();
		vertices[2*i+1] = radius_ * sin(angle) + centre_.y();
	}

	return vertices;
}

bool Circle::contains(const QPointF p) const
{
	float dx = p.x() - centre_.x();
	float dy = p.y() - centre_.y();
	float dx2 = dx*dx;
	float dy2 = dy*dy;
	float r2 = radius_*radius_;
	return dx2 + dy2 <= r2;
}