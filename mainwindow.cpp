#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

#include "QStageData.h"
#include "QCampaignData.h"
#include "QSudokuBoxModel.h"
#include "SudokuUtil.h"

static const QString FILE_FILTER = "KidSudoku Packed Campaign Files(*.pcd)";
static const QString EXTENSION_NAME = ".pcd";      //file extension name, pcd = Packed Campaign Data

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

    m_modelSudoku = new QSudokuBoxModel(this);
    ui->m_tbSudokuBox->setModel(m_modelSudoku);

    m_modelRes = new QStandardItemModel(this);
    ui->m_lvResources->setModel(m_modelRes);

    //set the solver's max search depth
    SudokuSolver::getInstance()->setMaxSolutionCount(4);
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
    m_modelStages->clear();

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
    /**
      * set current edit stagedata to NULL to avoid setting spin editor
      * value changed trigger resize
     **/
    m_pCurrentEditStage = 0;
    QStageData* data = m_pCampaign->getStageAt(pos);
    if (data) {
        if (!ui->m_grpStage->isEnabled())
            ui->m_grpStage->setEnabled(true);
        ui->m_spbColsPerGrid->setValue(data->gridSize().width());
        ui->m_spbRowsPerGrid->setValue(data->gridSize().height());
        ui->m_spbGridsInCol->setValue(data->boxSize().width());
        ui->m_spbGridsInRow->setValue(data->boxSize().height());

        data->initSolver(SudokuSolver::getInstance());
    } else {
        ui->m_grpStage->setEnabled(false);
    }


    m_pCurrentEditStage = data;
    refreshSudokuBox();
}

void MainWindow::refreshView() {
    onCampaignDataLoaded();
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
    refreshSudokuLabel();
    if (m_pCurrentEditStage) {
        int rows = m_pCurrentEditStage->boxSize().height() * m_pCurrentEditStage->gridSize().height();
        int cols = m_pCurrentEditStage->gridSize().width() * m_pCurrentEditStage->boxSize().width();

        m_modelSudoku->setRowCount(rows);
        m_modelSudoku->setColumnCount(cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                QModelIndex index = m_modelSudoku->index(i,j);
                int number = m_pCurrentEditStage->numberAt(i, j);
                if (number <= 0)
                    continue;

                m_modelSudoku->setData(index, number);
            }
        }
    }
}

void MainWindow::refreshSudokuLabel() {
    if (!m_pCurrentEditStage) {
        ui->m_lbSolves->setText("");
        return;
    }

    SudokuSolver *solver = SudokuSolver::getInstance();
    m_pCurrentEditStage->initSolver(solver);
    int scnt = solver->try_solve(false);
    if (scnt == 0) {
        ui->m_lbSolves->setText(tr("Current puzzle has no solution"));
    } else if (scnt == 1) {
        ui->m_lbSolves->setText(tr("Current puzzle has only one solution"));
    } else if (scnt <= 3) {
        ui->m_lbSolves->setText(tr("Current puzzle has ") + QString::number(scnt) + tr("solutions"));
    } else {
        ui->m_lbSolves->setText(tr("Current puzzle has too much solutions"));
    }
}

void MainWindow::on_m_tbSudokuBox_clicked(const QModelIndex &index) {
    if (!m_pCurrentEditStage)
        return;

    int row = index.row();
    int col = index.column();

    m_pCurrentEditStage->toggleSelected(row, col);

    int value = m_pCurrentEditStage->numberAt(row, col);
    m_modelSudoku->setData(index, value);

    if ((value & 0x7fff) <= 0)
        return;

    if ((value & QStageData::SELECTED_MASK) == 0) {
        SudokuSolver::getInstance()->unsetNumber(row, col);
    } else {
        SudokuSolver::getInstance()->setNumber(row, col, (value & 0x7fff));
    }
    refreshSudokuLabel();
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
    QDataStream stream(&file);

    //read campaign data from file.
    if (!m_pCampaign)
        m_pCampaign = new QCampaignData();
    stream >> *m_pCampaign;
    file.close();

    m_pCampaign->setSavePath(path);
    m_bChanged = false;
    m_pCurrentEditStage = NULL;
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
    if (!m_pCampaign)
        return;

    QStageData* data = QStageData::create(QSize(1,1), QSize(1,1));
    m_pCampaign->addStageData(data);

    QStandardItem* item = new QStandardItem(data->toQString());
    ((QStandardItemModel*)ui->m_lvStages->model())->appendRow(item);
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
    if (!m_pCampaign)
        return;
}

void MainWindow::on_actionDel_resource_triggered()
{

}

void MainWindow::on_actionDuplicate_selected_stage_triggered()
{
    if (!m_pCurrentEditStage)
        return;

    QModelIndex index = ui->m_lvStages->currentIndex();
    QStageData* data = QStageData::create(m_pCurrentEditStage);
    m_pCampaign->addStageData(data, index.row());

    QStandardItem* item = new QStandardItem(data->toQString());
    m_modelStages->insertRow(index.row() + 1, item);
}

MainWindow::ExecuteResult MainWindow::confirmSaveChanges() {
    if (!m_pCampaign)
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
    QString path = QFileDialog::getSaveFileName(this, tr("Save"), ".", FILE_FILTER, 0, QFileDialog::DontConfirmOverwrite);
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

    QFile file(path);
    if (!file.open(QFile::ReadWrite|QFile::Truncate)) {
        return MainWindow::Error;
    }

    QDataStream stream(&file);
    stream << *m_pCampaign;
    file.flush();
    file.close();

    //reset the campaign's save path to the new path
    m_pCampaign->setSavePath(path);
    m_bChanged = false;     //TODO change the window's title ???
    return MainWindow::DONE;
}

void MainWindow::on_m_btnGenerate_clicked() {
    if (!m_pCurrentEditStage)
        return;

    if (!m_pCurrentEditStage->isSizeValid()) {
        QMessageBox::information(this, tr("Invalid size"), tr("The Sudoku Box size is invalid."));
        return;
    }

    m_pCurrentEditStage->updateData();
    m_pCurrentEditStage->initSolver(SudokuSolver::getInstance());

    refreshSudokuBox();
}
