#ifndef SPECTRE_H
#define SPECTRE_H

#include <QWidget>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QtSvg>
#include "mainwindow.h"

#include "coord.cpp"
using namespace coords;


namespace Ui {
    class Spectre;
}

class Spectre : public QWidget
{
    Q_OBJECT

public:
    explicit Spectre(QDir *dir_ = new QDir("/"), int ns_ = 20, QString ExpName_ = "TSL");
    ~Spectre();
    int readFile(int &num, double ** dataFFT);
    void setSmooth(int);
    bool eventFilter(QObject *obj, QEvent *event);
    int readFilePhase(int &num, double ***dataPhase);
    void defaultState();



public slots:
    void countSpectra();
    void inputDirSlot();
    void outputDirSlot();
    void setFftLength();
    void setLeft();
    void setRight();
    void drawWavelets();
    void compare();
    void psaSlot();
    void integrate();
    void specifyRange();
    void center();

signals:
    void spValues(int, int, double);

private:
    Ui::Spectre *ui;
    QString ExpName;
    int ns;
    QDir *dir, *dirBC;
    int fftLength;
    QString helpString;
    QStringList lst, nameFilters;
    int left;
    int right;
    double spStep;
    int NumOfSlices;
    int Eyes;
    int helpInt;
    QFileDialog * browser1;
    QFileDialog * browser2;
    int spLength;
    QPixmap pic;
    QPainter *paint;
    bool ** boolArrrPirate;
    int ** rangeLimits;
    double norm;
    struct complex
    {
      double r;
      double i;
    }* spect;
    time_t duration;
    int chanNum;
    QString rangePicPath;
    FILE * file1;


};

#endif // SPECTRE_H
