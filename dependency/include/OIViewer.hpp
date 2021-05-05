//
// Created by jash on 07/02/19.
//

#ifndef OPENISS_VIEWER_H
#define OPENISS_VIEWER_H

class OIViewer
{
public:
    virtual ~OIViewer() = 0;
    virtual void draw() = 0;
    virtual void setup() = 0;
};

#endif // OPENISS_VIEWER_H
