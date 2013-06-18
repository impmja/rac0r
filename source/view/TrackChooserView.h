//
//  TrackChooserView.h
//  Rac0r
//
//  Created and copyright by
//  Benjamin Hintz
//  Florian Kaluschke
//  David Leska
//  Lars Peterke
//  Jan Schulte
//  on Jun 2013. All rights reserved.
//

#ifndef __Rac0r__TrackChooserView__
#define __Rac0r__TrackChooserView__

#include <iostream>
#include "View.h"
#include <string>
#include "../track/TrackFileManager.h"

class TrackChooserView:public View{
public:
    TrackChooserView(const Rect& frame = Rect(0,0,0,0));
    virtual ~TrackChooserView() = default;
    
    std::string getTrackPath();
    virtual void handleEvent(sf::Event event);
    
protected:
    virtual void layoutChildviews();
    
private:
    int trackNumber = 0;
    std::vector<Rac0r::TrackFile> tracks;
    int minValue;
    int maxValue;
    sf::Sprite track;
    sf::Texture arrowDownTexture;
    sf::Texture arrowUpTexture;
    sf::Sprite arrowUp;
    sf::Sprite arrowDown;
    sf::Texture trackTexture;
};


#endif /* defined(__Rac0r__TrackChooserView__) */