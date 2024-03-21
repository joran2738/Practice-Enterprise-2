#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto *t = new QTimer(this);
    t->setInterval(60);
    connect(t, &QTimer::timeout,this,[=]{loop();ui->screen->update();});
    t->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

