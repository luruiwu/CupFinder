#include "CupCollector.hpp"
#include <limits>
#include <cassert>

static point neighbours[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}}; //the neighbours a cell have

CupCollector::CupCollector(__attribute__((unused))rw::sensor::Image* map)
{
    //Convert image into mapspace map of the workspace
    CreateWorkspaceMap(map);

    //convert workspace into configurationspace
    configurationspace = workspace; //just set them as equal for now.

    //Do cell decomposition, create waypoints and cells.
    //Connect the waypoints and cells into a graph

}

CupCollector::~CupCollector()
{
    //Do cleanup
}

void CupCollector::CreateWorkspaceMap(rw::sensor::Image* map)
{
    int channel = 0; //the map is grayscale, so channel is 0
    for(uint32_t x = 0; x < map->getWidth(); x++)
    {
        std::vector<mapSpace> y_line; //Contains a single line on the y-axis
        for(uint32_t y = 0; y < map->getHeight(); y++)
        {
            mapSpace pixeltype;
            switch(map->getPixelValuei(x, y, channel))
            {
                case 0: case 128: case 129: case 130: case 131: case 132:
                    pixeltype = mapSpace::obstacle;
                    break;
                case 100:
                    pixeltype = mapSpace::dropoff;
                    break;
                case 150:
                    pixeltype = mapSpace::cup;
                    break;
                default:
                    pixeltype = mapSpace::freespace;
                    break;
            }
            y_line.push_back(pixeltype);
        }
        workspace.push_back(y_line);
    }
}

void CupCollector::SearchCell(__attribute__((unused))const Waypoint &startpoint, __attribute__((unused))const Waypoint &endpoint, __attribute__((unused))Cell &cell)
{   //Searches the given cell for cups, collects them, return them to dropoff
    //Start at startpoint and exit at endpoint.

    //Find the nearest corner

    //Go to that, find out which direction we are going.

    //Cover the cell in a shrinking spiral pattern, untill we get to the middle. For each round, define a new "smaller" cell to cover the next time

    //Go to the endpoint.
}

void CupCollector::WalkLine(vector2D const &line)
{   //Walk along the line from startpoint to endpoint
    //There should be a clear path, e.g. no obstacles.
    while(current_point != line.getEndPoint())
    {
        point next_point = FindNextPointOnLine(line);
        current_point = next_point;
    }
}

point CupCollector::FindNextPointOnLine(const vector2D &line) const
{
    //check all 8 directions collect the ones which are closer to the endpoint of the line
    // to the endpoint of the linethan the current.
    point closestpoint(-1, -1);
    float curdistance = current_point.GetDistance(line.getEndPoint());
    float mindistance = std::numeric_limits<float>::infinity();
    for(uint8_t i = 0; i < sizeof(neighbours) / sizeof(neighbours[i]); i++)
    {
        point this_point = current_point + neighbours[i];
        if(IsOutsideMap(this_point) || IsObstacle(this_point)) continue;

        if(this_point.GetDistance(line.getEndPoint()) <= curdistance)
        {   //select the wanted point as the one closest to the straight line to end.
            float distance_to_line = line.distanceToPoint(this_point);
            if(distance_to_line < mindistance)
            {
                closestpoint = this_point;
                mindistance = distance_to_line;
            }
        }
    }
    assert(closestpoint != point(-1, -1) );
    return closestpoint;
}

bool CupCollector::IsOutsideMap(const point &p) const
{
    if(p.y < 0 || p.x < 0 || p.y > size_y || p.x > size_x)
        return true;
    return false;
}

bool CupCollector::IsObstacle(const point &p) const
{
    if(configurationspace[p.x][p.y] == mapSpace::obstacle)
        return true;
    return false;
}

void CreateConfigurationspaceMap(const std::vector< std::vector< mapSpace> > inWorkspace)
// Creates a configurationspace map from input workspace map
// Saves configurationspace map as private variable configurationspace
{

}
