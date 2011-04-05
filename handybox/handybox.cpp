/*
    handybox
    Copyright (C) 2009-2010 José Francisco Calvo Moreno
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
    
    José Francisco Calvo Moreno
    josefranciscocalvo@gmail.com
*/

#include <QtGui>

#include "handybox.h"
#include "scribblearea.h"

#include "handy.h"

HandyBox::HandyBox(QMainWindow* parent) {
            
    setupUi(this);
    
    scribbleArea = new ScribbleArea(this->centralwidget);
    scribbleArea->setGeometry(0, 0, 380, 530);
    
    // Signal-slots mechanism
    connect(actionAbout_HandyBox, SIGNAL(triggered()), this, SLOT(about()));       
    connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    
    // Allograph Actions
    connect(actionNew_Allograph, SIGNAL(triggered()), scribbleArea, SLOT(newAllograph()));
    connect(actionLoad_Allograph, SIGNAL(triggered()), scribbleArea, SLOT(loadAllograph()));
    connect(actionSave_Allograph, SIGNAL(triggered()), scribbleArea, SLOT(saveAllograph()));
    connect(actionSet_Symbol, SIGNAL(triggered()), scribbleArea, SLOT(setSymbol()));
    connect(actionAdd_Allograph, SIGNAL(triggered()), scribbleArea, SLOT(addAllograph()));
    connect(actionRecognize_Allograph, SIGNAL(triggered()), scribbleArea, SLOT(recognizeAllograph()));
    
    // Normalizations    
    connect(actionNormalize_Repeated_Points, SIGNAL(triggered()), scribbleArea, SLOT(normalizeRepeatedPoints()));
    connect(actionNormalize_number_of_points, SIGNAL(triggered()), scribbleArea, SLOT(normalizeNumberOfPoints()));
    connect(actionNormalize_Slant, SIGNAL(triggered()), scribbleArea, SLOT(normalizeSlant()));
    connect(actionNormalize_Position, SIGNAL(triggered()), scribbleArea, SLOT(normalizePosition()));
    connect(actionNormalize_Size, SIGNAL(triggered()), scribbleArea, SLOT(normalizeSize()));
    connect(actionNormalize_All, SIGNAL(triggered()), scribbleArea, SLOT(normalizeAll()));
    
    // Collection Actions
    connect(action_New_Collection, SIGNAL(triggered()), scribbleArea, SLOT(newCollection()));
    connect(action_Load_Collection, SIGNAL(triggered()), scribbleArea, SLOT(loadCollection()));
    connect(actionSave_Collection, SIGNAL(triggered()), scribbleArea, SLOT(saveCollection()));
}

void HandyBox::about() {
    QMessageBox::about(this,"About HandyBox", "HandyBox v.1.0");
}
