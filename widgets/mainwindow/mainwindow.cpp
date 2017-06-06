#include <widgets/mainwindow.h>
#include "ui_mainwindow.h"

#include <myLib/drw.h>
#include <myLib/signalProcessing.h>
#include <myLib/mati.h>
#include <myLib/general.h>
#include <myLib/dataHandlers.h>

using namespace myOut;

MainWindow::MainWindow() :
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Main");
    setlocale(LC_NUMERIC, "C");

    autoProcessingFlag = false;
	stdOutBuf = std::cout.rdbuf();

    staSlice = 0;
    stopFlag = 0;

    def::dir->cd(QDir::root().absolutePath());

    QString helpString;

    // what with deletion?
    QButtonGroup * group1, *group2;
	group1 = new QButtonGroup();
    group2 = new QButtonGroup();
    group2->addButton(ui->windsButton);
	group2->addButton(ui->realsButton);

    ui->drawCoeffSpinBox->setValue(1.0); //draw coeff
    ui->drawCoeffSpinBox->setSingleStep(0.1); //draw coeff
	ui->sliceCheckBox->setChecked(true);
	ui->progressBar->setValue(0);

	ui->drawDirBox->addItem("Reals");
    ui->drawDirBox->addItem("cut");
	ui->drawDirBox->addItem("winds");

    ui->drawDirBox->addItem("SpectraSmooth");
	ui->drawDirBox->addItem("SpectraSmooth/winds");
	ui->drawDirBox->addItem("winds/SpectraSmooth"); /// tcpClient test
	ui->drawDirBox->addItem("winds/fromreal"); //generality

    /// set fileMarks lineEdit
    helpString.clear();
    for(const QString & fileMark : def::fileMarkers)
    {
        helpString += fileMark + "; ";
    }
    helpString.resize(helpString.size() - 2); // remove the last "; "
    ui->fileMarkersLineEdit->setText(helpString);


    int helpInt = 0;
    QVariant var;
    /*
    ui->reduceChannelsComboBox->addItem("old En->real-time");   //encephalan for real time
    var = QVariant("19 18 16 14 11 9 6 4 2 1 17 13 12 8 7 3 15 10 5 24");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    ui->reduceChannelsComboBox->addItem("En-19");   //encephalan w/o veget channels
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 22");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    ui->reduceChannelsComboBox->addItem("LN->En");
    var = QVariant("1 2 11 3 17 4 12 13 5 18 6 14 15 7 19 8 16 9 10");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    ui->reduceChannelsComboBox->addItem("Boris Nt->En");
    var = QVariant("1 3 7 4 5 6 8 25 14 15 16 26 27 20 21 22 28 29 31");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);
*/

    //0
    ui->reduceChannelsComboBox->addItem("MichaelBak");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 23 24 31");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //1
    ui->reduceChannelsComboBox->addItem("MichaelBakNew");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 23 24 27");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //2
    ui->reduceChannelsComboBox->addItem("MichaelBakNewNoEyes");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 27");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //3
    ui->reduceChannelsComboBox->addItem("MyCurrent");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 22 23 24");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //4
    ui->reduceChannelsComboBox->addItem("MyCurrentNoEyes");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 24");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //5
    ui->reduceChannelsComboBox->addItem("NewEnceph");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 22-23-20 24 25");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //6
    ui->reduceChannelsComboBox->addItem("NewEncephNoEyes");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 25");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //7
    ui->reduceChannelsComboBox->addItem("Mati");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 22 23 28");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //8
    ui->reduceChannelsComboBox->addItem("MatiNoEyes");
    var = QVariant("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 28");
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //9
    ui->reduceChannelsComboBox->addItem("MatiAmod");
    helpString.clear();
    for(int i = 0; i < 41; ++i)
    {
        helpString += QString::number(i+1) + " ";
    }
    var = QVariant(helpString);
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //10
    ui->reduceChannelsComboBox->addItem("MatiAmodNoEyes");
    helpString.clear();
    for(int i = 0; i < 41; ++i)
    {
        helpString += QString::number(i+1) + " ";
    }
    helpString.remove(" 22 23");
    var = QVariant(helpString);
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //11
    ui->reduceChannelsComboBox->addItem("20");
    helpString.clear();
    for(int i = 0; i < 20; ++i)
    {
        helpString += QString::number(i+1) + " ";
    }
    var = QVariant(helpString);
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //12
    ui->reduceChannelsComboBox->addItem("22");
    helpString.clear();
    for(int i = 0; i < 22; ++i)
    {
        helpString += QString::number(i+1) + " ";
    }
    var = QVariant(helpString);
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    //13
    ui->reduceChannelsComboBox->addItem("22NoEyes");
    helpString.clear();
    for(int i = 0; i < 19; ++i)
    {
        helpString += QString::number(i+1) + " ";
    }
    helpString += "22";
    var = QVariant(helpString);
    ui->reduceChannelsComboBox->setItemData(helpInt++, var);

    ui->reduceChannelsComboBox->setCurrentText("MyCurrent");
//    ui->reduceChannelsComboBox->setCurrentText("20");
    ui->reduceChannelsLineEdit->setText(ui->reduceChannelsComboBox->itemData(ui->reduceChannelsComboBox->currentIndex()).toString());


    /// seconds !!!!!
    ui->timeShiftSpinBox->setMinimum(0.1);
    ui->timeShiftSpinBox->setMaximum(32);
    ui->timeShiftSpinBox->setValue(0.5);
    ui->timeShiftSpinBox->setSingleStep(0.1);

    ui->windowLengthSpinBox->setMaximum(32);
    ui->windowLengthSpinBox->setMinimum(1);
    ui->windowLengthSpinBox->setSingleStep(0.1);
    ui->windowLengthSpinBox->setValue(4);
    ui->realsButton->setChecked(true);

    if(def::OssadtchiFlag)
    {
        ui->timeShiftSpinBox->setValue(10);
        ui->windowLengthSpinBox->setValue(10);
    }

	ui->highFreqFilterDoubleSpinBox->setMaximum(500);
	ui->lowFreqFilterDoubleSpinBox->setMaximum(500);

    ui->numOfIcSpinBox->setMaximum(19); //generality
    ui->numOfIcSpinBox->setMinimum(2);
    ui->numOfIcSpinBox->setValue(19);

    ui->numComponentsSpinBox->setMaximum(19);
    ui->numComponentsSpinBox->setValue(10);

    ui->svdDoubleSpinBox->setDecimals(1);
    ui->svdDoubleSpinBox->setMaximum(15);
    ui->svdDoubleSpinBox->setMinimum(2);
    ui->svdDoubleSpinBox->setValue(4.0);
    ui->svdDoubleSpinBox->setValue(9.0);
    ui->svdDoubleSpinBox->setSingleStep(0.5);

    ui->vectwDoubleSpinBox->setDecimals(1);
    ui->vectwDoubleSpinBox->setMaximum(15.0);
    ui->vectwDoubleSpinBox->setMinimum(1.5);
    ui->vectwDoubleSpinBox->setValue(12.0);
    ui->vectwDoubleSpinBox->setSingleStep(0.5);

	ui->cleanRealsCheckBox->setChecked(true);
    ui->cleanRealsSpectraCheckBox->setChecked(true);
	ui->cleanWindsCheckBox->setChecked(false);
	ui->cleanWindsSpectraCheckBox->setChecked(true);
	ui->cleanRealsSignalsCheckBox->setChecked(true);
	ui->cleanWindsSignalsCheckBox->setChecked(true);
	ui->cleanFromRealsCheckBox->setChecked(true);
    ui->cleanMarkersCheckBox->setChecked(true);

    ui->cleanSpectraImgCheckBox->setChecked(true);

    ui->highFreqFilterDoubleSpinBox->setValue(40.);
    ui->highFreqFilterDoubleSpinBox->setSingleStep(1.0);
    ui->lowFreqFilterDoubleSpinBox->setValue(3.5);
    ui->lowFreqFilterDoubleSpinBox->setSingleStep(0.1);

    ui->rereferenceDataComboBox->addItem("A1");
    ui->rereferenceDataComboBox->addItem("A2");
    ui->rereferenceDataComboBox->addItem("Ar");
    ui->rereferenceDataComboBox->addItem("N");
    ui->rereferenceDataComboBox->addItem("Base");
    ui->rereferenceDataComboBox->setCurrentText("Ar");

    ui->matiPieceLengthSpinBox->setMaximum(64);
    ui->matiPieceLengthSpinBox->setMinimum(4);
    ui->matiPieceLengthSpinBox->setValue(16);
    ui->matiCheckBox->setChecked(def::matiFlag);
    ui->markerBinTimeSpinBox->setMaximum(250*60*60*2);   //2 hours
    ui->markerSecTimeDoubleSpinBox->setMaximum(60*60*2); //2 hours

    ui->roundOffsetCheckBox->setChecked(true); // for mati constructEDF

#if SHOW_MATI_WIDGETS
	/// mati
	QObject::connect(ui->matiPreprocessingPushButton, SIGNAL(clicked()), this, SLOT(matiPreprocessingSlot()));
	QObject::connect(ui->markerGetPushButton, SIGNAL(clicked()), this, SLOT(markerGetSlot()));
	QObject::connect(ui->markerSetPushButton, SIGNAL(clicked()), this, SLOT(markerSetSlot()));
	QObject::connect(ui->markerBinTimeSpinBox, SIGNAL(valueChanged(int))this, SLOT(markerSetSecTime(int)));

//	QObject::connect(ui->markerDecimalLineEdit, SIGNAL(returnPressed()), this, SLOT(markerSetBinValueSlot()));
	QObject::connect(ui->markerDecimalLineEdit, SIGNAL(textChanged(QString)), this, SLOT(markerSetBinValueSlot()));
//	QObject::connect(ui->markerBin0LineEdit, SIGNAL(returnPressed()), this, SLOT(markerSetDecValueSlot()));
	QObject::connect(ui->markerBin0LineEdit, SIGNAL(textChanged(QString)), this, SLOT(markerSetDecValueSlot()));
//	QObject::connect(ui->markerBin1LineEdit, SIGNAL(returnPressed()), this, SLOT(markerSetDecValueSlot()));
	QObject::connect(ui->markerBin1LineEdit, SIGNAL(textChanged(QString)), this, SLOT(markerSetDecValueSlot()));

	QObject::connect(ui->markerSaveEdfPushButton, SIGNAL(clicked()), this, SLOT(markerSaveEdf()));
	QObject::connect(ui->matiCheckBox, &QCheckBox::stateChanged,
					 [this](int a) { this->globalEdf.setMatiFlag(a); });
#else
	for(int i = 0; i < ui->matiGridLayout->count(); ++i)
	{
		auto * a = ui->matiGridLayout->itemAt(i)->widget();
		if(a) { a->hide(); }
	}
	ui->matiPieceLengthSpinBox->hide();
	ui->matiCheckBox->hide();
	ui->matiPreprocessingPushButton->hide();
	ui->matiPieceLenlabel->hide();
#endif


	/// open other widgets
    QObject::connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(setEdfFileSlot()));
	QObject::connect(ui->cut_e, SIGNAL(clicked()), this, SLOT(showCut()));
	QObject::connect(ui->netButton, SIGNAL(clicked()), this, SLOT(showNet()));
	QObject::connect(ui->countSpectra, SIGNAL(clicked()), this, SLOT(showCountSpectra()));

	/// small things, ~constant edf
	QObject::connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop()));
	QObject::connect(ui->fileMarkersLineEdit, SIGNAL(returnPressed()), this, SLOT(setFileMarkers()));
	QObject::connect(ui->cleanDirsButton, SIGNAL(clicked()), this, SLOT(cleanDirs()));
	QObject::connect(ui->cleanDirsCheckAllButton, &QPushButton::clicked,
					 [this](){ this->cleanDirsAll(true); });
	QObject::connect(ui->cleanDirsUncheckAllButton, &QPushButton::clicked,
					 [this](){ this->cleanDirsAll(false); });
	QObject::connect(ui->drawButton, SIGNAL(clicked()), this, SLOT(drawDirSlot()));
	QObject::connect(ui->drawMapsPushButton, SIGNAL(clicked()), this, SLOT(drawMapsSlot()));
	QObject::connect(ui->eyesButton, SIGNAL(clicked()), this, SLOT(processEyes()));

	/// slice
	QObject::connect(ui->cutEDF, SIGNAL(clicked()), this, SLOT(sliceAll()));

	/// deprecated
//    QObject::connect(ui->windFromRealButton, SIGNAL(clicked()), this, SLOT(sliceWindFromReal()));

	/// process edf
	QObject::connect(ui->icaPushButton, SIGNAL(clicked()), this, SLOT(ICA()));
	/// to deprecate
	QObject::connect(ui->constructEdfButton, SIGNAL(clicked()), this, SLOT(constructEDFSlot()));

	/// edit edf
	QObject::connect(ui->reduceChannelsComboBox, SIGNAL(highlighted(int)),
					 this, SLOT(changeRedNsLine(int)));
	QObject::connect(ui->reduceChannelsComboBox, SIGNAL(currentIndexChanged(int)),
					 this, SLOT(changeRedNsLine(int)));
	QObject::connect(ui->cleanEdfFromEyesButton, SIGNAL(clicked()),
					 this, SLOT(cleanEdfFromEyesSlot()));
	QObject::connect(ui->reduceChannesPushButton, SIGNAL(clicked()), this, SLOT(reduceChannelsSlot()));
    QObject::connect(ui->refilterDataPushButton, SIGNAL(clicked()), this, SLOT(refilterDataSlot()));
    QObject::connect(ui->reduceChannelsNewEDFPushButton, SIGNAL(clicked()), this, SLOT(reduceChannelsEDFSlot()));
    QObject::connect(ui->rereferenceDataPushButton, SIGNAL(clicked()), this, SLOT(rereferenceDataSlot()));



    globalEdf.setMatiFlag(ui->matiCheckBox->isChecked());
    customFunc();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void QWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

void MainWindow::changeRedNsLine(int a)
{
	ui->reduceChannelsLineEdit->setText(ui->reduceChannelsComboBox->itemData(a).toString());
}

void MainWindow::showCountSpectra()
{
    Spectre *sp = new Spectre();
    sp->show();
}

void MainWindow::processEyes()
{
	readData();
	const edfFile & fil = globalEdf;

    std::vector<int> eegs;
    std::vector<int> eogs;
    for(int i = 0; i < fil.getNs(); ++i)
    {
        const auto & labl = fil.getLabels()[i];
        if(labl.contains("EOG"))
        {
            eogs.push_back(i);
        }
        /// what with A1-A2 and A1-N ???
        else if(labl.contains("EEG"))
        {
            if(!labl.contains("A1-A2") &&
               !labl.contains("A2-A1") &&
               !labl.contains("A1-N" ) &&
               !labl.contains("A2-N"))
            {
                eegs.push_back(i);
            }
        }
    }
    /// or (eogs, eegs)
	myLib::eyesProcessingStatic(eogs); // for first 19 eeg channels
}

void MainWindow::showNet()
{
    Net * ANN = new Net();
    ANN->show();
}

void MainWindow::showCut()
{
    Cut *cut_e = new Cut();
    cut_e->show();
}

void MainWindow::setEdfFileSlot()
{
    QString helpString;

    if(def::dir->absolutePath() == QDir::root().absolutePath())
    {
        helpString = QFileDialog::getOpenFileName((QWidget*)this,
                                                  tr("EDF to open"),
                                                  def::dataFolder,
                                                  tr("EDF files (*.EDF *.edf)"));
    }
    else
    {
        helpString = QFileDialog::getOpenFileName((QWidget*)this,
                                                  tr("EDF to open"),
                                                  def::dir->absolutePath(),
                                                  tr("EDF files (*.EDF *.edf)"));
    }

    if(helpString.isEmpty())
    {
//        QMessageBox::warning((QWidget*)this, tr("Warning"), tr("no EDF file was chosen"), QMessageBox::Ok);
        return;
    }
    setEdfFile(helpString);
}

void MainWindow::setEdfFile(const QString & filePath)
{
    QString helpString;
    helpString = filePath;


    if(!helpString.endsWith(".edf", Qt::CaseInsensitive))
    {
        helpString += ".edf";
    }
    if(!QFile::exists(helpString))
    {
        helpString = "Cannot open EDF file:\n" + helpString;
		std::cout << helpString << std::endl;
        return;
    }

    ui->filePathLineEdit->setText((helpString));

    //set ExpName
	def::ExpName = myLib::getExpNameLib(filePath);

	helpString.resize(helpString.lastIndexOf(slash));
    def::dir->cd(helpString);

	if(def::redirectStdOutFlag)
    {
		//redirect std::cout to logfile
        if(generalLogStream.is_open())
        {
            generalLogStream.close();
        }
		helpString +=  "/generalLog.txt";

		generalLogStream.open(helpString.toStdString(), std::ios_base::app);
		generalLogStream << std::endl << std::endl << std::endl;

		std::cout.rdbuf(generalLogStream.rdbuf());
    }
    else
    {
		std::cout.rdbuf(stdOutBuf);
    }


	def::dir->mkdir("Reals");
	def::dir->mkdir("Reals/BC");
	def::dir->mkdir("winds");
	def::dir->mkdir("winds/fromreal");
	def::dir->mkdir("SpectraSmooth");
	def::dir->mkdir("SpectraSmooth/winds");

	def::dir->mkdir("Signals");
	def::dir->mkdir("Signals/winds");
	def::dir->mkdir("SpectraImg");

	def::dir->mkdir("Help");
	def::dir->mkdir("Help/maps");
	def::dir->mkdir("Help/psa");
	def::dir->mkdir("Help/ica");
	def::dir->mkdir("Help/wm");
	def::dir->mkdir("Help/PA");
	def::dir->mkdir("Help/wts");
//	def::dir->mkdir("amod");
//	def::dir->mkdir("auxEdfs");

//	def::dir->mkdir("Signals/before");
//	def::dir->mkdir("Signals/after");
//	def::dir->mkdir("Signals/other");
//	def::dir->mkdir("Signals/winds/after");
//	def::dir->mkdir("Signals/winds/before");
//	def::dir->mkdir("Signals/winds/other");
//	def::dir->mkdir("SignalsCut");
//	def::dir->mkdir("SignalsCut/before");
//	def::dir->mkdir("SignalsCut/after");
//	def::dir->mkdir("SpectraSmooth/Bayes");
//	def::dir->mkdir("SpectraSmooth/PCA");
//	def::dir->mkdir("SpectraSmooth/Clean");
//	def::dir->mkdir("cut");

//	def::dir->mkdir("visualisation");
//	def::dir->mkdir("visualisation/video");
//	def::dir->mkdir("visualisation/wavelets");
//	for(int i = 0; i < 19; ++i)
//	{
//		helpString = "visualisation/wavelets/";
//		def::dir->mkdir(helpString.append(QString::number(i)));
//	}


    helpString = "EDF file read successfull";
    ui->textEdit->append(helpString);

    helpString="ns equals to ";
    helpString.append(QString::number(def::ns));
    ui->textEdit->append(helpString);

}


void MainWindow::readData()
{

    QTime myTime;
    myTime.start();

    QString helpString;
    helpString = ( ui->filePathLineEdit->text() );
    if(!QFile::exists(helpString))
    {
		std::cout << "readData: edf file doent exist\n" << helpString.toStdString() << std::endl;
        return;
    }
    globalEdf.readEdfFile(helpString);
    def::ns = globalEdf.getNs();

	helpString = "data have been read\n";
	helpString += "ns equals to " + nm(def::ns);
    ui->textEdit->append(helpString);

    ui->markerSecTimeDoubleSpinBox->setMaximum(globalEdf.getDataLen() / def::freq);
    ui->markerBinTimeSpinBox->setMaximum(globalEdf.getDataLen());
}

void MainWindow::drawDirSlot()
{
    const QString deer = ui->drawDirBox->currentText();
    if(deer.contains("spectr", Qt::CaseInsensitive))
    {
		drawSpectraSlot();
    }
    else
    {
		drawReals();
    }
}


void MainWindow::drawSpectraSlot()
{
	const QString prePath = def::dir->absolutePath() + "/" + ui->drawDirBox->currentText();
	const QString outPath = def::dir->absolutePath() + "/SpectraImg";
    drawSpectra(prePath, outPath);
}


void MainWindow::drawSpectra(const QString & prePath,
                             const QString & outPath)
{
    QTime myTime;
    myTime.start();

    QStringList lst;
	myLib::makeFullFileList(prePath, lst);
    std::valarray<double> dataS;
    int i = 0;
    QString helpString;
    for(const QString & str : lst)
    {
		helpString = prePath + "/" + str;
		myLib::readFileInLine(helpString,
                       dataS);

		helpString = outPath + "/" + str + ".jpg";
		myLib::drawTemplate(helpString);
		myLib::drawArray(helpString, dataS);

        ui->progressBar->setValue(100 * (++i) / lst.length());
        qApp->processEvents();
        if(stopFlag)
        {
            stopFlag = false;
            break;
        }
    }
    ui->progressBar->setValue(0);
	std::cout << "drawSpectra: time = " << myTime.elapsed() / 1000. << " sec" << std::endl;
}

void MainWindow::drawReals()
{
    QString helpString;

    QTime myTime;
    myTime.start();
    ui->progressBar->setValue(0);
    matrix dataD;

	QString prePath = def::dir->absolutePath() + "/" + ui->drawDirBox->currentText();
//    makeFullFileList(prePath, lst);

	auto a = def::edfFilters + QStringList("*." + def::plainDataExtension);
	QStringList lst = QDir(prePath).entryList(a);

    int redCh = -1;
    int blueCh = -1;
    const edfFile & fil = globalEdf;

	/// magic const, switch
    if(def::ns == 41)
    {
        redCh = 21;
        blueCh = 22;
    }
    else if(def::ns == 22 || def::ns == 21)
    {
        redCh = 19;
        blueCh = 20;
    }
    else if(def::ns == 24)
    {
        redCh = 21;
        blueCh = 22;
    }
	else /// not the best solution
    {
        for(int i = 0; i < fil.getNs(); ++i)
        {
            if(fil.getLabels()[i].contains("EOG1")) redCh = i;
            else if(fil.getLabels()[i].contains("EOG2")) blueCh = i;
        }
    }

    for(int i = 0; i < lst.length(); ++i)
    {
        if(stopFlag)
        {
            break;
        }
		helpString = prePath + "/" + lst[i];
		myLib::readPlainData(helpString,
					  dataD);

		if(dataD.cols() > 15000)
        {
            continue;
        }

		helpString = myLib::getPicPath(helpString);


		myLib::drw::drawEeg(dataD * ui->drawCoeffSpinBox->value(),
							def::freq,
		{ std::make_pair(blueCh, "blue"), std::make_pair(redCh, "red") }).save(
					helpString, 0, 100);


        ui->progressBar->setValue(100 * (i + 1) / lst.length());
        qApp->processEvents();
    }
    ui->progressBar->setValue(0);

	helpString = QString("signals are drawn\n") + "ns equals to " + nm(def::ns);
    ui->textEdit->append(helpString);

    stopFlag = 0;
	std::cout << "drawReals: time = " << myTime.elapsed()/1000. << " sec" << std::endl;
}

void MainWindow::cleanDirsAll(bool fl)
{
	ui->cleanRealsCheckBox->setChecked(fl);
	ui->cleanRealsSpectraCheckBox->setChecked(fl);
	ui->cleanWindsCheckBox->setChecked(fl);
	ui->cleanWindsSpectraCheckBox->setChecked(fl);
	ui->cleanRealsSignalsCheckBox->setChecked(fl);
	ui->cleanWindsSignalsCheckBox->setChecked(fl);
	ui->cleanFromRealsCheckBox->setChecked(fl);
	ui->cleanMarkersCheckBox->setChecked(fl);
	ui->cleanSpectraImgCheckBox->setChecked(fl);
}

void MainWindow::cleanDirs()
{
    QString helpString;

    if(def::dir->isRoot())
    {
        QMessageBox::critical((QWidget*)this, tr("Warning"), tr("No dir"), QMessageBox::Ok);
        return;
    }

	for(int i = 0; i < ui->cleanDirsLayout->count(); ++i)
	{
		QCheckBox * item = dynamic_cast<QCheckBox*>(ui->cleanDirsLayout->itemAt(i)->widget());
		if(item)
		{
			if(item->text() == "markers")
			{
				helpString = def::dir->absolutePath();
				myLib::cleanDir(helpString, "markers", 0);
			}
			else
			{
				helpString = def::dir->absolutePath()+ "/" + item->text();
				myLib::cleanDir(helpString);
			}
		}
	}
	helpString = "dirs cleaned\n";
	helpString += "ns equals to " + nm(def::ns);
    ui->textEdit->append(helpString);
}



void MainWindow::markerSetSecTime(int a)
{
    ui->markerSecTimeDoubleSpinBox->setValue(double(a) / def::freq);
}

void MainWindow::markerGetSlot()
{
    bool byteMarker[16];
    int timeIndex = ui->markerBinTimeSpinBox->value();
    int marker = 0;
    marker = globalEdf.getData()[globalEdf.getMarkChan()][timeIndex];
    QString helpString;

    for(int h = 0; h < 16; ++h)
    {
        byteMarker[16-1 - h] = (marker%(int(pow(2, h+1)))) / (int(pow(2,h)));
    }

    helpString.clear();
    for(int h = 0; h < 8; ++h)
    {
        helpString += QString::number(byteMarker[h]);
    }
    ui->markerBin0LineEdit->setText(helpString);

    helpString.clear();
    for(int h = 0; h < 8; ++h)
    {
        helpString += QString::number(byteMarker[h + 8]);
    }
    ui->markerBin1LineEdit->setText(helpString);

    ui->markerDecimalLineEdit->setText(QString::number(marker));
}

void MainWindow::markerSetSlot()
{
    int timeIndex = ui->markerBinTimeSpinBox->value();
    int marker = ui->markerDecimalLineEdit->text().toInt();
    globalEdf.setData(globalEdf.getMarkChan(), timeIndex, marker);
}

void MainWindow::markerSetDecValueSlot()
{
    int marker = 0;
    QString helpString;

    helpString = ui->markerBin0LineEdit->text();
    for(int h = 0; h < 8; ++h)
    {
        if(helpString[h] != '0' && helpString[h] != '1') return;
        marker += pow(2, 8) * (helpString[h] == '1') * pow(2, 7-h);
    }

    helpString = ui->markerBin1LineEdit->text();
    for(int h = 0; h < 8; ++h)
    {
        if(helpString[h] != '0' && helpString[h] != '1') return;
        marker += (helpString[h] == '1') * pow(2, 7-h);
    }
    ui->markerDecimalLineEdit->setText(QString::number(marker));
}

void MainWindow::markerSetBinValueSlot()
{
    int marker = ui->markerDecimalLineEdit->text().toInt();
    QString helpString;

	std::vector<bool> byteMarker = myLib::matiCountByte(double(marker));

    helpString.clear();
    for(int h = 15; h >= 8; --h)
    {
		helpString += (byteMarker[h] ? "1" : "0");
    }
    ui->markerBin0LineEdit->setText(helpString);

    helpString.clear();
    for(int h = 7; h >= 0; --h)
    {
		helpString += (byteMarker[h] ? "1" : "0");
    }
    ui->markerBin1LineEdit->setText(helpString);
}

void MainWindow::markerSaveEdf()
{
    QString helpString;
	helpString = def::dir->absolutePath() + "/" + def::ExpName + ui->newEdfNameLineEdit->text();
    if(!helpString.endsWith(".edf", Qt::CaseInsensitive))
    {
        helpString += ".edf";
    }
	helpString = myLib::setFileName(helpString);
    globalEdf.writeEdfFile(helpString);
}

void MainWindow::drawMapsSlot()
{
    QString helpString = QFileDialog::getOpenFileName(this,
                                                      tr("Choose maps file"),
                                                      def::dir->absolutePath(),
                                                      tr("*.txt"));
    if(helpString.isEmpty())
    {
        helpString = def::dir->absolutePath()
					 + "/Help"
					 + "/" + def::ExpName
                     + "_maps.txt";
        helpString.remove("_ica");
    }
    if(!QFile::exists(helpString))
    {
        return;
    }
	myLib::drawMapsICA(helpString,
					   def::dir->absolutePath()
					   + "/Help"
					   + "/Maps",
					   def::ExpName);
}


void MainWindow::stop()
{
    stopFlag = 1;
}

void MainWindow::setFileMarkers()
{
    def::fileMarkers = ui->fileMarkersLineEdit->text().split(QRegExp(R"([,;])"),
                                                             QString::SkipEmptyParts);
    ui->textEdit->append(R"(fileMarkers renewed)");
}
