#include <classifier/classifier.h>

using namespace myOut;

SVM::SVM() : Classifier()
{
    kernelNum = 0;
    myType = ModelType::SVM;
    typeString = "SVM";
}

void SVM::learn(std::vector<uint> & indices)
{
    makeFile(indices, learnFileName);
//    std::cout << std::endl << "LEARN start" << std::endl << std::endl;
    QString helpString = "cd "
                         + workDir
                         + " && svm-train "
                         + " -s " + nm(svmType)
//                         + " -v " + fold
                         + " -t " + nm(kernelNum) + " -q "
                         + learnFileName;
	system(helpString.toStdString().c_str());
//    std::cout << std::endl << "LEARN finish" << std::endl << std::endl;
}

void SVM::classifyDatum1(uint vecNum)
{
    /// to write!
    const QString fileName = "oneVec";
    makeFile({vecNum}, fileName);
//    std::cout << std::endl << "Test start" << std::endl << std::endl;
    QString helpString = "cd "
                         + workDir
                         + " && svm-predict "
                         + fileName + " "
                         + learnFileName + ".model "
                         + outputFileName + " >> /dev/null";
    system(helpString.toStdString().c_str());
//    std::cout << std::endl << "Test finish" << std::endl << std::endl;

    uint outClass;
    std::ifstream inStr;
	inStr.open((workDir + "/" + outputFileName).toStdString());
    inStr >> outClass;
    inStr.close();

	outputLayer = clLib::oneHot(myClassData->getNumOfCl(), outClass);
}


void SVM::setKernelNum(int inNum) /// 0-4
{
    kernelNum = inNum;
}

void SVM::setSvmType(int in) /// 0-4
{
    svmType = in;
}

void SVM::makeFile(const std::vector<uint> & indices,
                   const QString & fileName)
{
	QString helpString = workDir + "/" + fileName;
    /// create/clear
    std::ofstream outStream(helpString.toStdString());

    for(int ind : indices)
    {
		outStream << myClassData->getTypes()[ind] << ' ';
		for(uint l = 0; l < myClassData->getData().cols(); ++l)
        {
            outStream << l + 1 << ':'
					  << smLib::doubleRound(myClassData->getData()[ind][l], 4) << ' ';
        }
		outStream << std::endl;
    }
    outStream.close();
}

