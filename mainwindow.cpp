#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QString>
#include <vector>
#include <QTime>
#include <QTimer>
#include <QMessageBox>


int states_num, symbols_num;

int tape_size = 100000, head = tape_size / 2;

int current_state = 0;

std::vector <QString> symbols = {"-"};

void Clean(Ui::MainWindow*);

void FindHead(Ui::MainWindow* ui, int new_head) {
    ui -> table_tape -> item(0, head) -> setBackground(QColor(Qt::white));
    ui -> table_tape -> item(0, new_head) -> setBackground(QColor(Qt::yellow).lighter(170));

    QTableWidgetItem* tmp_itm1 = ui -> table_tape -> item(0, new_head + 5);
    ui -> table_tape->scrollToItem(tmp_itm1);


    QTableWidgetItem* tmp_itm2 = ui -> table_tape -> item(0, new_head - 5);
    ui -> table_tape->scrollToItem(tmp_itm2);
}


void MainWindow::on_button_new_symbol_clicked()
{

    QString elem = ui->textEdit ->toPlainText();
    if (elem == "")
        return;
    symbols_num++;
    ui->table_states->setColumnCount(symbols_num);
    if (elem.size() > 1){
        elem = elem[0];
    }
    symbols.push_back(elem);
    QTableWidgetItem* itm =  new QTableWidgetItem(elem);
    ui -> table_states ->setHorizontalHeaderItem(symbols_num - 1, itm);
    ui -> textEdit ->clear();
}

void MainWindow::on_button_new_state_clicked()
{
    QString elem = 'q' + QString::number(states_num);
    QTableWidgetItem* itm = new QTableWidgetItem(elem);
    states_num++;
    ui->table_states->setRowCount(states_num);
    ui->table_states ->setVerticalHeaderItem(states_num - 1, itm);
}

int FindSymbol(Ui::MainWindow* ui) {
    for (int i = 0; i < symbols_num; ++i) {
       QString symb = symbols[i];
       QTableWidgetItem* itm = ui -> table_tape -> item(0, head);
       if (itm ->text() == symb)
           return i;
    }
    return -1;
}

void UpdateHead(Ui::MainWindow* ui, int new_head) {
    ui -> table_tape -> item(0, head) -> setBackground(QColor(Qt::white));
    ui -> table_tape -> item(0, new_head) -> setBackground(QColor(Qt::yellow).lighter(170));
}

void MainWindow::FollowMyRules() {
    int symb_num = FindSymbol(ui);
    if (symb_num == -1) {
        timer->stop();
        QMessageBox::critical(0, "Error", "No such symbol");
        return;
    }
    QTableWidgetItem* itm = ui -> table_states -> item(current_state, symb_num);
    QString rule = itm ->text();


    for (int i = 0; i < rule.size(); ++i) {
        if (rule[i] == '!') {
            timer->stop();
            QMessageBox::information(0, "Information", "Program finished");
            return;
        }
        else if (rule[i] == '<') {
            if (head <= 0) {
                timer->stop();
                QMessageBox::critical(0, "Error", "Program works too long");
                return;
            }
            UpdateHead(ui, head - 1);
            head--;
            FindHead(ui, head);
        } else if (rule[i] == '>') {
            if (head >= tape_size-1) {
                timer->stop();
                QMessageBox::critical(0, "Error", "Program works too long");
                return;
            }
            UpdateHead(ui, head + 1);
            head++;
            FindHead(ui, head);
        } else if (rule[i] == 'q' &&
                   i + 1 < rule.size() && rule[i+1] <= '9' && rule[i+1] >= '0') {
            int next_state = 0;
            ++i;
            while (i < rule.size() && rule[i] <= '9' && rule[i] >= '0') {
                next_state = next_state * 10 + (int)(rule[i].unicode() - (int)'0');
                ++i;
            }
            if (next_state >= states_num) {
                timer->stop();
                QMessageBox::critical(0, "Error", "Wrong Rule.\nNo such state");
                return;
            }
            current_state = next_state;
        } else {
           ui -> table_tape -> item(0, head) ->setText(QString(rule[i]));
        }
    }

    ui -> table_tape -> repaint();
}

void MainWindow::on_button_start_clicked()
{
    timer->start();
    FollowMyRules();

 }

void MainWindow::on_table_states_itemChanged(QTableWidgetItem *item)
{
    item -> setTextAlignment(Qt::AlignCenter);
}
void Clean(Ui::MainWindow* ui) {
    ui -> table_states -> clear();
    ui -> table_states -> setRowCount(1);
    ui -> table_states -> setColumnCount(1);
    QTableWidgetItem* itm1 =  new QTableWidgetItem("q0");
    ui -> table_states -> setVerticalHeaderItem(0,itm1);
    QTableWidgetItem* itm2 =  new QTableWidgetItem("-");
    ui -> table_states -> setHorizontalHeaderItem(0,itm2);


    for (int i = 0; i< tape_size; ++i) {
        QTableWidgetItem *itm = new QTableWidgetItem("-");
        itm -> setTextAlignment(Qt::AlignCenter);
        ui -> table_tape -> setItem(0, i, itm);
    }

    FindHead(ui, tape_size / 2);
    UpdateHead(ui, tape_size/2);
    head = tape_size / 2;
    states_num = 1;
    symbols_num = 1;
    current_state = 0;


    symbols.clear();
    symbols.push_back("-");
}

void MainWindow::on_button_new_program_clicked()
{
    Clean(ui);
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer();
    timer->setInterval(1000);
    ui ->spinBox_speed ->setPrefix("×");
    connect(timer, SIGNAL(timeout()), this, SLOT(FollowMyRules()));
    QMessageBox::information(0, "Information", "Reserved symbols:\n"
                                               "      !      terminate\n"
                                               "      -      empty cell\n"
                                               "      <     move tape one square left     \n"
                                               "      >     move tape one square right                     ");


    ui -> table_tape -> setRowCount(1);
    ui -> table_tape -> setColumnCount(tape_size);


    Clean(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_speed_valueChanged(int arg1)
{
    timer->setInterval(1000/arg1);
}

void MainWindow::on_button_stop_clicked()
{
    timer->stop();
}

void MainWindow::on_button_clean_tape_clicked()
{
    for (int i = 0; i< tape_size; ++i) {
        ui ->table_tape ->item(0,i) ->setText("-");
    }
    current_state = 0;
}

void MainWindow::on_table_tape_itemChanged(QTableWidgetItem *item)
{
    item ->setText((QString)item->text()[0]);
}
