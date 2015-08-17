#ifndef NET_H
#define NET_H

//#include <QtWidgets>
#include <QWidget>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <QFileDialog>
#include <cmath>
#include <time.h>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include "makepa.h"
#include "cfg.h"
#include "qtempevent.h"
#include "tempthread.h"
#include "library.h"
#include <fstream>
#include <ios>
#include <unistd.h>
#include <QTime>

using namespace std;

#include <QMessageBox>


namespace Ui {
    class Net;
}

class Net : public QWidget
{
    Q_OBJECT

//    friend class MainWindow;

public:
    explicit Net();
    ~Net();
    bool ClassificateVector(int &vecNum);
    void closeLogFile();
    void setAutoProcessingFlag(bool);
    void prelearnDeepBelief();

    bool adjustReduceCoeff(QString spectraDir, int lowLimit, int highLimit, MakePa * outMkPa, QString paFileName = "1");
    int getEpoch();
    void setErrcrit(double);
    double getErrcrit();

    void Sammon(double ** distArr, int size, int * colors);
    void Kohonen(double ** input, double ** eigenVects, double * averageProjection, int size, int length);
    void moveCoordsGradient(double ** coords, double ** distOld, double ** distNew, int size);
    double thetalpha(int bmu_, int j_, int step_, double ** arr, int length_);

    void loadCfgByName(QString FileName);
    double setPercentageForClean();
    double mouseClick(QLabel * label, QMouseEvent * ev);
    void leaveOneOut();
    double getAverageAccuracy();
    void setReduceCoeff(double coeff);
    double getReduceCoeff();
    void setNumOfPairs(int num);
    void saveWts(QString wtsPath);
    void PaIntoMatrixByName(QString fileName);
    void autoClassification(QString spectraDir);
    void averageClassification();

protected:
    void mousePressEvent(QMouseEvent * event);

public slots:
    void loadCfg();
    void loadWts();


    void LearnNet();

    void tall();
    void reset();
    void saveWtsSlot();
    void stopActivity();
    void drawWts(QString wtsPath = QString(),
                 QString picPath = QString());
    void PaIntoMatrix();
    void leaveOneOutSlot();
    void neuronGas();
    void pca();
    void drawSammon();
    void autoClassificationSimple();
    void autoPCAClassification();
    void SVM();
    void Hopfield();
    void methodSetParam(int, bool);
    void memoryAndParamsAllocation();
    void testDistances();
    void optimizeChannelsSet();
    void adjustParamsGroup2(QAbstractButton*);

private:
    Ui::Net *ui;

    QButtonGroup  * group1,  * group2,  * group3;
    QDir *dirBC;
    double ** dataMatrix;
    double *** weight;
    int * dimensionality; //for backprop
//    double * output;
    char * helpCharArr;
    double * classCount;

//    double * tempRandoms;
    double ** tempRandomMatrix; //test linear transform

    QVector<int> channelsSet;
    QVector<int> channelsSetExclude;
    double averageAccuracy;

    QPixmap pic;
    QPainter *paint;

    int numOfLayers; //for backprop
    int numTest;
    int epoch;
    int  NetLength; //inputs
    int NumOfClasses; //outputs
    int NumberOfVectors;
    bool autoFlag;
    double critError;
    double currentError;
    double temperature;
    double learnRate;
    double Error;

    int * NumberOfErrors;
    char **FileName;

    QPixmap columns;

    int numOfTall;
    int loadPAflag;
    bool stopFlag;
    QString tmp, paFileBC;

    double ** coords; //new coords for Sammon method
    double mouseShit;

public:

    void loadWtsByName(QString,
                       double * *** wtsMatrix = nullptr);
};

#endif // NET_H
