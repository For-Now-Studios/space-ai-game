#include<cmath>
#include "graph.h"
#include "structs.h"

using namespace std;

Graph<Room *, int> *initPathfinding(vector<Room *> *v){
	Graph<Room *, int> *g = new Graph<Room *, int>();

	for(Room *r : *v){
		//Add the room to our pathfinding graph
		g->addNode(r);

		//Controll if the new room connects to any previous room
		for(pair<Room *, Node<Room *, int> *> p : g->nodes){
			if(r != p.first){
				//Do this by comparing if you share a door
				for(Door *d1 : r->doors){
					for(Door *d2 : p.first->doors){
						if(d1 == d2){
							g->addEdge(r, p.first, 1);
							g->addEdge(p.first, r, 1);
						}
					}
				}
			}
		}
	}

	return g;
}

/*
	Determines which room the given object is inside of
*/
Room *whichRoom(vector<Room *> *v, GameObject *obj){
	for(int i = 0; i < v->size(); i++) {
		SDL_Rect area;
		if(obj->image == nullptr){
			area = {obj->x, obj->y, 1, 1};
		}
		else{
			area = {obj->x, obj->y, obj->image->width, obj->image->height};
		}

		if(SDL_HasIntersection(&v->at(i)->area, &area)) {
			return v->at(i);
		}
	}

	return nullptr;
}

/*
	Finds the shortest path from the specified beacon/room, to the target
*/
vector<GameObject *> *findPathTo(Graph<GameObject *, int> *g, GameObject *from,
								GameObject *to){	
	return dijkstra(g, from, to);
}

/*
	Find a door which the two specified rooms share
*/
Door *sharedDoor(Room *a, Room *b){
	for(Door *d1 : a->doors){
		for(Door *d2 : b->doors){
			if(d1 == d2) return d1;
		}
	}

	return nullptr;
}

void updateMovement(CharacterObject *object, vector<Room *> *rooms,
						Graph<Room *, int> *g){
	//Check if the character wants to move anywhere
	if(object->goal == nullptr) return;

	//Calculate a path to the target if no such path has been calculated yet
	if(object->path == nullptr){
		Room *start = whichRoom(rooms, object);
		Room *end = whichRoom(rooms, object->goal);
		object->path = (vector<Room *> *) findPathTo(
					(Graph<GameObject *, int> *) g, start, end);
		object->target = sharedDoor(start, object->path->back());
	}

	if(!object->path->empty()){
		if(SDL_HasIntersection(&object->area, &object->path->back()->area)){
			Room *current = object->path->back();
			object->path->pop_back();

			if(object->path->empty()) return;

			object->target = sharedDoor(current, object->path->back());
		}
	}
	else{
		//The character is in the correct room, so walk to the goal
		object->target = object->goal;
	}

	int xSpeed = object->speed;
	int ySpeed = 0;

	//Controll that the speed isn't too high
	if(abs(object->x - object->target->x) - xSpeed < 0){
		xSpeed = abs(object->x - object->target->x);
	}

	//We are done moving in the xDirection, move in yDirection
	if(xSpeed == 0){
		ySpeed = object->speed;
		//Controll that the speed isn't too high
		if(abs(object->y - object->target->y) - ySpeed < 0){
			ySpeed = abs(object->y - object->target->y);
		}

		if(object->y > object->target->y){
			ySpeed *= -1;
		}
	}
	else{
		if(object->x > object->target->x){
			xSpeed *= -1;
		}
	}

	//Check if the goal has been reached
	if(xSpeed == 0 && ySpeed == 0){
		object->goal = nullptr;
		delete object->path;
		object->path = nullptr;
		object->target = nullptr;
	}

	object->moveBy(xSpeed, ySpeed);
}
