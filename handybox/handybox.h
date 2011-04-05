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

#ifndef HANDYBOX_H
#define HANDYBOX_H

#include "ui_handybox.h"

class ScribbleArea;
 
class HandyBox : public QMainWindow, private Ui::HandyBox {
    
    Q_OBJECT
 
    public:
        HandyBox(QMainWindow* parent = 0);
        
    public slots:
        void about();
        
    private:
        ScribbleArea *scribbleArea;
};

#endif /* HANDYBOX_H */
