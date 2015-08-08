#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QObject>
#include "common.hh"
#include "glwidget.hh"

class QPushButton;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    QPushButton *dockBtn;
    QPushButton *fbo;
    QPushButton *mvp;
    MyMesh mesh;
    GLWidget *window,*window_fbo;


signals:

private slots:
    void startMyProcess();
    void renderFbo();
    void mvpTest();
};

#endif // MAINWINDOW_H
