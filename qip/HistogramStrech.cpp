// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by Murtaza Yaqoob
//
// HistogramStrech.cpp - Histogram Strech widget.
//
// Written  by: Murtaza Yaqoob, 2016
// ======================================================================

#include "MainWindow.h"
#include "HistogramStrech.h"
#include "IPdefs.h"

extern MainWindow *g_mainWindowP;

HistogramStrech::HistogramStrech(QWidget *parent)
{

}

QGroupBox *HistogramStrech::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Histogram Streching");

    // INSERT YOUR CODE HERE
    // init widgets
    // create label
    QLabel *label[2] = {new QLabel,new QLabel};
    label[0]->setText("Min");
    label[1]->setText("Max");

    // create sliders
    m_slider[MIN] = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider[MIN]->setMinimum(0);
    m_slider[MIN]->setMaximum(255);
    m_slider[MIN]->setTickPosition(QSlider::TicksBelow);
    m_slider[MIN]->setTickInterval(32);
    m_slider[MIN]->setValue(0);

    m_slider[MAX] = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider[MAX]->setMinimum(0);
    m_slider[MAX]->setMaximum(255);
    m_slider[MAX]->setTickPosition(QSlider::TicksBelow);
    m_slider[MAX]->setTickInterval(32);
    m_slider[MAX]->setValue(255);

    // create spinboxes
    m_spinBox[MIN] = new QSpinBox(m_ctrlGrp);
    m_spinBox[MIN]->setMinimum(0);
    m_spinBox[MIN]->setMaximum(255);
    m_spinBox[MIN]->setValue(0);

    m_spinBox[MAX] = new QSpinBox(m_ctrlGrp);
    m_spinBox[MAX]->setMinimum(0);
    m_spinBox[MAX]->setMaximum(255);
    m_spinBox[MAX]->setValue(255);

    // init signal/slot connections
    connect(m_slider [MIN], SIGNAL(valueChanged(int)), this,            SLOT(setMin(int)));
    connect(m_slider [MIN], SIGNAL(valueChanged(int)), m_spinBox[MIN],  SLOT(setValue(int)));
    connect(m_spinBox[MIN], SIGNAL(valueChanged(int)), m_slider [MIN],  SLOT(setValue(int)));


    connect(m_slider [MAX], SIGNAL(valueChanged(int)), this,            SLOT(setMax(int)));
    connect(m_slider [MAX], SIGNAL(valueChanged(int)), m_spinBox[MAX],  SLOT(setValue(int)));
    connect(m_spinBox[MAX], SIGNAL(valueChanged(int)), m_slider [MAX],  SLOT(setValue(int)));

    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(    label[MIN], 0, 0);
    layout->addWidget( m_slider[MIN], 0, 1);
    layout->addWidget(m_spinBox[MIN], 0, 2);
    layout->addWidget(    label[MAX], 1, 0);
    layout->addWidget( m_slider[MAX], 1, 1);
    layout->addWidget(m_spinBox[MAX], 1, 2);

    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    return(m_ctrlGrp);

}

bool HistogramStrech::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // INSERT YOUR CODE HERE
    // error checking
    if(I1.isNull()) return 0;

    // get level value
    int min = m_slider[MIN]->value();
    int max = m_slider[MAX]->value();
    if(min > max || min == max){
        return 0;
    }

    // apply filter
    histstrech(I1, min, max, I2);

    return 1;
}

void HistogramStrech::reset()
{
    m_slider[MIN]->setValue(0);
    m_slider[MAX]->setValue(255);
}

void HistogramStrech::histstrech(ImagePtr I1, int minGray, int maxGray, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I2->height();
    int total = w * h;

    // compute lut[]
    int i;
    double lut[MXGRAY];
    for(i=0;i<MXGRAY;++i){
        lut[i] = CLIP((255*(i-minGray)/(maxGray-minGray)),0,255);
    }

    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }
}

void HistogramStrech::setMin(int min)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}

void HistogramStrech::setMax(int max)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}
