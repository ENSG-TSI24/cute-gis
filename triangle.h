#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class Triangle : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Triangle(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // TRIANGLE_H
