#include <QCoreApplication>
#include <QtTest>

#include "datasheet.h"

#include "model/lib.h"

class PdfExtract : public QObject
{
    Q_OBJECT

    void comparePdfTolib(const QString &fileName);

private slots:
    void init();
    void cleanup();

    void test_casePIC32MM_GPM();
    void test_casedsPIC33EPGS();
    void test_ucc27212a_q1();
    void test_ATtiny24();
    void test_IFX9201SG();
    void test_ticc();

protected:
    Datasheet dt;
};

void PdfExtract::comparePdfTolib(const QString &fileName)
{
    // dt.setDebugEnabled(true);
    QVERIFY(dt.open(QString("../src/autotest/%1.pdf").arg(fileName)));
    dt.analyse();

    Lib lib;
    QVERIFY(lib.readFrom(QString("../src/autotest/%1.lib").arg(fileName)));

    QCOMPARE(dt.components().count(), lib.componentsCount());
    for (int i = 0; i < dt.components().count(); i++)
    {
        Component *comp1 = dt.components()[i];
        Component *comp2 = lib.components()[i];
        QCOMPARE(comp1->pins().count(), comp2->pins().count());

        // check all pins name and pad
        comp1->sort();
        comp2->sort();
        for (int p = 0; p < comp1->pins().count(); p++)
        {
            Pin *pin1 = comp1->pins()[p];
            Pin *pin2 = comp2->pins()[p];
            QCOMPARE(pin1->name(), pin2->name());
            QCOMPARE(pin1->padName(), pin2->padName());
        }
    }
}

void PdfExtract::init()
{
}

void PdfExtract::cleanup()
{
    dt.clean();
    dt.close();
}

void PdfExtract::test_casePIC32MM_GPM()
{
    comparePdfTolib("PIC32MM_GPM_pins");
}

void PdfExtract::test_casedsPIC33EPGS()
{
    comparePdfTolib("dsPIC33EPXXGS50X_pins");
}

void PdfExtract::test_ucc27212a_q1()
{
    comparePdfTolib("ucc27212a-q1_pins");
}

void PdfExtract::test_ATtiny24()
{
    comparePdfTolib("ATtiny24_pins");
}

void PdfExtract::test_IFX9201SG()
{
    comparePdfTolib("IFX9201SG_pins");
}

void PdfExtract::test_ticc()
{
    comparePdfTolib("ticc_pins");
}

QTEST_GUILESS_MAIN(PdfExtract)

#include "tst_pdf_extract.moc"
