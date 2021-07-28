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
	Returns all game objects present in the room from a  pathfinding graph
*/
vector<GameObject *> *roomToGraph(Room *r, Graph<GameObject *, int> *g){
	vector<GameObject *> *res = new vector<GameObject *>();

	if(r == nullptr){
		//printf("WARNING: THE ROOM IS A NULLPOINTER!\n");
		return res;
	}

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

GameObject *closestNode(GameObject *obj, Graph<GameObject *, int> *g, vector<Room *> *r){
	GameObject *res = nullptr;
	int dist = INT_MAX;

	int objX = obj->x;
	int objY = obj->y;
	if(obj->image != nullptr){
		objX += obj->image->width / 2;
		objY += obj->image->height / 2;
	}

	//printf("(%d %d) : (%d %d)\n", obj->x, obj->y, objX, objY);

	//for(pair<GameObject *, Node<GameObject *, int> *> p : g->nodes){
	for(GameObject *n : *roomToGraph(whichRoom(r, obj), g)){
		//GameObject *n = p.first;

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
	Finds the shortest path from the specified beacon/room, to the target
*/
vector<GameObject *> *findPathTo(Graph<GameObject *, int> *g, GameObject *from,
								GameObject *to){	
	if(from == nullptr || to == nullptr){
		//printf("WARNING: TRIED TO PATHFIND FROM OR TO A NULL POINTER!\n");
		return nullptr;
	}

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
		if(obj->image == nullptr) c->yDist = obj->y - c->y;
		else c->yDist = (obj->y + obj->image->height) - (c->y + c->area.h);
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

void targetRoom(CharacterObject *c, Room *r){
	c->target = r;

	c->xDist = (r->x + r->area.w / 2) - c->x;
	c->yDist = (r->y + r->area.h) - (c->y + c->area.h);
}

void target(CharacterObject *c, GameObject *obj){
	Door *d = dynamic_cast<Door *>(obj);
	Room *r = dynamic_cast<Room *>(obj);
	if(d != nullptr) targetDoor(c, d);
	else if(r != nullptr) targetRoom(c, r);
	else targetGameObject(c, obj);
}

void blockDoorPath(CharacterObject *object, Door *d, vector<Room *> *rooms,
							Graph<GameObject *, int> *g){

	//Temporaril adjust the pathfinding graph
	Node<GameObject *, int> *n = g->nodes.at(d);
	vector<int> t;
	Room *r = whichRoom(rooms, d);
	for(pair<pair<GameObject *, GameObject *>, Edge<GameObject *, int> *> p :
									n->edges){
		pair<GameObject *, GameObject *> pp = p.first;

		//If the other object is in a separate room, temp block it in the graph
		if(r != whichRoom(rooms, pp.second)){
			t.push_back(p.second->value);
			g->updateEdge(d, pp.second, INT_MAX);
			g->updateEdge(pp.second, d, INT_MAX);
		}
	}

	//printf("\tDone blocking paths, total blocked edges: %d\n", t.size());


	// Forget the current path to the goal
	delete object->path;
	object->path = nullptr;
	object->target = nullptr;
	
	//printf("\tDeleted the old path!\n");

	//Figure out where to go
	GameObject *start = closestNode(object, g, rooms);
	GameObject *end = closestNode(object->goal, g, rooms);
	object->path = findPathTo(g, start, end);

	//printf("\t Found a path \n");

	//Restore the graph
	int i = 0;
	for(pair<pair<GameObject *, GameObject *>, Edge<GameObject *, int> *> p :
									n->edges){
		pair<GameObject *, GameObject *> pp = p.first;

		//Restore all the edges leading out of the room
		if(r != whichRoom(rooms, pp.second)){
			g->updateEdge(d, pp.second, t.at(i));
			g->updateEdge(pp.second, d, t.at(i));
			i++;
		}
	}

	//printf("Restored the path!\n");

	if(object->path == nullptr) return;

	//Set the new target TODO: MIGHT WALK TO AN UNECISSARY DOOR
	target(object, object->path->back());
	object->path->pop_back();
}

void checkDoor(CharacterObject *object, Door *d, vector<Room *> *rooms,
							Graph<GameObject *, int> *g){

	//If this was the last door on the path, or if the real door out is another door
	if(object->path->size() == 0 ||
			whichRoom(rooms, object->path->back()) == whichRoom(rooms, d)){
		object->target = nullptr;
		return;
	}

	//printf("%s is at a door (%d %d)! Path size %d\n", object->name, d->x, d->y,
	//							object->path->size());

	Door *arrival = dynamic_cast<Door *>(object->path->back());
	if(arrival == nullptr){
		printf("WARNING: Couldn't find the door we were going to.");
		printf("Someone probably forgot to initialize the level's");
		printf("path graph properly....\n");
	}


	//Make sure that the room of arrival isn't dangerous
	Room *r = whichRoom(rooms, arrival);
	if(r == nullptr){
		printf("WARNING: The door that %s is accessing", object->name);
		printf(" is outside of a room!\n");
	}
	if((r->flag & CLEARLYFATAL) != 0){
		//printf("Fatality!\n");
		blockDoorPath(object, arrival, rooms, g);
		return;
	}

	if(!d->IsOpen){
		if(d->IsLocked){
			//printf("Door %s is locked\n", d->n);
			blockDoorPath(object, d, rooms, g);
			object->actionTime = -1;
		}
		else{
			//The door is closed, wait to open it
			if(object->actionTime < 0) object->actionTime = 60 / 4;
			else if(object->actionTime > 0) object->actionTime--;
			else{
				//Open the door
				DoorClickPars *data = (DoorClickPars *) d->data;
				d->image = data->open;
				d->IsOpen = true;
				object->actionTime = -1;
			}
		}
	}
	else{

		if(arrival->IsLocked){
			//printf("Door %s (arrival) is locked \n", arrival->n);
			blockDoorPath(object, arrival, rooms, g);
			object->actionTime = -1;
		}
		else{
			//Close the door behind you
			DoorClickPars *data = (DoorClickPars *) d->data;
			d->image = data->closed;
			d->IsOpen = false;

			//Open the door of arrival
			data = (DoorClickPars *) arrival->data;
			arrival->image = data->open;
			arrival->IsOpen = true;
			
			object->moveTo(arrival->x, 
					arrival->y + (arrival->area.h - object->area.h));

			//Remove the second door and the room of arrival from the path
			object->path->pop_back();
			object->target = nullptr;
			//printf("%s is in the next room and ready to go!\n", object->name);
		}
	}
}

bool updateMovement(CharacterObject *object, vector<Room *> *rooms,
							Graph<GameObject *, int> *g){
	//Check if the character wants to move anywhere
	if(object->goal == nullptr) return true;

	//Calculate a path to the target if no such path has been calculated yet
	if(object->path == nullptr){
		GameObject *start = closestNode(object, g, rooms);
		GameObject *end = closestNode(object->goal, g, rooms);
		object->path = findPathTo(g, start, end);

		// If we can't find a path
		if(object->path == nullptr){
			//If it was because we were allready there, then just walk
			if(start == end){
				object->path = new vector<GameObject *>();
				
			}
			
			return false;
		}

		/*printf("%s: ", object->name);
		for(GameObject *go : *object->path){
			printf("%s, ", go->n);
		}
		printf("\n");*/

		//targetDoor(object, sharedDoor(start, object->path->back()));
		if(object->path->size() > 1) {
			/*printf("(%d, %d) -> (%d, %d), char: (%d, %d)\n",
				object->path->back()->x, object->path->back()->y,
				object->path->at(object->path->size() - 2)->x,
				object->path->at(object->path->size() - 2)->y,
				object->x, object->y);*/

			//Check if the two beacons are in the same room 
			if(whichRoom(rooms, object->path->back()) ==
				whichRoom(rooms, object->path->at(
							object->path->size() - 2))){

				//If so, make sure that we don't go the wrong way
				if(object->path->back()->x > object->x &&
					object->path->at(object->path->size() - 2)->x
									< object->x){

					//printf("%s decided to disregard a door\n",
					//				object->name);
					object->path->pop_back();
				}
				else if(object->path->back()->x <= object->x &&
						object->x <= object->path->at(
							object->path->size() - 2)->x){

					//printf("%s decided to disregard a door\n",
					//				object->name);
					object->path->pop_back();
				}
			}

			//printf("\t%s path size is %d before first target!\n",
			//			object->name, object->path->size());
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

	//printf("%s's path is %d\n", object->name, object->path->size());

	if(object->target == nullptr){
		if(!object->path->empty()){
			target(object, object->path->back());
			object->path->pop_back();
		}
		else{
			//The character is in the correct room, so walk to the goal
			target(object, object->goal);
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
			//printf("%s reached their goal!\n", object->name);

			object->goal = nullptr;
			delete object->path;
			object->path = nullptr;
			object->target = nullptr;
			return true;
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
	return false;

	//printf("%d, %d\n", xSpeed, ySpeed);
}
