#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "camera.h"
#include "view.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Scene *s = new Scene(new Camera());
    s->setLogger(ui->pLog);
    View *v = new View(s);

    ui->viewLayout->addWidget(v);
}

MainWindow::~MainWindow()
{
    delete ui;
}
