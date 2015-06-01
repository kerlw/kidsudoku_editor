#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStageData.h"
#include "QCampaignData.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bChanged(false),
    m_sizeGrid(QSize(1, 3)),
    m_sizeBox(QSize(1, 1))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_New_triggered()
{
    if (m_bChanged) {
        //TODO ask use save or not.
    }

    m_pData = new QCampaignData();
    QStageData* data = QStageData::create(m_sizeGrid, m_sizeBox);

    m_pData->addStageData(data);
    ui->m_propertyEditor->addObject(data);
}
