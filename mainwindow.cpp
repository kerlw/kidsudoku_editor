#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStageData.h"
#include "QCampaignData.h"

#include <iostream>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

static const QString FILE_FILTER = "KidSudoku Packed Campaign Files(*.spc)";
static const QString EXTENSION_NAME = ".spc";      //file extension name, spc = Sudoku Packed Campaign

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_sizeGrid(QSize(1, 3)),
    m_sizeBox(QSize(1, 1)),
    m_bChanged(false),
    m_pCampaign(NULL),
    m_pCurrentEditStage(NULL)
{
    ui->setupUi(this);

    m_modelStages = new QStandardItemModel(this);
    ui->m_lvStages->setModel(m_modelStages);

    m_modelSudoku = new QStandardItemModel(this);
    ui->m_tbSudokuBox->setModel(m_modelSudoku);

    m_modelRes = new QStandardItemModel(this);
    ui->m_lvResources->setModel(m_modelRes);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (m_pCampaign)
        delete m_pCampaign;
}

void MainWindow::on_action_New_triggered()
{
    MainWindow::ExecuteResult ret = confirmSaveChanges();
    if (ret == MainWindow::Canceled)
        return;

    //clear old data
    m_modelStages->clear();
    m_modelRes->clear();
    m_modelSudoku->clear();
    m_bChanged = false;

    //create new campaign data and refresh views
    m_pCampaign = new QCampaignData();
    QStageData* data = QStageData::create(m_sizeGrid, m_sizeBox);
    m_pCampaign->addStageData(data);

    onCampaignDataLoaded();
}

void MainWindow::onCampaignDataLoaded() {
    //check whether the campaign data is valid or not.
    if (!m_pCampaign || !m_pCampaign->getStageDataCount())
        return;

    QStringList list = m_pCampaign->getStagesStringList();
    int count  = list.count();
    for (int index = 0; index < count; index++) {
        QStandardItem* item = new QStandardItem(list.at(index));
        m_modelStages->appendRow(item);
    }
    ui->m_grpCampaign->setEnabled(true);

    //set current selection at row 0
    QModelIndex mindex = m_modelStages->index(0, 0);
    ui->m_lvStages->setCurrentIndex(mindex);
    ui->m_lvStages->clicked(mindex);
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

void MainWindow::refreshView() {
    refreshSudokuBox();
}

void MainWindow::refreshSelectedStageDescInList() {
    if (!m_pCurrentEditStage)
        return;

    QModelIndex index = ui->m_lvStages->currentIndex();
    if (index.isValid())
        m_modelStages->setData(index, m_pCurrentEditStage->toQString());
}

void MainWindow::refreshSudokuBox() {
    m_modelSudoku->clear();
    if (m_pCurrentEditStage) {
        int rows = m_pCurrentEditStage->boxSize().height() * m_pCurrentEditStage->gridSize().height();
        int cols = m_pCurrentEditStage->gridSize().width() * m_pCurrentEditStage->boxSize().width();

        m_modelSudoku->setColumnCount(cols);
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
            m_modelSudoku->appendRow(list);
        }

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

        refreshSelectedStageDescInList();
        refreshSudokuBox();
        m_bChanged = true;
    }
}

void MainWindow::on_m_spbColsPerGrid_valueChanged(int value)
{
    if (m_pCurrentEditStage) {
        m_pCurrentEditStage->resetColsPerGrid(value);

        refreshSelectedStageDescInList();
        refreshSudokuBox();
        m_bChanged = true;
    }
}

void MainWindow::on_m_spbGridsInRow_valueChanged(int value)
{
    if (m_pCurrentEditStage) {
        m_pCurrentEditStage->resetGridsInRow(value);

        refreshSelectedStageDescInList();
        refreshSudokuBox();
        m_bChanged = true;
    }
}

void MainWindow::on_m_spbGridsInCol_valueChanged(int value)
{
    if (m_pCurrentEditStage) {
        m_pCurrentEditStage->resetGridsInCol(value);

        refreshSelectedStageDescInList();
        refreshSudokuBox();
        m_bChanged = true;
    }
}

void MainWindow::on_action_Open_triggered()
{
    if (confirmSaveChanges() == MainWindow::Canceled)
        return;

    QString path = QFileDialog::getOpenFileName(this, "Open Kidsudoku Campaign File", ".", FILE_FILTER);
    if (path.length() == 0)
            return;

    std::cout << path.toStdString();
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::information(this, "Failed", "Open file failed!");
        return;
    }

    QByteArray ba = file.read(4);
    quint32 len = 0;
    QDataStream(ba) >> len;
    byte* buffer = new byte[len];
    file.read((char* )buffer, len);

    if (m_pCampaign == NULL)
        m_pCampaign = new QCampaignData();
    m_pCampaign->parseFromBytes(buffer, len);
    m_pCurrentEditStage = NULL;

    file.close();
    refreshView();
}

void MainWindow::on_action_Save_triggered()
{
    doSave();
}

void MainWindow::on_actionSave_as_triggered()
{
    doSaveAs();
}

void MainWindow::on_action_Add_a_stage_triggered()
{

}

void MainWindow::on_actionMove_up_stage_triggered()
{

}

void MainWindow::on_actionMove_dow_n_stage_triggered()
{

}

void MainWindow::on_actionDel_stage_triggered()
{

}

void MainWindow::on_actionAdd_resource_triggered()
{

}

void MainWindow::on_actionDel_resource_triggered()
{

}

void MainWindow::on_actionDuplicate_selected_stage_triggered()
{
    if (m_pCurrentEditStage == NULL)
        return;

    QStageData* data = QStageData::create(m_pCurrentEditStage);
    m_pCampaign->addStageData(data);

    QStandardItem* item = new QStandardItem(data->toQString());
    ((QStandardItemModel*)ui->m_lvStages->model())->appendRow(item);
}

MainWindow::ExecuteResult MainWindow::confirmSaveChanges() {
    if (m_pCampaign == NULL)
        return MainWindow::Error;

    MainWindow::ExecuteResult result = MainWindow::DONE;

    if (m_bChanged) {
        int ret = QMessageBox::information(this, tr("Confirm"), tr("Change has not been saved, save it?"),
                                           tr("Save it now!"),
                                           tr("Cancel"),
                                           tr("No"),
                                           0, 1);
        if (ret == 1) { //canceled, dont release campaign data.
            return MainWindow::Canceled;
        } else if (ret == 2) {
            result = MainWindow::DONE;
        } else {
            result = doSave();
        }
    }

    delete m_pCampaign;
    m_pCampaign = NULL;
    return result;
}

MainWindow::ExecuteResult MainWindow::doSave() {
    if (!m_pCampaign)
        return MainWindow::Error;

    QString path = m_pCampaign->getSavePath();
    if (path.length() == 0) {
        return doSaveAs();
    } else
        return saveToFile(path);
}

MainWindow::ExecuteResult MainWindow::doSaveAs() {
label_start:
    QString path = QFileDialog::getSaveFileName(this, tr("Save"), ".", FILE_FILTER);
    if (path.length() == 0)
        return MainWindow::Canceled;

    //check if has specified extention name, if not, add it.
    if (!path.endsWith(EXTENSION_NAME))
        path.append(EXTENSION_NAME);

    QFile file(path);
    if (file.exists()) {
        int ret = QMessageBox::information(this, tr("Overwrite confirm"), tr("File already exist, overwrite it?"),
                                           tr("Yes, overwrite it"),
                                           tr("Cancel"),
                                           tr("No"),
                                           0, 1);
        if (ret == 1)
            return MainWindow::Canceled;
        else if (ret == 2)
            goto label_start;
    }
    return saveToFile(path);
}

MainWindow::ExecuteResult MainWindow::saveToFile(const QString &path) {
    if (!m_pCampaign)
        return MainWindow::Error;

    byte* buff = NULL;
    int len = 0;
    m_pCampaign->toBytes(&buff, len);

    QFile file(path);
    if (!file.open(QFile::ReadWrite|QFile::Truncate)) {
        return MainWindow::Error;
    }

    file.write((const char*)buff, len);
    file.flush();
    file.close();
    //reset the campaign's save path to the new path
    m_pCampaign->setSavePath(path);
    m_bChanged = false;     //TODO change the window's title ???
    return MainWindow::DONE;
}
