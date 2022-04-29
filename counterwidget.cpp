#include <QHBoxLayout>
#include "counterwidget.h"
#include "config.h"

CounterWidget::CounterWidget(QWidget *parent)
    : QWidget(parent), m_count(0), m_timer(this), m_gpio(this)
{
    setWindowTitle("BinaryCounter");
    resize(300, 200);
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    // QLCDNumber zur Anzeige des Zaehlerstandes dezimal:

    m_LCD = new QLCDNumber(3);
    layout->addWidget(m_LCD);

    connect(&m_timer, &QTimer::timeout, this, &CounterWidget::updateCounter);
    m_timer.start(T_UPDATE);
}

CounterWidget::~CounterWidget()
{

}

// Zaehlerstand im Widget und dual ueber LEDs anzeigen:
void CounterWidget::updateCounter()
{
    int n = 0;
    for (auto pin : BUTTONS)
    {
        if(m_gpio.isActivated(pin))
        {
            switch (n)
            {
                case 0:
                    m_count--;
                    if(m_count < 0)
                        m_count = 15;
                    break;
                case 1:
                    m_count = 0;
                    break;
                case 2:
                    m_count++;
                    if(m_count > 15)
                        m_count = 0;
                    break;
                default:
                    break;
            }
        }
        m_LCD->display(m_count);
        m_gpio.set(m_count);
        n++;
    }
}


