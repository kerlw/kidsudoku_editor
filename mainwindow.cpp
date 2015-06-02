#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStageData.h"
#include "QCampaignData.h"

#include <iostream>
#include <QStandardItem>
#include <QStandardItemModel>

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

    m_pCampaign = new QCampaignData();
    QStageData* data = QStageData::create(m_sizeGrid, m_sizeBox);

    m_pCampaign->addStageData(data);

    //TODO get campaign's stages from its list
    QStringList list;
    list << data->toQString();

    //TODO this model's lifecycle should be managed
    QStandardItemModel* model = new QStandardItemModel(this);
    int count  = list.count();
    for (int index = 0; index < count; index++) {
        QStandardItem* item = new QStandardItem(list.at(index));
        model->appendRow(item);
    }
    ui->m_lvStages->setModel(model);

    ui->m_grpCampaign->setEnabled(true);
}

void MainWindow::on_m_lvStages_clicked(const QModelIndex &index)
{
    int pos = index.row();
    m_pCurrentEditStage = m_pCampaign->getStageAt(pos);
    if (m_pCurrentEditStage) {
        if (!ui->m_grpStage->isEnabled())
            ui->m_grpStage->setEnabled(true);
        ui->m_spbColsPerGrid->setValue(m_pCurrentEditStage->gridSize().width());
        ui->m_spbRowsPerGrid->setValue(m_pCurrentEditStage->gridSize().height());
        ui->m_spbGridsInCol->setValue(m_pCurrentEditStage->boxSize().width());
        ui->m_spbGridsInRow->setValue(m_pCurrentEditStage->boxSize().height());
    } else {
        ui->m_grpStage->setEnabled(false);
    }
    refreshSudokuBox();
}

void MainWindow::refreshSudokuBox() {
    if (m_pCurrentEditStage) {
        //TODO this model's lifecycle should be managed
        QStandardItemModel* model = new QStandardItemModel(this);

        int rows = m_pCurrentEditStage->boxSize().height() * m_pCurrentEditStage->gridSize().height();
        int cols = m_pCurrentEditStage->gridSize().width() * m_pCurrentEditStage->boxSize().width();

        model->setColumnCount(cols);

        for (int i = 0; i < rows; i++) {
            QList<QStandardItem*> list;
            for (int j = 0; j < cols; j++) {
                QString str("");
                int index = i * cols + j;
                int number = m_pCurrentEditStage->numberAt(index);
                if (number < 0)
                    continue;

                if (number > 0)
                    str.setNum(m_pCurrentEditStage->numberAt(index));
                list.append(new QStandardItem(str));
            }
            model->appendRow(list);
        }

        ui->m_tbSudokuBox->setModel(model);
        ui->m_tbSudokuBox->resizeColumnsToContents();
        ui->m_tbSudokuBox->resizeRowsToContents();
    } else {
        ui->m_tbSudokuBox->reset();
    }
}

void MainWindow::on_m_tbSudokuBox_clicked(const QModelIndex &index)
{
    int row = index.row();
    int col = index.column();
    std::cout << row << "," << col << std::endl;
}

void MainWindow::on_m_spbRowsPerGrid_valueChanged(int value)
{
    if (m_pCurrentEditStage) {
        m_pCurrentEditStage->resetRowsPerGrid(value);

        refreshSudokuBox();
    }
}

void MainWindow::on_m_spbColsPerGrid_valueChanged(int value)
{
    if (m_pCurrentEditStage) {
        m_pCurrentEditStage->resetColsPerGrid(value);

        refreshSudokuBox();
    }
}

void MainWindow::on_m_spbGridsInRow_valueChanged(int value)
{
    if (m_pCurrentEditStage) {
        m_pCurrentEditStage->resetGridsInRow(value);

        refreshSudokuBox();
    }
}

void MainWindow::on_m_spbGridsInCol_valueChanged(int value)
{
    if (m_pCurrentEditStage) {
        m_pCurrentEditStage->resetGridsInCol(value);

        refreshSudokuBox();
    }
}
