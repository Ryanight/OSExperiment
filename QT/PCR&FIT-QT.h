#include "pcr.h"
#include "ui_pcr.h"

PCR::PCR(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PCR)
{
    ui->setupUi(this);
    tableWidget = ui->tableWidget;
    tableWidget_2 = ui->tableWidget_2;
    tableWidget_3 = ui->tableWidget_3;
    tableWidget_4 = ui->tableWidget_4;
    tableWidget_5 = ui->tableWidget_5;
    timer = new QTimer(this);   //新建定时器
    if(timer==NULL)
    {
        qDebug("Timer Failed");
    }
    else{
        connect(timer,SIGNAL(timeout()),this,SLOT(on_pushButton_2_clicked()));
        connect(timer,SIGNAL(timeout()),this,SLOT(RefreshTable()));
        //关联定时器计满信号和相应的槽函数
        //每隔1ms执行xytimerUpDate()函数
        timer->setInterval(1000);
    }
}

PCR::~PCR()
{
    timer->stop();//停止计时器
    delete timer;//释放计时器内存
    delete ui;
}

void RefreshNotCreatedTable(QTableWidget *table)
{
    //新建一4行2列的表格
    table->setRowCount(Event.count);
    table->setColumnCount(3);

    //设置表格行标题
    QStringList headerLabels;
    headerLabels << "PID" << "Arrive Time" << "CPU Time";
    table->setHorizontalHeaderLabels(headerLabels);


    //设置表格行标题的对齐方式
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //设置行表题
    table->verticalHeader()->hide();
    //    QStringList rowLabels;
    //    for (int i = 0; i < Event.count; i++)
    //    {
    //        QString s = QString::number(i, 10);
    //        rowLabels << s;
    //    }
    //    table->setVerticalHeaderLabels(rowLabels);


    //设置每一行的高度
    for(int i = 0; i < Event.count; i++)
        table->setRowHeight(i, 22);

    //自动调整最后一列的宽度使它和表格的右边界对齐
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    //设置表格的选择方式
    table->setSelectionBehavior(QAbstractItemView::SelectItems);

    //设置编辑方式
    table->setEditTriggers(QAbstractItemView::DoubleClicked);

    //设置表格的内容
    QueueNode *p = Event.front;
    for (int row = 0; row < Event.count; ++row)
    {
        QTableWidgetItem *item0, *item1, *item2;
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;

        QString txt = QString::number(p->process->ID);
        item0->setText(txt);
        table->setItem(row, 0, item0);

        txt = QString::number(p->data);
        item1->setText(txt);
        table->setItem(row, 1, item1);

        txt = QString::number(p->process->cputime);
        item2->setText(txt);
        table->setItem(row, 2, item2);

        p=p->next;
    }
}

void RefreshReadyTable(QTableWidget *table)
{
    //新建表格
    table->setRowCount(Ready.count);
    table->setColumnCount(3);

    //设置表格行标题
    QStringList headerLabels;
    headerLabels << "PID" << "Arrive Time" << "CPU Time";
    table->setHorizontalHeaderLabels(headerLabels);


    //设置表格行标题的对齐方式
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //    //设置行表题
    table->verticalHeader()->hide();
    //    QStringList rowLabels;
    //    for (int i = 0; i < Ready.count; i++)
    //    {
    //        QString s = QString::number(i, 10);
    //        rowLabels << s;
    //    }
    //    table->setVerticalHeaderLabels(rowLabels);


    //设置每一行的高度
    for(int i = 0; i < Ready.count; i++)
        table->setRowHeight(i, 22);

    //自动调整最后一列的宽度使它和表格的右边界对齐
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    //设置表格的选择方式
    table->setSelectionBehavior(QAbstractItemView::SelectItems);

    //设置编辑方式
    table->setEditTriggers(QAbstractItemView::DoubleClicked);

    //设置表格的内容
    QueueNode *p = Ready.front;
    for (int row = 0; row < Ready.count; ++row)
    {
        QTableWidgetItem *item0, *item1, *item2;
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;

        QString txt = QString::number(p->process->ID);
        item0->setText(txt);
        table->setItem(row, 0, item0);

        txt = QString::number(p->data);
        item1->setText(txt);
        table->setItem(row, 1, item1);

        txt = QString::number(p->process->cputime);
        item2->setText(txt);
        table->setItem(row, 2, item2);

        p=p->next;
    }

}

void RefreshRunningTable(QTableWidget *table)
{
    //新建表格
    table->setRowCount(Running.count);
    table->setColumnCount(3);

    //设置表格行标题
    QStringList headerLabels;
    headerLabels << "PID" << "CPU Time" << "Need Time";
    table->setHorizontalHeaderLabels(headerLabels);


    //设置表格行标题的对齐方式
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //设置行表题
    table->verticalHeader()->hide();
    //    QStringList rowLabels;
    //    for (int i = 0; i < Running.count; i++)
    //    {
    //        QString s = QString::number(i, 10);
    //        rowLabels << s;
    //    }
    //    table->setVerticalHeaderLabels(rowLabels);


    //设置每一行的高度
    for(int i = 0; i < Running.count; i++)
        table->setRowHeight(i, 22);

    //自动调整最后一列的宽度使它和表格的右边界对齐
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    //设置表格的选择方式
    table->setSelectionBehavior(QAbstractItemView::SelectItems);

    //设置编辑方式
    table->setEditTriggers(QAbstractItemView::DoubleClicked);

    //设置表格的内容
    QueueNode *p = Running.front;
    for (int row = 0; row < Running.count; ++row)
    {
        QTableWidgetItem *item0, *item1, *item2;
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;

        QString txt = QString::number(p->process->ID);
        item0->setText(txt);
        table->setItem(row, 0, item0);


        txt = QString::number(p->process->cputime);
        item1->setText(txt);
        table->setItem(row, 1, item1);

        txt = QString::number(p->process->needtime);
        item2->setText(txt);
        table->setItem(row, 2, item2);

        p=p->next;
    }
}

void RefreshFinishedTable(QTableWidget *table)
{
    //新建表格
    table->setRowCount(Finished.count);
    table->setColumnCount(3);

    //设置表格行标题
    QStringList headerLabels;
    headerLabels << "PID" << "Arrive Time" << "Finish Time";
    table->setHorizontalHeaderLabels(headerLabels);


    //设置表格行标题的对齐方式
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //设置行表题
    table->verticalHeader()->hide();
    //    QStringList rowLabels;
    //    for (int i = 0; i < Finished.count; i++)
    //    {
    //        QString s = QString::number(i, 10);
    //        rowLabels << s;
    //    }
    //    table->setVerticalHeaderLabels(rowLabels);


    //设置每一行的高度
    for(int i = 0; i < Finished.count; i++)
        table->setRowHeight(i, 22);

    //自动调整最后一列的宽度使它和表格的右边界对齐
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    //设置表格的选择方式
    table->setSelectionBehavior(QAbstractItemView::SelectItems);

    //设置编辑方式
    table->setEditTriggers(QAbstractItemView::DoubleClicked);

    //设置表格的内容
    QueueNode *p = Finished.front;
    for (int row = 0; row < Finished.count; ++row)
    {
        QTableWidgetItem *item0, *item1, *item2;
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;


        QString txt = QString::number(p->process->ID);
        item0->setText(txt);
        table->setItem(row, 0, item0);

        txt = QString::number(p->data);
        item1->setText(txt);
        table->setItem(row, 1, item1);

        txt = QString::number(p->process->finishtime);
        item2->setText(txt);
        table->setItem(row, 2, item2);

        p=p->next;
    }
}

void RefreshMemoryTable(QTableWidget *table)
{
    //新建表格
    table->setRowCount(MemoryList.size());
    table->setColumnCount(3);

    //设置表格行标题
    QStringList headerLabels;
    headerLabels << "PID" << "Start Point" << "Length";
    table->setHorizontalHeaderLabels(headerLabels);


    //设置表格行标题的对齐方式
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //设置行表题
    table->verticalHeader()->hide();
    //    QStringList rowLabels;
    //    for (int i = 0; i < Finished.count; i++)
    //    {
    //        QString s = QString::number(i, 10);
    //        rowLabels << s;
    //    }
    //    table->setVerticalHeaderLabels(rowLabels);


    //设置每一行的高度
    for(int i = 0; i < MemoryList.size(); i++)
        table->setRowHeight(i, 22);

    //自动调整最后一列的宽度使它和表格的右边界对齐
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    //设置表格的选择方式
    table->setSelectionBehavior(QAbstractItemView::SelectItems);

    //设置编辑方式
    table->setEditTriggers(QAbstractItemView::DoubleClicked);

    //设置表格的内容
    LISTQ::iterator p;
    int row = 0;
    for (p = MemoryList.begin(); p != MemoryList.end(); p++)
    {
        QTableWidgetItem *item0, *item1, *item2;
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;


        QString txt = QString::number(p->process->ID);
        item0->setText(txt);
        table->setItem(row, 0, item0);

        txt = QString::number(p->process->memory_start);
        item1->setText(txt);
        table->setItem(row, 1, item1);

        txt = QString::number(p->process->memory_length);
        item2->setText(txt);
        table->setItem(row, 2, item2);

        row++;
    }
}


void PCR::RefreshTable()
{
    qDebug("Refreshing Table");
    RefreshNotCreatedTable(tableWidget);
    RefreshReadyTable(tableWidget_2);
    RefreshRunningTable(tableWidget_3);
    RefreshFinishedTable(tableWidget_4);
    RefreshMemoryTable(tableWidget_5);
}

void PCR::on_pushButton_clicked()
{
    timer->stop();
    realtime = 0;
    InitQueue(&Event);
    InitQueue(&Ready);
    InitQueue(&Finished);
    InitQueue(&Running);
    EventCreate();
    int MemorySize = 64;
    OccupySize = 12;
    InitMemory(MemorySize, OccupySize);
    GetFreeChart();
    qDebug("Count: %d", Event.count);
    ui->radioButton->setEnabled(true);
    ui->radioButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_2->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->pushButton->setEnabled(false);
    ui->radioButton_3->setEnabled(true);
    ui->radioButton_4->setEnabled(true);
    ui->radioButton_5->setEnabled(true);
    //RefreshNotCreatedTable(ui->tableWidget);
}

void PCR::on_pushButton_2_clicked()
{
    string METHOD;
    METHOD = "FCFS";
    QueueNode *p = Event.front;
    if (!QueueEmpty(&Event) || !QueueEmpty(&Ready) || !QueueEmpty(&Running))
    {
        p = Event.front;
        for (int i = 0; i < Event.count; i++){
            if (p != NULL && realtime >= p->data)
            {
                int Mem;
                if(ui->radioButton_3->isChecked())
                    Mem = FirstFit(p);
                else if (ui->radioButton_4->isChecked())
                    Mem = BestFit(p);
                else if (ui->radioButton_5->isChecked())
                    Mem = WorstFit(p);
                if (Mem == 1) {
                    p = p->next;
                    ExLinkQueue(&Ready, &Event);
                    Ready.rear->process->state = 1;
                }
                else
                    p = p->next;
            }
            else if(p != NULL && p->next != NULL)
                p = p->next;
        }
        if (!QueueEmpty(&Ready) || !QueueEmpty(&Running))
        {
            if (ui->radioButton->isChecked())
            {
FCFSAG:
                FCFS();
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data)
                    {
                        int Mem;
                        if(ui->radioButton_3->isChecked())
                            Mem = FirstFit(p);
                        else if (ui->radioButton_4->isChecked())
                            Mem = BestFit(p);
                        else if (ui->radioButton_5->isChecked())
                            Mem = WorstFit(p);
                        if (Mem == 1) {
                            p = p->next;
                            ExLinkQueue(&Ready, &Event);
                            Ready.rear->process->state = 1;
                        }
                        else
                            p = p->next;
                    }
                    else if(p != NULL && p->next != NULL)
                        p = p->next;
                }
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data && MemoryLeft() >= p->process->memory_length) {
                        goto FCFSAG;
                    }
                    else
                        p = p->next;
                }
                if (QueueEmpty(&Running) && !QueueEmpty(&Ready)) {
                    goto RRAG;
                }
                cout<<"time: "<<realtime<<endl;
                cout<<"Running ";
                ReadLinkQueue(&Running);
                cout<<"Ready ";
                ReadLinkQueue(&Ready);
                cout<<"Not Created ";
                ReadLinkQueue(&Event);
                cout<<"Finished ";
                ReadLinkQueue(&Finished);
                QString s = QString::number(realtime);
                ui->lineEdit->setText(s);
                realtime++;
                //goto ADD;

            }
            else if (ui->radioButton_2->isChecked())
            {
RRAG:
                RR();
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data)
                    {
                        int Mem;
                        if(ui->radioButton_3->isChecked())
                            Mem = FirstFit(p);
                        else if (ui->radioButton_4->isChecked())
                            Mem = BestFit(p);
                        else if (ui->radioButton_5->isChecked())
                            Mem = WorstFit(p);
                        if (Mem == 1) {
                            p = p->next;
                            ExLinkQueue(&Ready, &Event);
                            Ready.rear->process->state = 1;
                        }
                        else
                            p = p->next;
                    }
                    else if(p != NULL && p->next != NULL)
                        p = p->next;
                }
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data && MemoryLeft() >= p->process->memory_length) {
                        goto FCFSAG;
                    }
                    else
                        p = p->next;
                }
                if (QueueEmpty(&Running) && !QueueEmpty(&Ready)) {
                    goto RRAG;
                }
                cout<<"time: "<<realtime<<endl;
                cout<<"Running ";
                ReadLinkQueue(&Running);
                cout<<"Ready ";
                ReadLinkQueue(&Ready);
                cout<<"Not Created ";
                ReadLinkQueue(&Event);
                cout<<"Finished ";
                ReadLinkQueue(&Finished);
                QString s = QString::number(realtime);
                ui->lineEdit->setText(s);
                realtime++;
                //goto ADD;
            }
            else
                cout<<"Select a Method."<<endl;
        }
        else
            cout<<"No Progress to Work."<<endl;
    }
    //getchar();
    else
        return;
}

void PCR::on_pushButton_3_clicked()
{
//    QString s = QString::number(realtime);
//    ui->lineEdit->setText(s);
    if(timer->isActive())
    {timer->stop();}
    else
    {timer->start();}
    if(ui->radioButton_2->isChecked())
    {
        QString str = ui->lineEdit_2->text();
        timeslice_origin = str.toInt();
        timeslice = timeslice_origin;
        cout<<"timeslice"<<timeslice;
    }
}

void PCR::on_radioButton_2_clicked()
{
    if(ui->radioButton_2->isChecked())
    {
        ui->lineEdit_2->setVisible(true);
        ui->lineEdit_2->setEnabled(true);
    }
    else
    {
        ui->lineEdit_2->setVisible(false);
        ui->lineEdit_2->setEnabled(false);
    }
}

void PCR::on_radioButton_clicked()
{
    if(!ui->radioButton_2->isChecked())
    {
        ui->lineEdit_2->setVisible(false);
        ui->lineEdit_2->setEnabled(false);
    }
    else
        return;
}
