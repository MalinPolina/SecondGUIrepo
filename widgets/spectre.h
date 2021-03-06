#ifndef SPECTRE_H
#define SPECTRE_H

#include <other/coord.h>
#include <other/matrix.h>

#include <QWidget>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QtSvg>


#define SPECTRA_EXP_NAME_SPECIFICITY 0


namespace Ui {
    class Spectre;
}

class Spectre : public QWidget
{
    Q_OBJECT

public:
    explicit Spectre();
    ~Spectre();
    void setSmooth(int);
    bool eventFilter(QObject *obj, QEvent *event);
    void defaultState();
    void setPow(double);
    void setFftLength(int);

	void writeSpectra(const double leftFreq = def::leftFreq,
					  const double rightFreq = def::rightFreq);

    void setInPath(const QString &);
    void setOutPath(const QString &);
    void countSpectra();
    bool countOneSpectre(matrix & data2,
                         matrix & dataFFT);
    void cleanSpectra();


public slots:
    void countSpectraSlot();
    void inputDirSlot();
    void outputDirSlot();
    void setFftLengthSlot();

    void setLeft();
    void setRight();
    void drawWavelets();
    void compare();
    void psaSlot();
    void integrate();
    void center();

private:
    Ui::Spectre * ui;
    QString backupDirPath;

	/// replace in code
	QString defaultInPath = def::dirPath()
                            + "/" +"Reals";
	QString defaultOutPath = def::dirPath()
                             + "/" +"SpectraSmooth";

	QString defaultInPathW = def::dirPath()
                             + "/winds"
                             + "/fromreal";
	QString defaultOutPathW = def::dirPath()
                             + "/SpectraSmooth"
                             + "/winds";

    std::vector<std::pair<int, int>> rangeLimits;

    std::vector<matrix> dataFFT;
    std::vector<QString> fileNames;
};

#endif // SPECTRE_H
