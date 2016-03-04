// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by Murtaza Yaqoob
//
// Gamma.cpp - Gamma widget.
//
// Written  by: Murtaza Yaqoob, 2016
// ======================================================================

#include "MainWindow.h"
#include "Gamma.h"
#include "math.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::Gamma:
//
// Constructor.
//
Gamma::Gamma(QWidget *parent)
{

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::controlPanel:
//
// Create group box for control panel.
//
QGroupBox *Gamma::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Gamma Correction");

    // INSERT YOUR CODE HERE
    // init widgets
    // create label
    QLabel *label = new QLabel;
    label->setText("Alpha");

    // create sliders
    m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider->setMinimum(-5);
    m_slider->setMaximum( 5);
    m_slider->setValue(3);
    m_slider->setTickPosition(QSlider::TicksBelow);
    m_slider->setTickInterval(1);

    // create spinbox
    m_spinBox = new QSpinBox(m_ctrlGrp);
    m_spinBox->setMinimum(-5);
    m_spinBox->setMaximum( 5);
    m_spinBox->setValue(3);

    // init signal/slot connections
    connect(m_slider , SIGNAL(valueChanged(int)),      this,  SLOT(setAlpha(int)));
    connect(m_slider , SIGNAL(valueChanged(int)), m_spinBox,  SLOT(setValue(int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), m_slider ,  SLOT(setValue(int)));

    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(    label, 0, 0);
    layout->addWidget( m_slider, 0, 1);
    layout->addWidget(m_spinBox, 0, 2);

    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    return(m_ctrlGrp);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool Gamma::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // INSERT YOUR CODE HERE
    // error checking
    if(I1.isNull()) return 0;

    // get level value
    int alpha = m_slider->value();
    double a = (double) alpha;


    // apply filter
    gamma(I1, a, I2);

    return 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::reset:
//
// Reset parameters.
//
void Gamma::reset()
{
    m_slider->setValue(3);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// gamma:
//
// INSERT YOUR CODE HERE.
//
void Gamma::gamma(ImagePtr I1, double alpha, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I2->height();
    int total = w * h;

    // compute lut[]
    int i;
    double lut[MXGRAY];
    for(i=0;i<MXGRAY;++i){
        lut[i] = pow((double)i/MXGRAY, 1/alpha) * MXGRAY;
        //lut[i] = MXGRAY * ((i/MXGRAY)^(1/alpha));
    }

    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::changeAlpha:
//
// Slot to process change in alp caused by moving the slider.
//
void Gamma::setAlpha(int alp)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}
