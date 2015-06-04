#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCampaignData;
class QStageData;

class QStandardItemModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void onCampaignDataLoaded();
    void refreshView();
    void refreshSelectedStageDescInList();
    void refreshSudokuBox();

    enum ExecuteResult{
        DONE = 0,
        Canceled,
        Error
    };

    MainWindow::ExecuteResult doSave();
    MainWindow::ExecuteResult doSaveAs();
    MainWindow::ExecuteResult saveToFile(const QString& path);
    MainWindow::ExecuteResult confirmSaveChanges();

private:
    Ui::MainWindow *ui;

    QSize m_sizeGrid;
    QSize m_sizeBox;
    bool m_bChanged;

    QCampaignData* m_pCampaign;
    QStageData* m_pCurrentEditStage;

    QStandardItemModel* m_modelStages;
    QStandardItemModel* m_modelSudoku;
    QStandardItemModel* m_modelRes;

private slots:
    void on_action_New_triggered();

    void on_m_lvStages_clicked(const QModelIndex &index);
    void on_m_tbSudokuBox_clicked(const QModelIndex &index);
    void on_m_spbRowsPerGrid_valueChanged(int value);
    void on_m_spbColsPerGrid_valueChanged(int arg1);
    void on_m_spbGridsInRow_valueChanged(int arg1);
    void on_m_spbGridsInCol_valueChanged(int arg1);
    void on_action_Open_triggered();
    void on_action_Save_triggered();
    void on_actionSave_as_triggered();
    void on_action_Add_a_stage_triggered();
    void on_actionMove_up_stage_triggered();
    void on_actionMove_dow_n_stage_triggered();
    void on_actionDel_stage_triggered();
    void on_actionAdd_resource_triggered();
    void on_actionDel_resource_triggered();
    void on_actionDuplicate_selected_stage_triggered();
    void on_m_btnGenerate_clicked();
};

#endif // MAINWINDOW_H
