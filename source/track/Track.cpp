//
//  Track.cpp
//  Rac0r
//
//  Created and copyright by
//  Benjamin Hintz
//  Florian Kaluschke
//  Jannes Meyer
//  David Leska
//  Lars Peterke
//  Jan Schulte
//  on Jun 2013. All rights reserved.
//

#include "Track.h"

#include <cfloat>

#include "../utils/vector2.h"

namespace Rac0r {
 
Track::Track() :
    Track(Points(), "Undefined", "Undefined")
{
}

Track::Track(const Points & _points, const std::string & _name, const std::string & _author) :
    mPoints(_points),
    mCenter(0.0f, 0.0f),
    mScale(1.0f),
    mName(_name),
    mAuthor(_author)
{
    this->computeBounds();
}
  
void Track::setPoints(const Points & _value) {
    
    this->mPoints = _value;
    this->mScale = 1.0f;

    this->computeBounds();
}

void Track::setScale(float _value, bool _respectCurveDirection) {
    
    if (_value == 0.0f) {
        return;
    }
    
    if (!_respectCurveDirection) {
        for (auto & point : this->mPoints) {
            sf::Vector2f newPoint = point;
            // move to origin
            newPoint -= this->mCenter;
            // scale
            newPoint *= _value;
            // move back
            newPoint += this->mCenter;
            // store
            point = newPoint;
        }
    } else {
        sf::Vector2f lastDir;
        sf::Vector2f firstPoint = (*this)[0];

        for (size_t i = 0; i < this->mPoints.size(); ++i) {
            sf::Vector2f curPoint = (*this)[i];
            sf::Vector2f nextPoint = (*this)[i+1];
            
            if (i+1 == this->mPoints.size()) {
                nextPoint = firstPoint;
            }
            
            // compute point othogonal
            sf::Vector2f dir = orthogonal(normalize(nextPoint - curPoint)) * _value;
            
            lastDir = dir;
            // move to origin
            curPoint -= this->mCenter;
            // scale
            curPoint += dir;
            // move back
            curPoint += this->mCenter;
            // store
            this->mPoints[i] = curPoint;
        }
    }
    
    this->mScale = _value;
    this->computeBounds();
    
    std::cout << "Track Scale: Scale(" << this->mScale << ")" << std::endl;
}

void Track::scaleToFitBounds(const sf::Vector2f _value, bool _respectCurveDirection, float _adjustValue) {
    
    float width = this->mBounds[1].x - this->mBounds[0].x;
    float height = this->mBounds[1].y - this->mBounds[0].y;
    float scale = fmin(_value.x / width, _value.y / height) + _adjustValue;
    
    this->setScale(scale, _respectCurveDirection);
}

void Track::setCenter(const sf::Vector2f & _value) {

    sf::Vector2f offset = _value - this->mCenter;

    for (auto & point : this->mPoints) {
        point += offset;
    }
    
    this->mCenter = _value;
    
    std::cout << "Track Center: Center(" << this->mCenter << ")" << std::endl;
}


void Track::computeBounds() {

    this->mBounds[0] = sf::Vector2f(FLT_MAX, FLT_MAX); // top-left
    this->mBounds[1] = sf::Vector2f(FLT_MIN, FLT_MIN); // bottom_right
    
    for (auto & point : this->mPoints) {
        this->mBounds[0].x = fmin(point.x, this->mBounds[0].x);
        this->mBounds[0].y = fmin(point.y, this->mBounds[0].y);
        this->mBounds[1].x = fmax(point.x, this->mBounds[1].x);
        this->mBounds[1].y = fmax(point.y, this->mBounds[1].y);
    }
 
    this->mCenter = (this->mBounds[0] + this->mBounds[1]) / 2.0f;
    
    std::cout << "Track Center: Center(" << this->mCenter << ")" << std::endl;
    std::cout << "Track Bounds: Top-Left(" << this->mBounds[0] << ") - Bottom-Right(" << this->mBounds[1] << ")" << std::endl;
}

bool Track::findSegment(const sf::Vector2f & _currentLocation, size_t _startIndex, size_t & _segmentStart, size_t & _segmentEnd) const {

    for (size_t i = _startIndex; i < this->mPoints.size(); ++i) {
        sf::Vector2f firstPoint = (*this)[i];
        sf::Vector2f secondPoint = (*this)[i+1];
        
        sf::Vector2f projectedPoint = project(_currentLocation, firstPoint, secondPoint);
        sf::Vector2f segmentDir = Rac0r::normalize(secondPoint - firstPoint);
        
        sf::Vector2f firstLocationDir = Rac0r::normalize(projectedPoint - firstPoint);
        sf::Vector2f secondlocationDir = Rac0r::normalize(projectedPoint - secondPoint);
        
        float firstDir = Rac0r::scalar(segmentDir, firstLocationDir);
        float secondDir = Rac0r::scalar(segmentDir, secondlocationDir);
        
        if (firstDir < 0.0f && secondDir < 0.0f) {
            //std::cout << "Warning: Failed to find segment in time." << std::endl;
            return false;
        }
        
        if (firstDir >= 0.0f && secondDir <= 0.0f) { // segment found
            _segmentStart = i;
            _segmentEnd = i+1;
            
            if (_segmentEnd >= this->mPoints.size()) {
               _segmentEnd = this->mPoints.size() - _segmentEnd;
               //std::cout << "End reached" << std::endl;
            }
            
            //std::cout << "New Segment found (Start: " << _segmentStart << ", End: " << _segmentEnd << std::endl;
            return true;
        }
        
        // hack to start at the beginning again...
        if (i+1 >= this->mPoints.size()) {
            i = -1;
        }
    }
    
    return false;
}
    
const sf::Vector2f & Track::operator[] (size_t _index) const {
    
    if (_index >= this->mPoints.size()) {
        size_t index = this->mPoints.size() - _index;
        return (*this)[index];
    } else {
        return this->mPoints[_index];
    }
}


}