#pragma warning(disable : 26451)

#include "pch.h"
#include "EntityList.h"

std::map<unsigned int, Entity*> EntityList::Entities;

void EntityList::Refresh()
{
	uintptr_t entList = Addresses::Get("entitylist");

	// Copy the key set into a vector
	std::vector<unsigned int> keys;
	for (auto const& key : Entities)
		keys.push_back(key.first);

	for (unsigned int i = 0; i < MAX_ENTITIES; i++) {
		// Location of each entity in the list
		uintptr_t* ptr = (uintptr_t*)(entList + 0x48 + 0x10 * i);

		if (*ptr != NULL){
			EntityData* data = (EntityData*)*ptr;
			
			// Check if the name ptr is valid and populated. Filter out weapons
			if ((uintptr_t*)(data->Name) != 0x0 && data->Name != "" && strcmp(data->Name, "weaponx")) {
				Entity* c;
				auto search = Entities.find(data->Id);
				
				// Entity not found. Insert a new one
				if (search == Entities.end()) {
					c = new Entity();
					Entities.insert({ data->Id, c });
				}
				else
					c = search->second;

				c->data = data;
				c->SetLastPos(c->data->Position1); // Used for aim prediction

				// Remove this entity from the 'keys to delete' list
				keys.erase(std::remove(keys.begin(), keys.end(), data->Id), keys.end());
			}
		}
	}

	// Remove Entities that no longer exist
	for (auto const& key : keys) {
		delete Entities.find(key)->second;
		Entities.erase(key);
	}
}

void EntityList::Cleanup()
{
	for (auto const& c : Entities)
		delete c.second;
}
