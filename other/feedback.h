#ifndef EDFFILE_FB_H
#define EDFFILE_FB_H
#include <other/edffile.h>

namespace fb
{

class FBedf : public edfFile
{
public:
	enum class taskType : int {spat = 0, verb = 1, rest = 2};
private:

	/// [type][numReal] = realMatrix
	std::vector<std::vector<matrix>> realsSignals;

	/// [type][numReal] = length
	std::vector<std::valarray<double>> solvTime;

	/// [type][numReal]
	std::vector<std::vector<int>> ans;

	/// [numReal]
	std::vector<int> ansRow;

	/// [type][numOfReal] = spectre
	std::vector<std::vector<matrix>> realsSpectra;


	/// from 0 - for dispersion, distance, etc
	const std::vector<int> chansToProcess{
//		0, 1,		// Fp1,	Fp2
//		2, 6,		// F7,	F8
		3, 4, 5,	// F3,	Fz,	F4
//		7, 11,		// T3, T4
		8, 9, 10,	// C3,	Cz,	C4
//		12, 16,		// T5,	T6
		13, 14, 15,	// P3,	Pz,	P4
//		17, 18		// O1,	O2
	};

	static const int numTasks = 40;
	static constexpr double spStep = 250. / 4096.;
	static constexpr double leftFreq = 5.;
	static constexpr double rightFreq = 20.;
	static constexpr double solveThres = 40.;			/// 40 sec for a task

public:
	/// solvTime? ans spectre inside
	FBedf(const QString & edfPath, const QString & ansPath);

	double spectreDispersion(taskType typ);
	double distSpec(taskType type1, taskType type2);
	double insightPartOfAll(double thres);
	double insightPartOfSolved(double thres);
	QPixmap kdeForSolvTime(taskType typ);
	QPixmap verbShortLong(double thres);		/// spectra of short and long anagramms

	int getAns(int i) { return ansRow[i]; }

private:
	std::vector<double> freqs;
	std::vector<int> readAns(const QString & ansPath);
	std::valarray<double> spectralRow(taskType type, int chan, double freq);
};




/// feedback
class FeedbackClass
{
public:
	FeedbackClass() {}
	FeedbackClass(const QString & guyPath_,
				  const QString & guyName_,
				  const QString & postfix_)
		: guyPath(guyPath_), guyName(guyName_), postfix(postfix_) {}
	~FeedbackClass() {}

	enum class taskType : int {spat = 0, verb = 1};
	enum class fileNum  : int {first = 0, third = 1};
	enum class ansType  : int {skip = 0, right = 1, wrong = 2};

	void checkStat();
	void writeFile();

	void checkStatTimes(taskType in, ansType howSolved);
	void checkStatSolving(taskType typ, ansType howSolved);
	void countTimes();

	static const int numTasks = 40;

private:
	QString ansPath(int numSes);
	std::vector<int> readAnsFile(int numSes);
	std::valarray<double> timesToArray(taskType in, fileNum filNum, ansType howSolved);

private:
	QString guyPath;
	QString guyName;
	QString postfix;

	std::vector<std::pair<int, double>> times[2][2]; /// [numFile][taskType][taskNum][ansType, time]

};


void createAnsFiles(const QString & guyPath, QString guyName);
void checkMarkFBfinal(const QString & filePath);
void timesSolving(const QString & guyPath,
				  const QString & guyName,
				  const QString & postfix = QString());
QString timesPath(const QString & guyPath,
				  const QString & guyName,
				  int numSes,
				  int typ);
std::vector<double> timesFromFile(const QString & timesPath, int howSolved);
void feedbackFinalTimes(const QString & guyPath,
						const QString & guyName,
						const QString & postfix = QString());
void checkStatResults(const QString & guyPath, const QString & guyName);

void successiveNetPrecleanWinds(const QString & windsPath);


} // end namespace fb

#endif // EDFFILE_FB_H
