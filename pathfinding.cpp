#include<cmath>
#include "graph.h"
#include "structs.h"
#include "clickfunctions.h"

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

/*
	Targets an arbitrary game object, making sure that the character so that the
	lower left corners of the two align
*/
void targetGameObject(CharacterObject *c, GameObject *obj){
		c->target = obj;
		c->xDist = obj->x - c->x;
		c->yDist = (obj->y + obj->image->height) - (c->y + c->area.h);
}

/*
	Handles the targeting of doors and ladders
*/
void targetDoor(CharacterObject *c, Door *d){
	c->target = d;

	// Check if the door is a hatch
	if(d->bottom != nullptr){
		// Check if we are beneath the hatch
		if(d->y < c->y){
			//Have we arrived at the base of the ladder to the hatch?
			if(d->bottom->x == c->x &&
			(d->bottom->y + d->bottom->image->height) == (c->y + c->area.h)){
				c->xDist = d->x - c->x;
				c->yDist = d->y - (c->y + c->area.h);
			}
			else{
				//Target the base of the ladder instead
				c->target = d->bottom;
				c->xDist = c->target->x - c->x;
				c->yDist = c->target->y + c->target->image->height - 
								(c->y + c->area.h);
			}
		}
		else{
			//Are we on top of the hatch yet?
			if(d->x == c->x && d->y == c->y + c->area.h){
				//Target the base of the ladder instead
				c->target = d->bottom;
				c->xDist = c->target->x - c->x;
				c->yDist = c->target->y + c->target->image->height - 
								(c->y + c->area.h);

			}
			else{
				c->xDist = d->x - c->x;
				c->yDist = d->y - (c->y + c->area.h);
			}
		}
	}
	else{ //Target the door
		c->xDist = (d->x + d->area.w) - c->x;
		if(c->xDist <= 0) c->xDist = d->x - (c->x + c->area.w);

		c->yDist = (d->y + d->area.h) - (c->y + c->area.h);
	}
}

void checkDoor(CharacterObject *object, Door *d, vector<Room *> *rooms,
					Graph<Room *, int> *g, int xSpeed, int ySpeed){
	if(!d->IsOpen){                             		
		object->moveBy(-xSpeed, -ySpeed);
		object->xDist += xSpeed;
		object->yDist += ySpeed;

		if(d->IsLocked){
			//Remember the room we were heading to
			Room *r = nullptr;
			if(!object->path->empty()){
				r = object->path->back();
			}
			else{
				printf("Error: Trying to access a ");
				printf("locked door in pathfinding ");
				printf("after reaching the final ");
				printf("room of that path!\n");
			}

			// Forget the current path to the goal
			delete object->path;
			object->path = nullptr;
			object->target = nullptr;

			//Make a new mental model of the ship
			Room *start = whichRoom(rooms, object);
			int t = g->getEdgeValue(start, r);
			g->updateEdge(start, r, INT_MAX);
			g->updateEdge(r, start, INT_MAX);

			//Figure out where to go
			Room *end = whichRoom(rooms, object->goal);
			object->path = (vector<Room *> *) findPathTo(
				(Graph<GameObject *, int> *) g, start, end);

			//Restore the graph
			g->updateEdge(start, r, t);
			g->updateEdge(r, start, t);

			if(object->path == nullptr) return;

			targetDoor(object, sharedDoor(start,
							object->path->back()));
		}
		else{
			DoorClickPars *data = (DoorClickPars *) d->data;
			d->image = data->open;
			d->IsOpen = true;
		}
	}
	
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

		printf("Tried to find path\n");
		if(object->path == nullptr) return;
		printf("Found path\n");

		targetDoor(object, sharedDoor(start, object->path->back()));
	}

	if(object->target == nullptr){
		if(!object->path->empty()){
			//If we have arrived at the next target room
			if(SDL_HasIntersection(&object->area,
							&object->path->back()->area)){
				Room *current = object->path->back();
				object->path->pop_back();
	
				if(object->path->empty()) return;
	
				//Find the next target door
				targetDoor(object, sharedDoor(current,
								object->path->back()));
			}
			else{
				Room *start = whichRoom(rooms, object);
				targetDoor(object, sharedDoor(start,
								object->path->back()));
			}
		}
		else{
			//The character is in the correct room, so walk to the goal
			targetGameObject(object, object->goal);
		}
	}

	int xSpeed = object->speed;
	int ySpeed = object->speed;
	
	//Make sure the object is moving in the right direction
	if(object->xDist < 0) xSpeed *= -1;
	if(object->yDist < 0) ySpeed *= -1;
	

	//Don't move too far
	if(abs(xSpeed) > abs(object->xDist)) xSpeed = object->xDist;
	if(abs(ySpeed) > abs(object->yDist)) ySpeed = object->yDist;

	object->moveBy(xSpeed, ySpeed);

	//Handle collision correctly
	Door *d = dynamic_cast<Door *>(object->target);
	if(d != nullptr){
		if(SDL_HasIntersection(&object->area, &d->area)){
			checkDoor(object, d, rooms, g, xSpeed, ySpeed);
		}
	}
	
	object->xDist -= xSpeed;
	object->yDist -= ySpeed;

	//Check if the current target has been reached
	if(xSpeed == 0 && ySpeed == 0){
		//If the target was the goal, then we are done
		if(object->target == object->goal){
			object->goal = nullptr;
			delete object->path;
			object->path = nullptr;
			object->target = nullptr;
		}
		else{
			if(d != nullptr){
				if(d->bottom != nullptr){
					checkDoor(object, d, rooms, g, xSpeed, ySpeed);

					if(object->target = d) object->target = nullptr;
				}
				else object->target = nullptr;
			}
			else{
				object->target = nullptr;
			}
		}
	}

	//printf("%d, %d\n", xSpeed, ySpeed);
}
