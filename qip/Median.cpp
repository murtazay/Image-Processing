// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by Murtaza Yaqoob
//
// Sharpen.cpp - Sharpen widget.
//
// Written  by: Murtaza Yaqoob, 2016
// ======================================================================

#include "MainWindow.h"
#include "Median.h"
#include <algorithm>

Median::Median(QWidget *parent)
{

}

QGroupBox *Median::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Median");

    // INSERT YOUR CODE HERE
    // init widgets
    // create label
    QLabel *labelx = new QLabel;
    labelx->setText("Filter Size");
    QLabel *labely = new QLabel;
    labely->setText("Factor");

    // create sliders
    m_slider[0] = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider[0]->setMinimum(3);
    m_slider[0]->setMaximum(7);
    m_slider[0]->setValue(3);
    m_slider[0]->setTickPosition(QSlider::TicksBelow);
    m_slider[0]->setTickInterval(2);

    m_slider[1] = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider[1]->setMinimum(0);
    m_slider[1]->setMaximum(10);
    m_slider[1]->setValue(0);
    m_slider[1]->setTickPosition(QSlider::TicksBelow);
    m_slider[1]->setTickInterval(1);

    // create spinbox
    m_spinBox[0] = new QSpinBox(m_ctrlGrp);
    m_spinBox[0]->setSingleStep(2);
    m_spinBox[0]->setMinimum(3);
    m_spinBox[0]->setMaximum(7);
    m_spinBox[0]->setValue(3);

    m_spinBox[1] = new QSpinBox(m_ctrlGrp);
    m_spinBox[1]->setMinimum(0);
    m_spinBox[1]->setMaximum(10);
    m_spinBox[1]->setValue(0);

    // init signal/slot connections
    connect(m_slider[0]  , SIGNAL(valueChanged(int)),         this,   SLOT(setSize(int)));
    connect(m_slider[0]  , SIGNAL(valueChanged(int)), m_spinBox[0],   SLOT(setValue(int)));
    connect(m_spinBox[0] , SIGNAL(valueChanged(int)), m_slider[0] ,   SLOT(setValue(int)));

    connect(m_slider[1]  , SIGNAL(valueChanged(int)),         this,   SLOT(setAverage(int)));
    connect(m_slider[1]  , SIGNAL(valueChanged(int)), m_spinBox[1],   SLOT(setValue(int)));
    connect(m_spinBox[1] , SIGNAL(valueChanged(int)), m_slider[1] ,   SLOT(setValue(int)));
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(      labelx, 1, 0);
    layout->addWidget( m_slider[0], 1, 1);
    layout->addWidget(m_spinBox[0], 1, 2);
    layout->addWidget(      labely, 2, 0);
    layout->addWidget( m_slider[1], 2, 1);
    layout->addWidget(m_spinBox[1], 2, 2);

    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    return(m_ctrlGrp);
}

bool Median::applyFilter(ImagePtr, ImagePtr)
{
    // INSERT YOUR CODE HERE
    // error checking
    if(I1.isNull()) return 0;

    // get level value
    int sz = m_spinBox[0]->value();
    int av = m_spinBox[1]->value();

    // apply filter
    median(I1,sz,av,I2);

    return 1;
}

void Median::reset()
{
    m_slider[0]->setValue(3);
    m_slider[1]->setValue(0);
}

void Median::median(ImagePtr I1, double size, double average, ImagePtr I2)
{
   IP_copyImageHeader(I1, I2);
   int w = I1->width();
   int h = I2->height();
   int total = w * h;

   int type;
   ChannelPtr<uchar> p1, p2;
   for(int ch = 0; IP_getChannel(I1, ch, p1, type); ++ch){
        IP_getChannel(I2,ch,p2,type);
        for(int i = 0; i < h; ++i){
            med(p1, p2, w, size, w, average);
            p1++;
            p2++;
        }
   }
}

void Median::setSize(int)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}

void Median::setAverage(int)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}
