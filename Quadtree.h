#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "Object.h"

#include <vector>

#include <boost/shared_ptr.hpp>

class Quadtree;

typedef boost::shared_ptr<Quadtree> QuadtreePtr;
typedef boost::shared_ptr<Object> ObjectPtr;

class Quadtree {

enum Node {
	NW = 0,
	NE,
	SW,
	SE,
	NodeCount
};

public:
					Quadtree();

					Quadtree( double left, double right, double top, double down, unsigned int numObjectsToGrow = 3 );

					~Quadtree();

	void				AddObject( ObjectPtr object );

	void				Clear();

	std::vector<ObjectPtr>			GetObjectsAt( double x, double y );

private:
	double				left;

	double				right;

	double				top;

	double				down;

	unsigned int			numObjectsToGrow;

	std::vector<ObjectPtr> objects;

	std::vector<QuadtreePtr> nodes;

	bool				isLeaf;
	
	bool				contains( ObjectPtr object );
	
	bool				contains( double x, double y );

	void				createLeaves();

	void				moveObjectsToLeaves();
	
};

#endif
