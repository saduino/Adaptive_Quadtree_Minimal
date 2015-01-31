#include "Quadtree.h"

Quadtree::Quadtree() :
left( 0 ),
right( 0 ),
top( 0 ),
down( 0 ),
numObjectsToGrow( 4 ),
nodes( 0 ),
isLeaf( true )
{
}

Quadtree::Quadtree( double _left, double _right, double _top, double _down, unsigned int _numObjectsToGrow ) :
left( _left ),
right( _right ),
top( _top ),
down( _down ),
numObjectsToGrow( _numObjectsToGrow ),
nodes( 0 ),
isLeaf( true )
{
}

Quadtree::~Quadtree()
{
}

void Quadtree::AddObject( ObjectPtr object )
{
	if ( isLeaf ) {
		objects.push_back( object );
		bool reachedMaxObjects = ( objects.size() == numObjectsToGrow );
		if ( reachedMaxObjects ) {
			createLeaves();
			moveObjectsToLeaves();
			isLeaf = false;
		}
		return;
	}

	for ( int n = 0; n < NodeCount; ++n ) {
		if ( nodes[n]->contains( object ) ) {
			nodes[n]->AddObject( object );
		}
	}
}

void Quadtree::Clear()
{
	objects.clear();

	if ( !isLeaf ) {
		for ( int n = 0; n < NodeCount; ++n ) {
			nodes[n]->Clear();
		}
	}
	
	nodes.clear();
}

std::vector<ObjectPtr> Quadtree::GetObjectsAt( double x, double y )
{
	if ( isLeaf ) {
		return objects;
	}

	std::vector<ObjectPtr> returnedObjects;
	std::vector<ObjectPtr> childObjects;

	if ( !objects.empty() )
		returnedObjects.insert( returnedObjects.end(), objects.begin(), objects.end() );

	for ( int n = 0; n < NodeCount; ++n ) {
		if ( nodes[n]->contains( x, y ) ) {
			childObjects = nodes[n]->GetObjectsAt( x, y );
			returnedObjects.insert( returnedObjects.end(), childObjects.begin(), childObjects.end() );
			break;
		}
	}
	
	return returnedObjects;
}

bool Quadtree::contains( ObjectPtr object )
{
	if (object->left > left && object->left < right && 
        object->top > top && object->top < down)
        return true;
    
    else if (object->right > left && object->right < right && 
        object->top > top && object->top < down)
        return true;
    
    else if (object->left > left && object->left < right && 
        object->down > top && object->down < down)
        return true;
    
    else if (object->right > left && object->right < right && 
        object->down > top && object->down < down)
        return true;
    
    else
        return false;
}

bool Quadtree::contains( double x, double y )
{
	return 	( x >= left && x <= right ) &&
		( y >= top && y <= down );
}

void Quadtree::createLeaves()
{
	Quadtree* quad0 = new Quadtree( left, (left+right)/2, top, (top+down)/2, numObjectsToGrow );
  nodes.push_back(boost::shared_ptr<Quadtree>(quad0));
  Quadtree* quad1 = new Quadtree( (left+right)/2, right, top, (top+down)/2, numObjectsToGrow );
  nodes.push_back(boost::shared_ptr<Quadtree>(quad1));
  Quadtree* quad2 = new Quadtree( left, (left+right)/2, (top+down)/2, down, numObjectsToGrow );
  nodes.push_back(boost::shared_ptr<Quadtree>(quad2));
  Quadtree* quad3 = new Quadtree( (left+right)/2, right, (top+down)/2, down, numObjectsToGrow );
  nodes.push_back(boost::shared_ptr<Quadtree>(quad3));
}

void Quadtree::moveObjectsToLeaves()
{
	for ( unsigned int m = 0; m < objects.size(); ++m ) {
        for ( int n = 0; n < NodeCount; ++n ) {
            if ( nodes[n]->contains( objects[m] ) ) {
                nodes[n]->AddObject( objects[m] );
            }
        }
        objects.erase( objects.begin() + m );
        --m;
    }
}
