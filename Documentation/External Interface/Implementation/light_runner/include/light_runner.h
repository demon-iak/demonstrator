#ifndef LIGHT_RUNNER_H
#define LIGHT_RUNNER_H

#include <QMainWindow>
#include <QDebug>

class CEvc_com;

namespace Ui {
class LightRunner;
}

class LightRunner : public QMainWindow
{
    Q_OBJECT

public:
    explicit LightRunner(QWidget *parent = 0);
    ~LightRunner();

protected:

    virtual void timerEvent( QTimerEvent* event );

private:
    Ui::LightRunner *ui;
    CEvc_com* m_pEvc_com;
    int m_TIUTimerId;
    int m_ODOTimerId;
    double m_Position;

private slots:
    void on_m_StartEVCButton_clicked();
    void mainswitchon();
};

#endif // LIGHT_RUNNER_H
