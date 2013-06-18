//
//  TrackChooserView.cpp
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

#include "TrackChooserView.h"
#include "ResourcePath.hpp"


TrackChooserView::TrackChooserView(const Rect& frame) :
View(frame){
    // Get a list of all tracks
    Rac0r::TrackFileManager fileManager;
    tracks = fileManager.getTrackList();
    
    // Load textures
    arrowDownTexture.loadFromFile(resourcePath() + "arrowDown.png");
    arrowUpTexture.loadFromFile(resourcePath() + "arrowUp.png");
    std::cout << tracks.at(trackNumber).getTrackFile() << std::endl;
    
    // Initialize arrows and the track image
    arrowUp.setTexture(arrowUpTexture);
    arrowDown.setTexture(arrowDownTexture);
    addChild(arrowUp);
    addChild(arrowDown);
    arrowUp.rotate(90);
    arrowDown.rotate(90);
    addChild(track);
    layoutChildviews();
    
}

void TrackChooserView::handleEvent(sf::Event event) {
    //event handling
    bool pressed = (event.type == sf::Event::KeyPressed);
    bool released = (event.type == sf::Event::KeyReleased);

    if (pressed && event.key.code == sf::Keyboard::Right) {
        //choose a track (next)
        if (trackNumber < tracks.size() - 1){
            trackNumber++;
            std::cout << tracks.at(trackNumber).getTrackFile() << std::endl;
            layoutChildviews();
            arrowUp.move(6,0);
                        
        }
    }else if (released && event.key.code == sf::Keyboard::Right){
        layoutChildviews();
    }
    if (pressed && event.key.code == sf::Keyboard::Left) {
        //choose a track (previous)
        if (trackNumber > 0){
            trackNumber--;
            std::cout << tracks.at(trackNumber).getTrackFile() << std::endl;
            layoutChildviews();
            arrowDown.move(-6, 0);
        }
    } else if (released && event.key.code == sf::Keyboard::Left) {
        layoutChildviews();
    }
}

// This is called whenever the size is set
void TrackChooserView::layoutChildviews() {
    sf::Vector2f position;
    sf::Vector2f size;
    
    track.setPosition(frame.width / 2 - 320, 0);
    trackTexture = sf::Texture();
    std::string filename = tracks.at(trackNumber).getImageFile();
    if (!trackTexture.loadFromFile(filename)){
        std::cout<< "fehler beim laden des image"<< std::endl;
    }
    track.setTexture(trackTexture);
    
    
    // Arrows
    size = sf::Vector2f(arrowUpTexture.getSize());
    position.x = frame.width - size.x + 430;
    position.y = 150;
    arrowUp.setPosition(position);
    
    size = sf::Vector2f(arrowDownTexture.getSize());
    position.x -= 740;
    arrowDown.setPosition(position);
    
    
    std::cout << tracks.at(trackNumber).getTrackFile() << std::endl;
    trackTexture = sf::Texture();
    
    filename = tracks.at(trackNumber).getImageFile();
    std::cout << filename << std::endl;
    if (!trackTexture.loadFromFile(filename)){
        std::cout<< "fehler beim laden des image"<< std::endl;
    }
    track.setTexture(trackTexture);
}

std::string TrackChooserView::getTrackPath() {
    return tracks.at(trackNumber).getTrackFile();
}