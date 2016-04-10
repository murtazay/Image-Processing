// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by Murtaza Yaqoob
//
// Median.h - Median widget
//
// Written  by: Murtaza Yaqoob, 2016
// ======================================================================
#ifndef MEDIAN_H
#define MEDIAN_H

#include "ImageFilter.h"

class Median : public ImageFilter{
    Q_OBJECT
public:
    Median(QWidget *parent = 0);                    // constructor
    QGroupBox*	controlPanel	             ();	// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);    // apply filter to input to init output
    void		reset		                 ();	// reset parameters

protected:
    void median(ImagePtr I1, double size, double average, ImagePtr I2);

protected slots:
    void setSize(int);
    void setAverage(int);

private:

    QSlider     *m_slider[2];
    QSpinBox    *m_spinBox[2];
    QGroupBox	*m_ctrlGrp;	// groupbox for panel
};

#endif // MEDIAN_H
