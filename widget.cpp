#include "widget.h"

#include <QDebug>
#include <QFont>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QQuickWidget>
#include <QTimer>
#include <QVBoxLayout>

using namespace std::chrono_literals;


// Fires a timer every millisecond
class PeriodicTimer : public QObject
{
    Q_OBJECT
public:
    explicit PeriodicTimer(QObject *parent = nullptr)
        : QObject(parent)
    {
        QTimer *timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
        timer->setInterval(1ms);
        connect(timer, &QTimer::timeout, this, &PeriodicTimer::tick);
        timer->start();

        m_timer.start();
    }

    double average() const
    {
        return m_average;
    }

signals:
    void averageChanged(double);

private:
    QElapsedTimer m_timer;

    double m_average = 0;

    void tick()
    {
        // Simple EMA
        constexpr double alpha = 0.9;
        const double sample = m_timer.restart();

        double newAverage = alpha * sample + (1.0 - alpha) * m_average;

        if (newAverage != m_average) {
            m_average = newAverage;
            emit averageChanged(m_average);
        }
    }
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    PeriodicTimer *timing = new PeriodicTimer(this);

    QLabel *averageLabel = new QLabel(this);
    QFont labelFont = averageLabel->font();
    labelFont.setPointSize(36);
    averageLabel->setFont(labelFont);
    connect(timing, &PeriodicTimer::averageChanged, this, [averageLabel](double average) {
        averageLabel->setText(QString("Average between wakeups: %1 ms").arg(average, 0, 'f', 2));
    });

    QPushButton *button = new QPushButton("Add QQuickWidget", this);

    // Animating infinite progress bar
    QProgressBar *progress = new QProgressBar(this);
    progress->setMinimum(0);
    progress->setMaximum(0);
    progress->setValue(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(averageLabel);
    layout->addWidget(button);
    layout->addWidget(progress);
    setLayout(layout);

    connect(button, &QPushButton::clicked, this, [=](){
        QQuickWidget *quick = new QQuickWidget(this);
        layout->addWidget(quick);
    });
}

#include "widget.moc"
