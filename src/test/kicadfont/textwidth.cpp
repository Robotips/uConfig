/* This 'program' create an array of width of kicad chars */

#include <QDebug>

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QStringList>

void widthCompute()
{
    //
    /*QString namePin, namePin2;
    namePin.append('|');
    for (char c=32; c<=125; c++)
    {
        namePin.append(c);
        namePin.append('|');
        namePin2.append(c);
    }
    component->addPin(new Pin(namePin, "A"));
    component->addPin(new Pin(namePin2, "B"));*/

    QImage imgText("textPin.png");
    QImage imgOne("textPipe.png");
    int y = 67;

    QPixmap pix(imgText.width(), imgOne.height());
    QPainter painter(&pix);
    painter.setBrush(QColor(255, 0, 0, 64));
    painter.drawImage(QPoint(0, 0), imgText, QRect(0, y, imgText.width(), imgOne.height()));
    qDebug() << imgText.width();

    QStringList widths;
    int oldX = 0;
    char c = 32;
    for (int x = 0; x < imgText.width() - imgOne.width(); x++)
    {
        int conv = 0;
        for (int x0 = 0; x0 < imgOne.width(); x0++)
        {
            for (int y0 = 0; y0 < imgOne.height(); y0++)
            {
                conv += qAbs(qGray(imgText.pixel(x0 + x, y0 + y)) - qGray(imgOne.pixel(x0, y0)));
            }
        }
        if (conv < 5000 && x - oldX - imgOne.width() > 5)
        {
            widths.append(QString::number(x - oldX - imgOne.width()));
            qDebug() << c++ << x << conv << x - oldX - imgOne.width();
            painter.drawRect(x, 0, imgOne.width() - 1, imgOne.height() - 1);
            oldX = x;
        }
    }
    pix.save("test.png");

    qDebug() << widths.join(", ");
}
