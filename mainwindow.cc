#include "mainwindow.hh"
#include <QVBoxLayout>
#include <QPushButton>
#include "externalimporter.hh"
#include "gausscurvature.hh"
#include "meancurvature.hh"

void MainWindow::startMyProcess()
{
//    MyMesh mesh;

#ifdef BUILDIN_READER
    if(! OpenMesh::IO::read_mesh(mesh,"3.off"))
    {
        std::cerr << "Error: Cannot read mesh from ..." << std::endl;
        return 1;
    }
#else
    if(!ExternalImporter<MyMesh>::read_mesh(mesh,"/home/h005/Documents/QtProject/3DFeature/dragon.off"))
    {
        std::cerr << "Error: Cannot read mesh from " << std::endl;
        return;
    }
#endif

    GaussCurvature<MyMesh> a(mesh);
    MeanCurvature<MyMesh> b(mesh);

    window = new GLWidget(mesh, 0);
    window->show();

}

void MainWindow::renderFbo()
{

    GLWidget *window2 = window->getGLWidget();
    window2->show();
}

void MainWindow::mvpTest()
{
//    window->getGLWidget4();

//    GLWidget *window3 = window->getGLWidget5();
//    window3->show();

    window->zwzdone();
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    mesh = MyMesh();
    //window = NULL;
    dockBtn = new QPushButton(tr("Start"),this);
    connect(dockBtn,SIGNAL(clicked()),this,SLOT(startMyProcess()));
    fbo = new QPushButton(tr("fbo"),this);
    connect(fbo,SIGNAL(clicked()),this,SLOT(renderFbo()));
    mvp = new QPushButton(tr("mvp"),this);
    connect(mvp,SIGNAL(clicked()),this,SLOT(mvpTest()));


    QVBoxLayout *middleLayout = new QVBoxLayout;
    middleLayout->addWidget(dockBtn);
    middleLayout->addWidget(fbo);
    middleLayout->addWidget(mvp);
    setLayout(middleLayout);

}

MainWindow::~MainWindow()
{

}

QSize MainWindow::sizeHint() const
{
    return QSize(100,100);
}


