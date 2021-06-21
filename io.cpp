#include "engine.h"
#include "structs.h"
#include "globals.h"

/*
	Read the next line of text from the specified open file stream.
	At max maxLength text can be read, but the actual size of the line is stored
	in len.

	If the line was properly read, or if the EOF was encountered, the
	function returns true. Otherwise it will return false and report that an error
	had occured
*/
bool fileReadLine(fstream *fs, char *line, int *len, int maxLength){
	fs->getline(line, maxLength);
	*len = fs->gcount();
	if(fs->eof()){
		return true;
	}
	else if(fs->fail()){
		printf("Error reading in text with legth %d!\n", len);
		return false;
	}

	return true;
}

/*
	Takes the given string and splits it around the delimiter " ".
	Returns an array of len substrings, where each substring can at most be
	maxLength long.

	NOTE: String will be truncated to only containe the first substring, and
	the array of substrings (consisting of all substrings) will have to be
	deallocated at some point
*/
char **splitString(char *string, int &len, int maxLength){
	len = 0;

	vector<char *> temp;
	temp.push_back(strtok(string, " "));
	while(temp.at(len) != NULL){
		len += 1;
		temp.push_back(strtok(NULL, " "));
	}
	
	char **result = new char *[len];
	for(int i = 0; i < len; i++){
		result[i] = new char[maxLength];
		strncpy(result[i], temp.at(i), maxLength);
	}
	
	return result;
}

/*
	Initializes the specified media object by loading all the media which filepath
	is specified in the specified manifest file. Returns true if all media was
	loaded correctly, or false if an error was encountered
*/
bool loadMedia(Media *media, const char *path, SDL_Renderer *render){
	const int MAX_FILE_NAME_LENGTH = 1024;

	bool result = true;

	fstream fs;
	fs.open(path, fstream::in);

	char text[MAX_FILE_NAME_LENGTH];
	int mode = 0;
	int len;
	if(!fileReadLine(&fs, text, &len, MAX_FILE_NAME_LENGTH)){
		printf("Error reading file paths from file %s!\n", path);
		result = false;
		len = 0;
	}
	while(len > 0){
		if(mode == 0){
			if(strcmp(text, "IMAGES:") == 0){} //Skip the first line
			else if(strcmp(text, "MUSIC:") == 0) mode++;
			else{
				char **parts = splitString(text, len, len);
				if(len == 3){
					int w = atoi(parts[1]);
					int h = atoi(parts[2]);
					media->images.push_back(new Image(parts[0], w, h,
										render));
				}
				else if(len == 1){
					media->images.push_back(new Image(text, render));
				}
				else{
					printf("Image file %s was badly formated!\n",
										 text);
					result = false;
					break;
				}

				//Cleanup
				for(int i = 0; i < len; i++){
					delete parts[i];
				}				
				delete parts;
			}
		}
		else if(mode == 1){
			if(strcmp(text, "SOUNDS:") == 0) mode++;
			else media->music.push_back(new Music(text));
		}
		else if(mode == 2){
			if(strcmp(text, "FONTS:") == 0) mode++;
			else media->sounds.push_back(new Sound(text));
		}
		else if(mode == 3){
			char **parts = splitString(text, len, len);
			
			if(len != 2){
				printf("Font file %s was badly formated!\n", text);
				result = false;
				break;
			}

			int size = atoi(parts[1]);
			media->fonts.push_back(new Font(text, size));

			//Cleanup
			for(int i = 0; i < len; i++){
				delete parts[i];
			}				
			delete parts;
		}

		if(!fileReadLine(&fs, text, &len, MAX_FILE_NAME_LENGTH)){
			printf("Error reading file paths from file %s!\n", path);
			result = false;
			break;
		}
	}

	fs.close();

	return result;
}
