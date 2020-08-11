#ifndef FaceBounds_h
#define FaceBounds_h

#include <vector>

struct Bounds {
    int x, y, width, height;

    int area() const
    {
        return width * height;
    }

    /**
     * Greater-than operator
     * Used to sort vectors of Bounds by area
     *
     * @param comp
     * @return
     */
    bool operator>( const Bounds &comp ) const
    {
        return area() > comp.area();
    }
};

struct FaceCropBounds {
    Bounds face, crop;
};

typedef std::vector<Bounds> FacesDetected;


#endif //FaceBounds_h
