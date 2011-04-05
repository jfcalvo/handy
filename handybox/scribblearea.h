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

#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

#include "handy.h"

#define DATA_ALLOGRAPHS_DIRECTORY "../data/allographs"
#define DATA_COLLECTIONS_DIRECTORY "../data/collections"

class ScribbleArea : public QWidget {
    
    Q_OBJECT
    
    public:
        ScribbleArea(QWidget* parent = 0);
        ~ScribbleArea();
        
        void setPenColor(const QColor &newColor);
        void setPenWidth(int newWidth);        
        
    public slots:
        void clear();
        void newAllograph();
        void loadAllograph();
        void saveAllograph();
        void setSymbol();
        void addAllograph();
        void recognizeAllograph();
        void normalizeAll();
        void normalizeRepeatedPoints();
        void normalizeNumberOfPoints();
        void normalizeSlant();
        void normalizePosition();
        void normalizeSize();
        // Collections
        void newCollection();
        void loadCollection();
        void saveCollection();
                
    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent *event);
        void resizeEvent(QResizeEvent *event);
        
    private:
        void addPoint(QPoint point);
        void resizeImage(QImage *image, const QSize &newSize);        
        
        int    penWidth;
        QColor penColor;

        bool scribbling;

        QImage image;
        
        // Handy Utils
        handy_allograph* main_allograph;
        handy_collection* main_collection;
};

#endif /* SCRIBBLEAREA_H */
