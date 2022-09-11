#include "./Entity.hpp"
#include "../Zone/Zone.hpp"
#include "../Engine/Engine.hpp"

Entity::Entity(const char* name, int draw_layer){
    this->name = StrDeepCopy(name);

    this->draw_layer = draw_layer;

    //Initializing animation/visibility attributes
    this->active = true;
    this->visible = true;

    //Initializing attributes
    this->attr = new AttributeHash(64);
}

Entity::Entity(FILE* file){
    if(this->deserializeData(file)){
        if(this->name != nullptr){
            printf("Extended object deserialization of %s failed\n", this->name);
        }
        else{
            printf("Extended object deserialization failed\n");
        }
    }
}

Entity::~Entity(){
    free(name);
}

char* Entity::getName(){
    return this->name;
}

int16_t Entity::getDrawLayer(){
    return this->draw_layer;
}

bool Entity::isActive(){
    return this->active;
}

bool Entity::isVisible(){
    return this->visible;
}

void* Entity::getAttr(const char* key){
    return this->attr->get(key);
}

void Entity::setActive(bool active){
    this->active = active;
}

void Entity::setVisible(bool visible){
    this->visible = visible;
}

void Entity::setAttr(const char* key, bool val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_BOOL);
}

void Entity::setAttr(const char* key, char val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_CHAR);
}

void Entity::setAttr(const char* key, int64_t val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_INT);
}

void Entity::setAttr(const char* key, uint64_t val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_UINT);
}

void Entity::setAttr(const char* key, char* val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_STRING);
}

uint32_t Entity::getType(){
    return this->type;
}

int Entity::serializeData(FILE* file, Zone* base_zone){
    if(file == nullptr){
        return -1;
    }

    //GENERAL SECTION
    //Write the entity type
    WriteVar(this->type, uint32_t, file);

    //Identifier
    uint16_t identifier_len = strlen(this->name);
    WriteVar(identifier_len, uint16_t, file);
    fwrite(this->name, 1, identifier_len, file);

    //Draw layer
    WriteVar(draw_layer, int16_t, file);

    //ATTRIBUTE SECTION
    Attribute* attr_list;
    uint32_t num_entries = attr->dump(&attr_list);
    WriteVar(num_entries, uint32_t, file);

    while(attr_list != nullptr){
        //Attribute type
        uint16_t attr_type = attr_list->type;
        WriteVar(attr_type, uint16_t, file);

        //Write the key info
        uint16_t attr_key_len = strlen(attr_list->key);
        WriteVar(attr_key_len, uint16_t, file);
        fwrite(attr_list->key, attr_key_len, 1, file);

        //Write the val info
        switch(attr_type){
            //Writing chars & bools (1 byte)
            case ATTR_DATA_TYPE::DATA_BOOL:
            case ATTR_DATA_TYPE::DATA_CHAR:{
                uint8_t attr_val = (uint8_t)((uint64_t)attr_list->val & 0xFF);
                WriteVar(attr_val, uint8_t, file);
                break;
            }

            //Writing ints/uints (8 bytes)
            case ATTR_DATA_TYPE::DATA_INT:
            case ATTR_DATA_TYPE::DATA_UINT:{
                WriteVar((uint64_t)attr_list->val, uint64_t, file);
                break;
            }

            //Writing strings (? bytes)
            case ATTR_DATA_TYPE::DATA_STRING:{
                char* attr_str = (char*)attr_list->val;
                uint16_t attr_val_len = strlen(attr_str);
                WriteVar(attr_val_len, uint16_t, file);
                fwrite(attr_str, 1, attr_val_len, file);
                break;
            }
        }
    }

    return 0;
}

int Entity::deserializeData(FILE* file){
    if(file == nullptr){
        return -1;
    }

    //Entity type
    ReadVar(this->type, file);

    //Identifier
    uint16_t identifier_len;
    ReadVar(identifier_len, file);
    this->name = (char*)malloc(identifier_len + 1);
    fread(&this->name, sizeof(char), identifier_len, file);
    name[identifier_len] = '\0';

    //Draw layer
    ReadVar(this->draw_layer, file);

    //ATTRIBUTE SECTION
    uint32_t num_entries;
    ReadVar(num_entries, file);

    for(uint32_t i = 0; i < num_entries; i++){
        //Attribute type
        uint16_t attr_type;
        ReadVar(attr_type, file);

        //Attribute key
        uint16_t attr_key_len;
        ReadVar(attr_key_len, file);
        char attr_key[attr_key_len + 1];
        fread(attr_key, attr_key_len, 1, file);
        attr_key[attr_key_len] = '\0';

        switch(attr_type){
            //Reading chars & bools (1 byte)
            case ATTR_DATA_TYPE::DATA_BOOL:{
                bool attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }
            case ATTR_DATA_TYPE::DATA_CHAR:{
                char attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }

            //Reading ints/uints (8 bytes)
            case ATTR_DATA_TYPE::DATA_INT:{
                int64_t attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }
            case ATTR_DATA_TYPE::DATA_UINT:{
                uint64_t attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }

            //Reading strings (? bytes)
            case ATTR_DATA_TYPE::DATA_STRING:{
                uint16_t attr_str_len;
                ReadVar(attr_str_len, file);
                char attr_str[attr_str_len + 1];
                fread(attr_str, 1, attr_str_len, file);
                attr_str[attr_str_len] = '\0';
                this->setAttr(attr_key, attr_str);
                break;
            }
        }
    }

    return 0;
}

int Entity::serializeAssets(FILE* file, SerializeSet& serialize_set){
    if(file == nullptr){
        return -1;
    }

    return 0;
}