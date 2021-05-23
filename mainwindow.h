#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_new_symbol_clicked();

    void on_button_new_state_clicked();

    void on_button_start_clicked();

    void on_table_states_itemChanged(QTableWidgetItem *item);

    void on_button_new_program_clicked();

    void on_textEdit_copyAvailable(bool b);

   // void keyPressEvent(QKeyEvent *e);
private slots:
    void FollowMyRules();
    void on_spinBox_speed_valueChanged(const QString &arg1);

    void on_spinBox_speed_valueChanged(int arg1);

    void on_button_stop_clicked();

    void on_button_clean_tape_clicked();

    void on_table_tape_itemChanged(QTableWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
};
#endif // MAINWINDOW_H
