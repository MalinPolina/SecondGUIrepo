#include "net.h"
#include "ui_net.h"


void Net::autoClassification(const QString & spectraDir)
{
    QTime myTime;
    myTime.start();

#if 0
    //set random matrix - add in PaIntoMatrixByName
    for(int i = 0; i < def::nsWOM(); ++i)
    {
        for(int j = 0; j < def::nsWOM(); ++j)
        {
            tempRandomMatrix[i][j] = (i == j); //identity matrix
//            tempRandomMatrix[i][j] = (i == j) * (5 + rand()%36) / 10.; //random diagonal
//            tempRandomMatrix[i][j] = (5. + rand()%36) / 50.; //full random
//            tempRandomMatrix[i][j] = (i == j) * (1. + (rand()%8 == 0) * (5. + rand()%35) / 5.); //random diagonal
        }
    }
#endif

    loadData(spectraDir);

    switch(Mode)
    {
    case myMode::k_fold:
    {
        crossClassification(); break;
    }
    case myMode::N_fold:
    {
        leaveOneOutClassification(); break;
    }
    case myMode::train_test:
    {
        trainTestClassification(); break;
    }
    case myMode::half_half:
    {
        halfHalfClassification(); break;
    }
    default: {break;}
    }
	myClassifier->averageClassification();
    std::cout <<  "AutoClass: time elapsed = " << myTime.elapsed()/1000. << " sec" << std::endl;
}


avType Net::autoClassification()
{
    QTime myTime;
    myTime.start();

	if(myClassifier->getType() == ClassifierType::ANN)
	{
		/// adjust learnRate
		ANN * myANN = dynamic_cast<ANN *>(myClassifier);
		myANN->adjustLearnRate();
	}

    switch(Mode)
    {
    case myMode::k_fold:
    {
        crossClassification();
//		std::cout << "cross classification:" << std::endl;
        break;
    }
    case myMode::N_fold:
	{
		leaveOneOutClassification();

		myClassifier->learnAll();
		myClassifier->printParams();
//		std::cout << "N-fold cross-validation:" << std::endl;
        break;
    }
    case myMode::train_test:
    {
        trainTestClassification();
//		std::cout << "train-test classification:" << std::endl;
        break;
    }
    case myMode::half_half:
    {
        halfHalfClassification();
//		std::cout << "half-half classification:" << std::endl;
        break;
    }
    default: {break;}
    }
    std::cout <<  "autoClassification: time elapsed = " << myTime.elapsed()/1000. << " sec" << std::endl;
    return myClassifier->averageClassification();
}


void Net::autoClassificationSimple()
{
    QString helpString = def::dir->absolutePath()
                         + slash + "SpectraSmooth";

    switch(Source)
    {
    case source::winds:
    {
        helpString += slash + "winds"; break;
    }
    case source::bayes:
    {
        helpString += slash + "Bayes"; break;
    }
    case source::pca:
    {
        helpString += slash + "PCA"; break;
    }
    default: {break;}
    }

    if(!helpString.isEmpty())
    {
        autoClassification(helpString);
    }
}


void Net::leaveOneOutClassification()
{
	const auto & dataMatrix = myClassifier->getClassifierData()->getData();

	std::vector<uint> learnIndices;
    for(uint i = 0; i < dataMatrix.rows(); ++i)
    {
//		std::cout << i + 1 << " "; std::cout.flush();

        learnIndices.clear();
        learnIndices.resize(dataMatrix.rows() - 1);
        std::iota(std::begin(learnIndices),
                  std::begin(learnIndices) + i,
                  0);
        std::iota(std::begin(learnIndices) + i,
                  std::end(learnIndices),
                  i + 1);

		myClassifier->learn(learnIndices);
		myClassifier->test({i});
    }
//    std::cout << std::endl;
}

void Net::crossClassification()
{
	const matrix & dataMatrix = myClassifier->getClassifierData()->getData();
	const std::vector<uint> & types = myClassifier->getClassifierData()->getTypes();

    const int numOfPairs = ui->numOfPairsBox->value();
    const int fold = ui->foldSpinBox->value();

    std::vector<std::vector<uint>> arr; // [class][index]
    arr.resize(def::numOfClasses());
    for(uint i = 0; i < dataMatrix.rows(); ++i)
    {
        arr[ types[i] ].push_back(i);
    }
    std::cout << "Net: crossClass (max " << numOfPairs << "):" << std::endl;

    for(int i = 0; i < numOfPairs; ++i)
    {
        std::cout << i + 1 << " "; std::cout.flush();

        // mix arr for one "pair"-iteration
        for(int i = 0; i < def::numOfClasses(); ++i)
        {
			smLib::mix(arr[i]);
        }

        for(int numFold = 0; numFold < fold; ++numFold)
        {
            auto sets = makeIndicesSetsCross(arr, numFold); // on const arr
            myClassifier->learn(sets.first);
            myClassifier->test(sets.second);
        }

        qApp->processEvents();
        if(stopFlag)
        {
            stopFlag = 0;
            return;
        }
    }
    std::cout << std::endl;
}

std::pair<std::vector<uint>,
std::vector<uint>> Net::makeIndicesSetsCross(
                       const std::vector<std::vector<uint> > & arr,
                       const int numOfFold)
{
	const std::valarray<double> & classCount = myClassifier->getClassifierData()->getClassCount();
	const double & numOfClasses = myClassifier->getClassifierData()->getNumOfCl();

    std::vector<uint> learnInd;
    std::vector<uint> tallInd;

    const int fold = ui->foldSpinBox->value();

	for(int i = 0; i < numOfClasses; ++i)
    {
        for(int j = 0; j < classCount[i]; ++j)
        {
            if(j >= (classCount[i] * numOfFold / fold) &&
               j < (classCount[i] * (numOfFold + 1) / fold))
            {
                tallInd.push_back(arr[i][j]);
            }
            else
            {
                learnInd.push_back(arr[i][j]);
            }
        }
    }
    return make_pair(learnInd, tallInd);
}

void Net::halfHalfClassification()
{
	const auto & dataMatrix = myClassifier->getClassifierData()->getData();
    std::vector<uint> learnIndices;
    std::vector<uint> tallIndices;

    for(uint i = 0; i < dataMatrix.rows() / 2; ++i)
    {
        learnIndices.push_back(i);
        tallIndices.push_back(i + dataMatrix.rows() / 2);
    }
    if(learnIndices.empty() || tallIndices.empty())
    {
        std::cout << "Net::halfHalfClassification: indicesArray empty, return" << std::endl;
        return;
    }
    myClassifier->learn(learnIndices);
    myClassifier->test(tallIndices);
}

void Net::trainTestClassification(const QString & trainTemplate,
                                  const QString & testTemplate)
{
	const auto & dataMatrix = myClassifier->getClassifierData()->getData();
	const std::vector<QString> & fileNames = myClassifier->getClassifierData()->getFileNames();

    std::vector<uint> learnIndices;
    std::vector<uint> tallIndices;
    for(uint i = 0; i < dataMatrix.rows(); ++i)
    {
        if(fileNames[i].contains(trainTemplate))
        {
            learnIndices.push_back(i);
        }
        else if(fileNames[i].contains(testTemplate))
        {
            tallIndices.push_back(i);
        }
    }
    if(learnIndices.empty() || tallIndices.empty())
    {
        std::cout << "Net::trainTestClassification: indicesArray empty, return" << std::endl;
        return;
    }
    myClassifier->learn(learnIndices);
    myClassifier->test(tallIndices);
}


void Net::customF()
{

#if 0
	/// get wts pics from classifier

	ANN * myANN = dynamic_cast<ANN *>(myClassifier);
	myANN->learnAll();
	myANN->writeWeight();
	myANN->drawWeight();
	return;
#endif

#if 01
	/// clean CLassifierData to 3 * N train windows
	uint N = 80;
	myClassifierData.clean(N, "_train");
	return;
#endif

#if 0
    /// search optimal params
    QString outFileName;
//    for(QString cls : {"DIST", "WARD", "KNN", "NBC"}) /// recheck
    QString cls = "RDA";
    {
        setClassifier(cls);
        outFileName = "PCA_" + cls.toLower() + "SucRes_manual.txt";

        std::vector<std::vector<double>> results{};
        cycleParams(results);

        const int accNum = results[0].size() - 2;

        std::sort(std::begin(results), std::end(results),
                  [accNum](const std::vector<double> & in1, const std::vector<double> & in2)
        {
			/// comment to sort
//            return true;
            if(in1[accNum] == in2[accNum]) return in1[accNum + 1] > in2[accNum + 1]; /// by kappa
            return in1[accNum] > in2[accNum];
        });

        ofstream outStr;
        outStr.open((def::dir->absolutePath()
                     + slash + outFileName).toStdString(), std::ios_base::app);
        outStr << def::ExpName << std::endl;

        int num = 0;
        for(auto vec : results)
        {
            /// comment for all results
//            if(vec[accNum] < results[0][accNum] - 2.5 || num == 20) break;
            outStr << vec << std::endl;
            ++num;
        }
        outStr.close();
    }
#endif


}

#define SUCCESSIVE_CHECK {\
    loadData(def::dir->absolutePath() + "/SpectraSmooth/PCA", {def::ExpName});\
    resizeData(i);\
    myClassifier->learnAll();\
    loadData(def::dir->absolutePath() + "/SpectraSmooth", {def::ExpName.left(3) + "_test"});\
    applyPCA(def::dir->absolutePath() + "/Help/ica/" + def::ExpName + "_pcaMat.txt");\
    resizeData(i);\
    myClassifier->testAll();\
    a = myClassifier->averageClassification();\
    }


void Net::cycleParams(std::vector<std::vector<double> > & in)
{
	std::vector<int> rdaPar{1, 10, 1, 10};

	int i = 0;
//    for(int i = 60; i >= 24; i -= 2)
	{
        //            dataMatrix.resizeCols(i);
        switch(myClassifier->getType())
        {
        case ClassifierType::RDA:
        {
            for(double lambda = 0.1 * rdaPar[0]; lambda <= 0.1 * rdaPar[1]; lambda += 0.1)
            {
                for(double shr = 0.1 * rdaPar[2]; shr <= 0.1 * rdaPar[3]; shr += 0.1)
                {
                    setRdaLambdaSlot(lambda);
                    setRdaShrinkSlot(shr);


#if 0
                    avType a; SUCCESSIVE_CHECK
        #else
                    /// one file
                    auto a = autoClassification();
#endif
//                    in.push_back(std::vector<double>{i, lambda, shr, a.first, a.second});
					in.push_back(std::vector<double>{lambda, shr, a.first, a.second});
                }
            }
            break;
        }
        case ClassifierType::ANN:
        {
            double lrat = 0.05;
            {
                setLrate(lrat);
				avType a;
//				SUCCESSIVE_CHECK
                        //            auto a = autoClassification();
						in.push_back(std::vector<double>{double(i), lrat, a.first, a.second});
            }
            break;
        }
        case ClassifierType::SVM:
        {
            for(int svmTyp = 0; svmTyp <= 1; ++svmTyp)
            {
                //            for(int kerTyp = 0; kerTyp <= 4; ++kerTyp)
                int kerTyp = 0;
                {
                    setSvmTypeSlot(svmTyp);
                    setSvmKernelNumSlot(kerTyp);

                    auto a = autoClassification();
					in.push_back(std::vector<double>{double(i), double(svmTyp), double(kerTyp), a.first, a.second});
                }
            }
            break;
        }
        case ClassifierType::KNN:
        {
            for(int numNear = 1; numNear <= 10; ++numNear)
            {
                setKnnNumSlot(numNear);

                auto a = autoClassification();
				in.push_back(std::vector<double>{double(i), double(numNear), a.first, a.second});
            }
            break;
        }
        case ClassifierType::WARD:
        {
			for(int numOfClust = 6; numOfClust <= 20; ++numOfClust)
            {
				setWordNumSlot(numOfClust);

                auto a = autoClassification();
				in.push_back(std::vector<double>{double(i), double(numOfClust), a.first, a.second});
            }
            break;
        }
        default:
        {
            auto a = autoClassification();
			in.push_back(std::vector<double>{double(i), a.first, a.second});
            break;
        }
        }
    }

}
