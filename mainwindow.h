#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCampaignData;
class QStageData;

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
    void refreshSudokuBox();

private:
    Ui::MainWindow *ui;

    QSize m_sizeGrid;
    QSize m_sizeBox;
    bool m_bChanged;

    QCampaignData* m_pCampaign;
    QStageData* m_pCurrentEditStage;

private slots:
    void on_action_New_triggered();

    void on_m_lvStages_clicked(const QModelIndex &index);
    void on_m_tbSudokuBox_clicked(const QModelIndex &index);
    void on_m_spbRowsPerGrid_valueChanged(int value);
    void on_m_spbColsPerGrid_valueChanged(int arg1);
    void on_m_spbGridsInRow_valueChanged(int arg1);
    void on_m_spbGridsInCol_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
