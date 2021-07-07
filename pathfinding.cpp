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

GameObject *closestNode(GameObject *obj, Graph<GameObject *, int> *g){
	GameObject *res = nullptr;
	int dist = INT_MAX;

	int objX = obj->x;
	int objY = obj->y;
	if(obj->image != nullptr){
		objX += obj->image->width / 2;
		objY += obj->image->height / 2;
	}

	//printf("(%d %d) : (%d %d)\n", obj->x, obj->y, objX, objY);

	for(pair<GameObject *, Node<GameObject *, int> *> p : g->nodes){
		GameObject *n = p.first;

		int nX = n->x;
		int nY = n->y;
		if(n->image != nullptr){
			nX += n->image->width / 2;
			nY += n->image->height / 2;
		}
		//printf("\t(%d %d) : (%d %d)\n", n->x, n->y, nX, nY);
		
		//Calculate the euclidean distance between the two objects
		int newDist = sqrt((objX - nX)*(objX - nX) + (objY - nY)*(objY - nY));
		//printf("\tdist: %d newDist: %d\n", dist, newDist);
		if(newDist < dist){
			res = n;
			dist = newDist;
		}
	}

	return res;
}

/*
	Returns all game objects present in the room from a  pathfinding graph
*/
vector<GameObject *> *RoomToGraph(Room *r, Graph<GameObject *, int> *g){
	vector<GameObject *> *res = new vector<GameObject *>();

	for(pair<GameObject *, Node<GameObject *, int> *> p : g->nodes){
		GameObject *obj = p.first;

		SDL_Rect area;
		if(obj->image == nullptr){
			area = {obj->x, obj->y, 1, 1};
		}
		else{
			area = {obj->x, obj->y, obj->image->width, obj->image->height};
		}

		if(SDL_HasIntersection(&r->area, &area)) {
			res->push_back(obj);
		}
	}

	return res;
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
		c->xDist = d->x - c->x;
		c->yDist = (d->y + d->area.h) - (c->y + c->area.h);
	}
	else{ //Target the door
		c->xDist = d->x - (c->x + c->area.w);
		if(c->xDist < 0) c->xDist = (d->x + d->area.w) - c->x;

		c->yDist = (d->y + d->area.h) - (c->y + c->area.h);
	}
}

void target(CharacterObject *c, GameObject *obj){
	Door *d = dynamic_cast<Door *>(obj);
	if(d != nullptr) targetDoor(c, d);
	else targetGameObject(c, obj);
}

void blockDoorPath(CharacterObject *object, Door *d, vector<Room *> *rooms,
							Graph<GameObject *, int> *g){
	//Remember the next object we were heading towards
	GameObject *r = nullptr;
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
	int t = g->getEdgeValue(d, r);
	g->updateEdge(d, r, INT_MAX);
	g->updateEdge(r, d, INT_MAX);

	//Figure out where to go
	//Room *start = whichRoom(rooms, object);
	GameObject *end = closestNode(object->goal, g);
	object->path = findPathTo(g, d, end);

	//Restore the graph
	g->updateEdge(d, r, t);
	g->updateEdge(r, d, t);

	if(object->path == nullptr) return;

	//targetDoor(object, sharedDoor(start, object->path->back()));
	//Room *room = dynamic_cast<Room *>(object->path->back());

	//Remove the door we are standing at from the path
	object->path->pop_back();
	target(object, object->path->back());
	object->path->pop_back();
}

void checkDoor(CharacterObject *object, Door *d, vector<Room *> *rooms,
							Graph<GameObject *, int> *g){

	Door *arrival = dynamic_cast<Door *>(object->path->back());
	if(arrival == nullptr){
		printf("WARNING: Couldn't find the door we were going to.");
		printf("Someone probably forgot to initialize the level's");
		printf("path graph properly....\n");
	}

	//Make sure that the room of arrival isn't dangerous
	Room *r = whichRoom(rooms, arrival);
	if((r->flag & CLEARLYFATAL) != 0){
		blockDoorPath(object, d, rooms, g);
		return;
	}

	if(!d->IsOpen){
		if(d->IsLocked){
			//printf("Door %s is locked\n", d->n);
			blockDoorPath(object, d, rooms, g);
		}
		else{
			//Open the door
			DoorClickPars *data = (DoorClickPars *) d->data;
			d->image = data->open;
			d->IsOpen = true;
		}
	}
	else{

		if(arrival->IsLocked){
			//printf("Door %s (arrival) is locked \n", arrival->n);
			blockDoorPath(object, d, rooms, g);
		}
		else{
			//Close the door behind you
			DoorClickPars *data = (DoorClickPars *) d->data;
			d->image = data->closed;
			d->IsOpen = true;

			//Open the door of arrival
			data = (DoorClickPars *) arrival->data;
			arrival->image = data->open;
			arrival->IsOpen = true;
			
			object->x = arrival->x;
			object->y = arrival->y + (arrival->area.h - object->area.h);

			//Remove the second door and the room of arrival from the path
			object->path->pop_back();
			object->target = nullptr;
		}
	}
}

void updateMovement(CharacterObject *object, vector<Room *> *rooms,
							Graph<GameObject *, int> *g){
	//Check if the character wants to move anywhere
	if(object->goal == nullptr) return;

	//Calculate a path to the target if no such path has been calculated yet
	if(object->path == nullptr){
		//Room *start = whichRoom(rooms, object);
		//Room *end = whichRoom(rooms, object->goal);
		GameObject *start = closestNode(object, g);
		GameObject *end = closestNode(object->goal, g);
		object->path = findPathTo(g, start, end);

		//printf("%p %p\n", start, end);

		//printf("Paul is searching for the way\n");
		if(object->path == nullptr) return;
		//printf("Paul knows the way\n");

		/*for(GameObject *go : *object->path){
			printf("%s\n", go->n);
		}*/

		//targetDoor(object, sharedDoor(start, object->path->back()));
		if(object->path->size() > 1){
			if(object->path->back()->x > object->x &&
			object->path->at(object->path->size() - 2)->x < object->x){
				object->path->pop_back();
			}
			else if(object->path->back()->x < object->x &&
			object->path->at(object->path->size() - 2)->x > object->x){
				object->path->pop_back();
			}

			target(object, object->path->back());
			object->path->pop_back();
		}
		else{
			if(object->path->back()->x > object->x &&
				object->goal->x < object->x){
				object->path->pop_back();
				object->target = nullptr;
			}
			else if(object->path->back()->x < object->x &&
				object->goal->x > object->x){
				object->path->pop_back();
				object->target = nullptr;
			}

		}
	}

	if(object->target == nullptr){
		if(!object->path->empty()){
			target(object, object->path->back());
			object->path->pop_back();
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
	//printf("%d %d\n", xSpeed, ySpeed);

	object->xDist -= xSpeed;
	object->yDist -= ySpeed;
	
	//printf("%d, %d\n", object->xDist, object->yDist);

	//Check if the current target has been reached
	if(object->xDist == 0 && object->yDist == 0){
		//If the target was the goal, then we are done
		if(object->target == object->goal){
			object->goal = nullptr;
			delete object->path;
			object->path = nullptr;
			object->target = nullptr;
		}
		else{
			//Check if door
			Door *d = dynamic_cast<Door *>(object->target);
			if(d != nullptr){
				checkDoor(object, d, rooms, g);
			}
			else{
				object->target = nullptr;
			}
		}
	}

	//printf("%d, %d\n", xSpeed, ySpeed);
}
