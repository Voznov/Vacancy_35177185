#ifndef MTRIANGLE_H
#define MTRIANGLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

class MTriangleRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    MTriangleRenderer() : m_t(0), m_program(0) { }
    ~MTriangleRenderer();

    void setT(qreal t) { m_t = t; }

public slots:
    void init();
    void paint();

private:
    GLuint m_posAttr;
    GLuint m_colLeftAttr;
    GLuint m_colRightAttr;
    GLuint m_timeAttr;
    GLuint m_matrixUniform;

    qreal m_t;
    QOpenGLShaderProgram *m_program;
};

class MTriangle : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    MTriangle();

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    qreal m_t;
    MTriangleRenderer *m_renderer;
};

#endif // MTRIANGLE_H
