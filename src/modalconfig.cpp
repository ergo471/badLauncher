#include "../include/modalconfig.h"
#include "ui_modalconfig.h"

modalConfig::modalConfig(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::modalConfig)
{
	ui->setupUi(this);

}

datos modalConfig::getValues()
{
	datos app;
	app.MinimumWorkingSetSize = ui->min_lineEdit->text().toLongLong();
	app.MaximumWorkingSetSize = ui->max_lineEdit->text().toLongLong();
	app.ProcessMemoryLimit = ui->pro_lineEdit->text().toLongLong();
	app.CheckMiliSec = ui->checkM_lineEdit->text().toLongLong();
	app.PauseMiliSec = ui->pauseM_lineEdit->text().toLongLong();

	return app;
}

void modalConfig::setValues(datos app)
{
	ui->min_lineEdit->setText(QString::number(app.MinimumWorkingSetSize));
	ui->max_lineEdit->setText(QString::number(app.MaximumWorkingSetSize));
	ui->pro_lineEdit->setText(QString::number(app.ProcessMemoryLimit));
	ui->checkM_lineEdit->setText(QString::number(app.CheckMiliSec));
	ui->pauseM_lineEdit->setText(QString::number(app.PauseMiliSec));
}

modalConfig::~modalConfig()
{
	delete ui;
}
