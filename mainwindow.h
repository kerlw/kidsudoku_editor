#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCampaignData;

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
    Ui::MainWindow *ui;

    QSize m_sizeGrid;
    QSize m_sizeBox;
    bool m_bChanged;

    QCampaignData* m_pData;

private slots:
    void on_action_New_triggered();


};

#endif // MAINWINDOW_H
