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
#include "scribblearea.h"

#include "handy.h"

ScribbleArea::ScribbleArea(QWidget *parent): QWidget(parent) {
     
    setAttribute(Qt::WA_StaticContents);    
    penWidth = 4;
    penColor = Qt::red;    
    scribbling = false;
    
    // Creating Allograph
    main_allograph = handy_allograph_new();
    // Creating Allograph Collection
    main_collection = handy_collection_new();
}

ScribbleArea::~ScribbleArea() {
    handy_allograph_free(main_allograph);
    handy_collection_free(main_collection);
}

// Slots

void ScribbleArea::clear() {
    image.fill(qRgb(255, 255, 255));     
    update();
}

void ScribbleArea::newAllograph() {
    handy_allograph_free(main_allograph);
    main_allograph = handy_allograph_new();        
    clear();
}

void ScribbleArea::loadAllograph() {
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Allograph"),
                                                    DATA_ALLOGRAPHS_DIRECTORY,
                                                    tr("Allograph binary files (*.bal)"));    
    if(!fileName.isEmpty()) {
        QByteArray fileNameArray = fileName.toLatin1();
        char* fileNameChar = fileNameArray.data();
        
        // Loading Allograph from disk
        handy_allograph* temp_allograph;
        temp_allograph = handy_allograph_load(fileNameChar, HANDY_SERIALIZE_BINARY);
        if(temp_allograph != NULL) {
            handy_allograph_free(main_allograph);
            main_allograph = temp_allograph;
            clear();
        } else {
            QMessageBox::critical(this, "Error loading allograph",
                                  "Error loading allograph from file " + fileName,
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
}

void ScribbleArea::saveAllograph() {
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Allograph"),
                                                    DATA_ALLOGRAPHS_DIRECTORY,
                                                    tr("Allograph binary files (*.bal)"));
    if(!fileName.isEmpty()) {
        QByteArray fileNameArray = fileName.toLatin1();
        char* fileNameChar = fileNameArray.data();

        // Saving Allograph to disk
        if(handy_allograph_save(main_allograph, fileNameChar, HANDY_SERIALIZE_BINARY) == HANDY_ERROR_CODE) {
            QMessageBox::critical(this, "Error saving allograph",
                                  "Error saving allograph to file " + fileName,
                                  QMessageBox::Ok, QMessageBox::NoButton);        
        }
    }
}

void ScribbleArea::setSymbol() {
    
    bool ok;
    QString allographSymbol = QInputDialog::getText(this, tr("Allograph symbol"),
                                                    tr("Allograph symbol:"), QLineEdit::Normal,
                                                    "", &ok);
    if(ok && !allographSymbol.isEmpty()) {
        QByteArray allographSymbolArray = allographSymbol.toLatin1();
        char* allographChar = allographSymbolArray.data();
        
        if(handy_allograph_set_symbol(main_allograph, allographChar) == HANDY_ERROR_CODE) {
            QMessageBox::critical(this, "Error with set symbol",
                                  "Error setting the symbol " + allographSymbol + " to the allograph",
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
}

void ScribbleArea::addAllograph() {
             
    bool ok;
    QString allographSymbol = QInputDialog::getText(this, tr("Allograph symbol"),
                                                    tr("Allograph symbol:"), QLineEdit::Normal,
                                                    "", &ok);
    if(ok && !allographSymbol.isEmpty()) {
        QByteArray allographSymbolArray = allographSymbol.toLatin1();
        char* allographChar = allographSymbolArray.data();
        
        if(handy_allograph_set_symbol(main_allograph, allographChar) == HANDY_ERROR_CODE) {
            QMessageBox::critical(this, "Error adding allograph",
                                  "Error setting the symbol " + allographSymbol + " to the allograph",
                                  QMessageBox::Ok, QMessageBox::NoButton);
        } else {
            handy_collection_add_allograph(main_collection, main_allograph);
        }        
    }
}

void ScribbleArea::recognizeAllograph() {
    
    // Recognizer configuration
    handy_recognize_config config;
    
    // Set recognize configuration
    config.recognize = HANDY_RECOGNIZE_DTW;
    config.distance_config.distance = HANDY_DISTANCE_DTW;
    config.distance_config.c = 0.849999;
    config.distance_config.boundary = false;
    config.distance_config.monotonicity = true;
    config.distance_config.symmetry = true;

    if(handy_allograph_recognize(main_allograph, main_collection, &config) != HANDY_ERROR_CODE) {
        QMessageBox::information(this, "Allograph recognized!!!", 
                                 "Allograph recognized with the symbol: " + QString(main_allograph->symbol),
                                 QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void ScribbleArea::normalizeAll() {
    handy_allograph_normalize(main_allograph, HANDY_NORMALIZE_ALL);
    update();
}

void ScribbleArea::normalizeRepeatedPoints() {
    handy_allograph_normalize(main_allograph, HANDY_NORMALIZE_REPEATED_POINTS);
    update();
}

void ScribbleArea::normalizeNumberOfPoints() {
    handy_allograph_normalize(main_allograph, HANDY_NORMALIZE_NUMBER_OF_POINTS);
    update();
}

void ScribbleArea::normalizeSlant() {
    handy_allograph_normalize(main_allograph, HANDY_NORMALIZE_SLANT);
    update();
}

void ScribbleArea::normalizePosition() {
    handy_allograph_normalize(main_allograph, HANDY_NORMALIZE_TRANSLATE_POINTS);
    update();
}

void ScribbleArea::normalizeSize() {
    handy_allograph_normalize(main_allograph, HANDY_NORMALIZE_SIZE_POINTS);
    update();
}

// Slots Collections

void ScribbleArea::newCollection() {
    handy_collection_free(main_collection);
    main_collection = handy_collection_new();    
}

void ScribbleArea::loadCollection() {
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Collection"),
                                                    DATA_COLLECTIONS_DIRECTORY,
                                                    tr("Collection binary files (*.bco);;UJIpen 2 (*.uji2)"));                                                    
    if(!fileName.isEmpty()) {
        
        handy_serialize format = HANDY_SERIALIZE_BINARY;
        
        if(fileName.contains(QRegExp("^.+\\.bco$"))) {
            format = HANDY_SERIALIZE_BINARY;
        } else if(fileName.contains(QRegExp("^.+\\.uji2$"))) {
            format = HANDY_SERIALIZE_UJI2;
        }
        
        QByteArray fileNameArray = fileName.toLatin1();
        char* fileNameChar = fileNameArray.data();
        
        // Loading Collection from disk
        handy_collection* temp_collection;        
        temp_collection = handy_collection_load(fileNameChar, format);
        if(temp_collection != NULL) {
            handy_collection_free(main_collection);
            main_collection = temp_collection;            
        } else {
            QMessageBox::critical(this, "Error loading collection",
                                  "Error loading collection from file" + fileName,
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
}

void ScribbleArea::saveCollection() {
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Collection"),
                                                    DATA_COLLECTIONS_DIRECTORY,
                                                    tr("Collection binary files (*.bco)"));
    if(!fileName.isEmpty()) {
        QByteArray fileNameArray = fileName.toLatin1();
        char* fileNameChar = fileNameArray.data();
        
        // Saving Collection to disk
        if(handy_collection_save(main_collection, fileNameChar, HANDY_SERIALIZE_BINARY) == HANDY_ERROR_CODE) {
            QMessageBox::critical(this, "Error saving collection",
                                  "Error saving collection to file " + fileName,
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
}

// Slots declaration end

void ScribbleArea::setPenColor(const QColor &newColor) {
    penColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth) {
    penWidth = newWidth;
}

void ScribbleArea::addPoint(QPoint point) {
    handy_point h_point;
    h_point.x = point.x();
    h_point.y = point.y();
    
    handy_allograph_add_point(main_allograph, h_point);    
}

// ScribbleArea Mouse Events redefinitions

void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {                
        // Adding Point to Allograph
        addPoint(event->pos());                            
        scribbling = true;
        
        update();       
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {    
    if((event->buttons() & Qt::LeftButton) && scribbling) {
        // Adding Point to Allograph
        addPoint(event->pos());
        update();
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event) {     
    if(event->button() == Qt::LeftButton && scribbling) {
        scribbling = false;        
        update();
    }
}

void ScribbleArea::paintEvent(QPaintEvent * /* event */) {
    // Drawing Allograph
    image.fill(qRgb(255, 255, 255));
    QPainter n_painter(&image);
    n_painter.setRenderHint(QPainter::Antialiasing);
    n_painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    
    if(main_allograph != NULL) {            
        for(unsigned int i=0; i < main_allograph->num_points; i++) { 
            n_painter.drawEllipse(main_allograph->points[i].x - penWidth/2, 
                                  main_allograph->points[i].y - penWidth/2, 
                                  penWidth, penWidth);
        }
     }
     
    // Drawing Normalization Box
    n_painter.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    n_painter.drawRect(0, 0, HANDY_NORMALIZE_HEIGHT, HANDY_NORMALIZE_HEIGHT);
        
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), image);
}

void ScribbleArea::resizeEvent(QResizeEvent *event) {
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize) {
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
