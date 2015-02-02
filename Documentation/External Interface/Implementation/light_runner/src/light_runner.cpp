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

#include <QApplication>
#include <QTimer>

#include "light_runner.h"
#include "ui_light_runner.h"

#include "evc_com.h"
#include "etcs_config.h"

LightRunner::LightRunner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LightRunner),
    m_pEvc_com( NULL ),
    m_TIUTimerId(0),
    m_ODOTimerId(0),
    m_Position(0.0)
{
    ui->setupUi(this);
}

LightRunner::~LightRunner()
{
    if( 0 != m_TIUTimerId )
    {
        killTimer( m_TIUTimerId );
    }

    if( 0 != m_ODOTimerId )
    {
        killTimer( m_ODOTimerId );
    }

    m_pEvc_com->SIM_Stop();

    delete ui;
    delete m_pEvc_com;
}

void LightRunner::on_m_StartEVCButton_clicked()
{
    if( NULL == m_pEvc_com )
    {
        m_pEvc_com = new CEvc_com();
        m_pEvc_com->SIM_Modify_EVC_Configuration( false, CFG_USE_JRU );
        m_pEvc_com->SIM_Init( 0 );
        m_pEvc_com->SIM_Start_processes();
        m_pEvc_com->SIM_Run();

        QTimer::singleShot( 1000, this, SLOT(mainswitchon()) );
    }
}

void LightRunner::mainswitchon()
{
    m_pEvc_com->ODO_Send_TIUDriver_request( TIU_RQST_MAINSWITCH_ON );
    m_pEvc_com->ODO_Send_TIUDriver_request( TIU_RQST_CABIN_A );

    // Start timers
    if( 0 != m_TIUTimerId )
    {
        killTimer( m_TIUTimerId );
    }

    m_TIUTimerId = startTimer( 100 );

    if( 0 != m_ODOTimerId )
    {
        killTimer( m_ODOTimerId );
    }

    m_ODOTimerId = startTimer( 50 );
}

void LightRunner::timerEvent( QTimerEvent* event )
{
    if( event->timerId() == m_TIUTimerId )
    {
        if( NULL != m_pEvc_com )
        {
            m_pEvc_com->ODO_RefreshTIUData();
            struct STxTiuData tiu = m_pEvc_com->ODO_getEvcTiu();

            if( tiu.bEB_App )
            {
                qDebug() << tiu.bEB_App;
            }

            ui->m_EBButton->setChecked( tiu.bEB_App );
            ui->m_SBButton->setChecked( tiu.bSB_App );
        }
    }
    else if( event->timerId() == m_ODOTimerId )
    {
        if( NULL != m_pEvc_com )
        {
            m_pEvc_com->ODO_Send_Odo_data( m_Position, 5, 0 );
            m_Position = m_Position + 0.001;
        }
    }
    else
    {
        QMainWindow::timerEvent( event );
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LightRunner w;
    w.show();

    return a.exec();
}
