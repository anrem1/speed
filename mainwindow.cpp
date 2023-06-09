#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <QMessageBox>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::on_normsearch_clicked()
{
    flagnorm = true;
ui->using_2->setText("Using: Normal Search");
for(int i = 0; i<ui->inputsize->text().toInt(); i++) {
    if(ui->find->text().toInt() == ui->list->item(i)->text().toInt()) {
        return 1;
    }
}
return 0;
}

bool MainWindow::on_binsearch_clicked()
{
    flagbin = true;
    ui->using_2->setText("Using: Binary Search");
    int l = v[0];
    int r = v[ui->inputsize->text().toInt()-1];
    int x = ui->find->text().toInt();
    while (l <= r) {
        int m = l + (r - l) / 2;

        // Check if x is present at mid
        if (v[m] == x)
            return 1;

        // If x greater, ignore left half
        if (v[m] < x)
            l = m + 1;

        // If x is smaller, ignore right half
        else
            r = m - 1;
    }

    // if we reach here, then element was
    // not present
    return 0;
}

void MainWindow::genran() {
    v.clear();
    srand(time(0));
    for(int i = 0; i<ui->inputsize->text().toInt(); i++) {
        v.push_back(rand());
    }

}
 void MainWindow::on_gendata_clicked()
{
 ui->list->clear();
  ui->sortuse->setText("Sort using");
  ui->using_2->setText("Using");
  flagbin = false;
  flagnorm = false;
  flagst = false;
  flagm = false;
 genran();
 for(int i = 0; i<ui->inputsize->text().toInt(); i++) {
     ui->list->addItem(QString::number(v[i]));
 }
ui->datasize->setText("The Dataset size is: " + ui->inputsize->text());
ui->arrsort->setText("Unsorted Array");
}

void MainWindow:: merge(std::vector<int>& arr, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;


    std::vector<int> leftArr(leftSize);
    std::vector<int> rightArr(rightSize);

for (int i = 0; i < leftSize; ++i)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < rightSize; ++j)
        rightArr[j] = arr[mid + 1 + j];

int i = 0; // Index of the left subarray
    int j = 0; // Index of the right subarray
    int k = left; // Index of the merged array

    while (i < leftSize && j < rightSize) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            ++i;
        } else {
            arr[k] = rightArr[j];
            ++j;
        }
        ++k;
    }

    while (i < leftSize) {
        arr[k] = leftArr[i];
        ++i;
        ++k;
    }

    while (j < rightSize) {
        arr[k] = rightArr[j];
        ++j;
        ++k;
    }
}

void MainWindow::mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}


void MainWindow::on_sortmerge_clicked()
{
    flagm = true;
    int size = ui->list->count();
    std::vector<int> arr(size);

    for (int i = 0; i < size; ++i)
        arr[i] = ui->list->item(i)->text().toInt();

    mergeSort(arr, 0, size - 1);

    for (int i = 0; i < size; ++i)
        ui->list->item(i)->setText(QString::number(arr[i]));

    ui->arrsort->setText("Array is Sorted");
    ui->sortuse->setText("Sort using Merge Sort");

}

void MainWindow::on_sortstl_clicked()
{
    flagst=true;
    stlsort();
}

void MainWindow::stlsort() {
    std::sort(v.begin(), v.end());
    ui->list->clear();
    for(int i = 0; i<ui->inputsize->text().toInt(); i++) {
      ui->list->addItem(QString::number(v[i]));
    }
    ui->arrsort->setText("Array is Sorted");
    ui->sortuse->setText("Sort using STL Sort");
}

void MainWindow::on_findit_clicked()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    bool performSort = false;

    if (flagbin == true) {
        on_binsearch_clicked();
    }
    else if (flagnorm == true) {
        on_normsearch_clicked();
    }

    std::chrono::steady_clock::time_point then = std::chrono::steady_clock::now();
    int searchtime = std::chrono::duration_cast<std::chrono::nanoseconds>(then - now).count();

    std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();

    if (flagm == true) {
        on_sortmerge_clicked();
        performSort = true;
    }
    else if (flagst == true) {
        performSort = true;
        stlsort();
    }

    std::chrono::steady_clock::time_point t4 = std::chrono::steady_clock::now();
    int sorttime = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();

    QString message;
    if (on_binsearch_clicked() || on_normsearch_clicked()) {
        message += "This item exists.\nTime in ns taken to search: " + QString::number(searchtime) + "\n";
    }
    else {
        message += "This item does not exist.\nTime in ns taken to search: " + QString::number(searchtime) + "\n";
    }

    if (performSort) {
        message += "Time in ns taken to sort: " + QString::number(sorttime);
    }

    QMessageBox::information(this, "Search", message, QMessageBox::Ok);
}


