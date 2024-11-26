#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

class Triangle : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Triangle(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QTimer* m_timer;
    float m_angle;

private slots:
    void updateRotation();
};


#endif // TRIANGLE_H


