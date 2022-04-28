#ifndef MODALCONFIG_H
#define MODALCONFIG_H

#include <QDialog>

namespace Ui {
class modalConfig;
}

typedef struct{
    QString appDir, workDir;
    unsigned long long MinimumWorkingSetSize = 256,
                       MaximumWorkingSetSize = 2000,
                       ProcessMemoryLimit;
    unsigned long long CheckMiliSec = 1000, PauseMiliSec = 0;
}datos;


class modalConfig : public QDialog
{
    Q_OBJECT

public:
    explicit modalConfig(QWidget *parent = 0);
    datos getValues();
    void setValues(datos app);
    ~modalConfig();

private:
    Ui::modalConfig *ui;
};

#endif // MODALCONFIG_H
