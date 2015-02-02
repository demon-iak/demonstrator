/*****************************************************************
Copyright © 2014 - European Rail Software Applications (ERSA)
                   5 rue Maurice Blin
                   67500 HAGUENAU
                   FRANCE
                   http://www.ersa-france.com

Author(s): Alexis JULIN (ERSA), Didier WECKMANN (ERSA)

Licensed under the EUPL Version 1.1.

You may not use this work except in compliance with the License.
You may obtain a copy of the License at:
http://ec.europa.eu/idabc/eupl.html

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
either express or implied. See the License for the specific
language governing permissions and limitations under the License.
*****************************************************************/

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
