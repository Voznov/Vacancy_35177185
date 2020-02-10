#include "mtriangle.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtCore/QRunnable>

MTriangle::MTriangle()
    : m_t(0)
    , m_renderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &MTriangle::handleWindowChanged);
}

void MTriangle::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void MTriangle::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &MTriangle::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &MTriangle::cleanup, Qt::DirectConnection);

        win->setColor(Qt::black);
    }
}

void MTriangle::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

class CleanupJob : public QRunnable
{
public:
    CleanupJob(MTriangleRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    MTriangleRenderer *m_renderer;
};

void MTriangle::releaseResources()
{
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

void MTriangle::sync()
{
    if (!m_renderer) {
        m_renderer = new MTriangleRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &MTriangleRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &MTriangleRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setT(m_t);
}

MTriangleRenderer::~MTriangleRenderer()
{
    delete m_program;
}

void MTriangleRenderer::init()
{
    if (!m_program) {
        initializeOpenGLFunctions();

        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/mtriangle.vsh");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/mtriangle.fsh");
        m_program->link();

        m_posAttr = m_program->attributeLocation("posAttr");
        m_colLeftAttr = m_program->attributeLocation("colLeftAttr");
        m_colRightAttr = m_program->attributeLocation("colRightAttr");
        m_timeAttr = m_program->attributeLocation("timeAttr");
        m_matrixUniform = m_program->uniformLocation("matrix");
    }
}

void MTriangleRenderer::paint()
{
    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(45.0f, 1.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    matrix.rotate(180.0f * m_t, 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    const GLfloat vertices[] = {
        0.0f,   1.0f,   0.0f,
        -0.5f,  -0.5f,  0.0f,
        0.5f,   -0.5f,  0.0f
    };

    const GLfloat colorsLeft[] = {
        1.0f,   0.0f,   0.0f,
        1.0f,   0.0f,   0.0f,
        1.0f,   0.0f,   0.0f
    };

    const GLfloat colorsRight[] = {
        0.0f,   1.0f,   0.0f,
        0.0f,   1.0f,   0.0f,
        0.0f,   1.0f,   0.0f
    };

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colLeftAttr, 3, GL_FLOAT, GL_FALSE, 0, colorsLeft);
    glVertexAttribPointer(m_colRightAttr, 3, GL_FLOAT, GL_FALSE, 0, colorsRight);
    glVertexAttrib1f(m_timeAttr, m_t);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colLeftAttr);
    glEnableVertexAttribArray(m_colRightAttr);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(m_colRightAttr);
    glDisableVertexAttribArray(m_colLeftAttr);
    glDisableVertexAttribArray(m_posAttr);


    m_program->release();
}
