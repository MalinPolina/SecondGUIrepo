#include "cfg.h"
#include "ui_cfg.h"

//awwww


cfg::cfg(QDir *dir_, int ns_, int spLength_, double error_, double lrate_, QString FileName_) :
    ui(new Ui::cfg)
{
    ui->setupUi(this);
    dir=dir_;

    this->setWindowTitle("Make CFG");
    ui->nsBox->setMaximum(50);
    ui->nsBox->setMinimum(1);
    ui->nsBox->setValue(ns_);

    ui->spLBox->setMaximum(1000);
    ui->spLBox->setMinimum(1);
    ui->spLBox->setValue(spLength_);

    ui->numOfOutsBox->setValue(3);
    ui->epsilonSpinBox->setValue(lrate_);
    ui->errorSpinBox->setValue(0.1);
    ui->tempSpinBox->setValue(10);
    ui->nameEdit->setText(FileName_);
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(makeCfg()));
    this->setAttribute(Qt::WA_DeleteOnClose);

}

cfg::~cfg()
{
    delete ui;
}

void cfg::makeCfg()
{
//    inputs    882
//    outputs     2
//    lrate       0.1
//    ecrit       0.5
//    temp       10
//    srand      77
    int helpInt=ui->spLBox->value() * ui->nsBox->value();
//    cout<<"SpL = "<<ui->spLBox->value()<<" ns = "<<ui->nsBox->value()<<endl;

    helpString=QDir::toNativeSeparators(dir->absolutePath()).append(QDir::separator()).append(ui->nameEdit->text()).append(".net");
    FILE * cfgFile = fopen(helpString.toStdString().c_str(), "w");
    if(cfgFile==NULL) {return;}
    /////////////generality//////////////



    fprintf(cfgFile, "inputs    %d\n", helpInt);
    fprintf(cfgFile, "outputs    %d\n", ui->numOfOutsBox->value());
    fprintf(cfgFile, "lrate    %.2lf\n", ui->epsilonSpinBox->value());
    fprintf(cfgFile, "ecrit    %.2lf\n", ui->errorSpinBox->value());
    fprintf(cfgFile, "temp    %d\n", ui->tempSpinBox->value());
    fprintf(cfgFile, "srand    %d\n", int(time (NULL))%1234);
    fclose(cfgFile);
}
