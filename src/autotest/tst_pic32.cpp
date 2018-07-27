#include <QtTest>
#include <QCoreApplication>

#include "datasheet.h"

#include "model/lib.h"

class PIC32 : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void test_casePIC32MM_GPM();

protected:
    Datasheet dt;
};

void PIC32::init()
{
}

void PIC32::cleanup()
{
    dt.clean();
    dt.close();
}

void PIC32::test_casePIC32MM_GPM()
{
    //dt.setDebugEnabled(true);
    QVERIFY(dt.open("../../src/autotest/PIC32MM_GPM_pins.pdf"));
    dt.analyse();

    Lib lib;
    QVERIFY(lib.readFrom("../../src/autotest/PIC32MM_GPM_pins.lib"));

    QVERIFY(dt.components().count() == lib.componentsCount());
    for (int i=0; i<dt.components().count(); i++)
    {
        Component *comp1 = dt.components()[i];
        Component *comp2 = lib.components()[i];
        QVERIFY(comp1->pins().count() == comp2->pins().count());

        // check all pins name and pad
        comp1->sort();
        comp2->sort();
        for (int p=0; p<comp1->pins().count(); p++)
        {
            Pin *pin1 = comp1->pins()[p];
            Pin *pin2 = comp2->pins()[p];
            QVERIFY(pin1->name() == pin2->name());
            QVERIFY(pin1->padName() == pin2->padName());
        }
    }
}

QTEST_MAIN(PIC32)

#include "tst_pic32.moc"
